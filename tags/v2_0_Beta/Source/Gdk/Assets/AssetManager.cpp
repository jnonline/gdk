/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "AssetManager.h"

using namespace Gdk;

// Static Instantiations
AssetManager* AssetManager::singleton = NULL;

// ***********************************************************************
AssetManager::AssetManager(int backgroundWorkerThreads, bool useBaseFileAssetProvider)
{
	// Default values
	performingReleaseAll = false;

	// Create the thread sync objects
	assetMapMutex = Mutex::Create();
	workItemQueueMutex = Mutex::Create();
	backgroundThreadSignalEvent = Event::Create();
	backgroundWorkerStopRequest = false;

	// Create the FileSystemProvider for the "Base" assets?
	if(useBaseFileAssetProvider)
	{
		// Create a file system provider for the common Assets folder
		string assetsFolder = Path::GetCommonPath(CommonPaths::AssetFolder);
		FileAssetProvider* provider = GdkNew FileAssetProvider(assetsFolder.c_str());

		// Add the "Base" bundle
		provider->AddBundle("Base", 0);

		// Register the provider
		this->RegisterProvider(provider, 0);
	}

	// Loop through the requested thread count
	for(int threadIndex=0; threadIndex < backgroundWorkerThreads; threadIndex++)
	{
		// Create a background worker thread
		threads.push_back(
			Thread::Create(&AssetManager::BackgroundWorker, (void*)this, false)
			);
	}
}

// ***********************************************************************
AssetManager::~AssetManager()
{
	// Stop the background threads
	this->StopBackgroundWorkers();

	// Release all assets
	this->ReleaseAll();

	// Delete the mutex
	GdkDelete(assetMapMutex);
}

// ***********************************************************************
AssetManager* AssetManager::GetSingleton()
{
	return singleton;
}

// ***********************************************************************
void AssetManager::InitSingleton(int numBackgroundThreads)
{
	// Create the singleton instance
	if(singleton == NULL)
	{
		singleton = GdkNew AssetManager(numBackgroundThreads);
	}
}

// ***********************************************************************
void AssetManager::ShutdownSingleton()
{
	// Delete the singleton
	GdkDelete( singleton );
}

// ***********************************************************************
void AssetManager::RegisterProvider(AssetProvider* provider, int priority, bool autoLookup)
{
	// Add the provider (sorted by priority)
	this->registeredProviders.AddSorted(ProviderRegistration(provider, priority, autoLookup));
}

// ***********************************************************************
void AssetManager::UnRegisterProvider(AssetProvider* provider)
{
	// Loop through the registered providers
	for(ProviderRegistrationSet::iterator iter = this->registeredProviders.begin(); iter != this->registeredProviders.end(); iter++)
	{
		// Is this the provider?
		if(iter->Provider == provider)
		{
			// Remove the provider
			this->registeredProviders.erase(iter);
			return; 
		}
	}
}

// ***********************************************************************
void AssetManager::PerformAssetLoad(AssetBase* asset)
{
	Stream* stream = NULL;
	int providersSearched = 0;

	// Is the asset NOT using a specific provider?
	if(asset->provider == NULL)
	{
		// Loop through the providers in priority order
		for(ProviderRegistrationSet::iterator providerIter = this->registeredProviders.begin(); providerIter != this->registeredProviders.end(); providerIter++)
		{
			// Ignore non-auto search providers
			if(providerIter->AutoLookup == false)
				continue;

			// Attempt to get the stream through this provider
			AssetProvider* provider = providerIter->Provider;
			stream = provider->GetStream(asset->name.c_str(), asset->type);
			providersSearched++;

			// Did we get the stream?
			if(stream != NULL)
			{
				asset->provider = provider;
				break;
			}

			// Otherwise, continue on to the next provider
		}
	}
	else
	{
		// Attempt to get the stream through the specific provider
		stream = asset->provider->GetStream(asset->name.c_str(), asset->type);
	}

	// Did we fail to get a stream?
	if(stream == NULL)
	{
		// Set the asset to "LoadFailed"
		asset->state = AssetState::LoadFailed;

		// Log the error
		if(asset->provider == NULL)
			LOG_ERROR(L"Can not find asset: %hs.  [Searched %d auto-lookup providers]", asset->name.c_str(), providersSearched);
		else
			LOG_ERROR(L"Can not find asset: %hs.  [SpecificProvider: %hs]", asset->name.c_str(), asset->provider->GetName().c_str());
		return;
	}

	// Create an AssetLoadContext
	AssetLoadContext context;
	context.AssetName = asset->name.c_str();
	context.AssetStream = stream;
	context.Asset = asset;
	context.Provider = asset->provider;
	context.Manager = this;

	// Load the asset from the stream
	asset->InternalLoadAsset(&context);

	// Close the stream
	stream->Close();

	// Did the asset load fail?
	if(asset->GetUnknownInstance() == NULL)
	{
		LOG_ERROR(L"Asset Load Failed: %hs.  [Provider: %hs]", asset->name.c_str(), asset->provider->GetName().c_str());
		asset->state = AssetState::LoadFailed;
	}
	else
	{
		// Success!
		asset->state = AssetState::Ready;
	}
}

// ***********************************************************************
void AssetManager::ReleaseAll()
{
	// Lock the asset map & work item queues
	assetMapMutex->Lock();
	workItemQueueMutex->Lock();

	// Disable recursive deletions (as we are deleting Everything)
	this->performingReleaseAll = false;

	// Loop through the asset type map
	for(TypeNamedAssetsMap::iterator typeIter = this->assetsMap.begin(); typeIter != this->assetsMap.end(); typeIter++)
	{
		NamedAssetsMap& namedAssets = typeIter->second;

		// Loop through the assets
		for(NamedAssetsMap::Iterator assetIter = namedAssets.Begin(); assetIter != namedAssets.End(); assetIter++)
		{
			AssetBase* asset = assetIter->second;

			// delete the asset
			GdkDelete( asset );
		}

		// Clear the map
		namedAssets.Clear();
	}

	// Clear the typemap
	this->assetsMap.clear();

	// Clear any queue'd work items
	this->workItemQueue = WorkItemQueue();

	// Re-enable recursive deletions
	this->performingReleaseAll = false;

	// Unlock the asset map & work item queues
	workItemQueueMutex->Unlock();
	assetMapMutex->Unlock();
}

// ***********************************************************************
void AssetManager::Destroy(AssetBase* asset)
{
	// We dont allow destroy calls during a "ReleaseAll" call
	if(performingReleaseAll == true)
		return;

	// Lock the asset map 
	assetMapMutex->Lock();
	
	// Find the named-asset map for the given asset
	NamedAssetsMap& assetsByName = assetsMap[(int)asset->GetType()];

	// Find the asset in the name map
	NamedAssetsMap::Iterator assetIter = assetsByName.Find(asset->name.c_str());
	if(assetIter != assetsByName.End())
	{
		// Remove the asset from the map
		assetsByName.Remove(assetIter);
	}

	// Unlock the asset map
	assetMapMutex->Unlock();

	// Delete the asset
	GdkDelete( asset );
}

// ***********************************************************************
void* AssetManager::BackgroundWorker(void* data)
{
	// Get the asset manager (passed in through the void*)
	AssetManager* assetManager = (AssetManager*)data;

	// Loop forever
	while(1)
	{
		// Wait for the 'work' signal
		assetManager->backgroundThreadSignalEvent->Wait(false);

		// Loop while we have work to do
		while(1)
		{
			// Lock the work item queue mutex
			assetManager->workItemQueueMutex->Lock();

			// Is there a stop request in order?
			if(assetManager->backgroundWorkerStopRequest == true)
			{
				// Unlock the work item queue mutex
				assetManager->workItemQueueMutex->Unlock();
				
				// Stop the thread
				Thread::Exit(0);
			}

			// Is the work item queue empty?
			if(assetManager->workItemQueue.empty() == true)
			{
				// Reset the work item signal event
				assetManager->backgroundThreadSignalEvent->Reset();

				// Unlock the work item queue mutex
				assetManager->workItemQueueMutex->Unlock();

				// Stop working
				break;
			}

			// Get the next work item off the queue
			WorkItem workItem = assetManager->workItemQueue.top();
			assetManager->workItemQueue.pop();
			
			// Unlock the work item queue mutex
			assetManager->workItemQueueMutex->Unlock();
			
			// Lock the asset map mutex
			assetManager->assetMapMutex->Lock();

			// Has this asset already been loaded?
			if(workItem.Asset->state != AssetState::Created)
			{
				// Unlock the asset map mutex
				assetManager->assetMapMutex->Unlock();

				// Stop working
				break;
			}

			// Set the asset state to "Loading"
			workItem.Asset->state = AssetState::Loading;

			// Unlock the asset map mutex
			assetManager->assetMapMutex->Unlock();

			// Load the asset in the work item
			assetManager->PerformAssetLoad( workItem.Asset );

		} // while(1) - working loop

	} // while(1) - forever loop
}

// ***********************************************************************
void AssetManager::StopBackgroundWorkers()
{
	// Lock the work item queue mutex
	this->workItemQueueMutex->Lock();

	// Set the stop request
	this->backgroundWorkerStopRequest = true;

	// Signal to all the worker threads
	this->backgroundThreadSignalEvent->Set();

	// Unlock the work item queue mutex
	this->workItemQueueMutex->Unlock();


	// Join the worker threads
	for(size_t i=0; i<threads.size(); i++)
	{
		threads[i].Join();
	}	
}