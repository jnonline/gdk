/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Asset.h"
#include "AssetPool.h"
#include "AssetProvider.h"

#include "../System/Containers/StringHashMap.h"

namespace Gdk
{
	// ============================================================================

	class AssetManager
	{
	public:

		// Types
		// --------------------------------

		
		typedef StringHashMap<AssetBase*>			NamedAssetsMap;
		typedef map<int, NamedAssetsMap>			TypeNamedAssetsMap;

		// Work Items
		struct WorkItem
		{
			// Properties
			AssetBase* Asset;
			int Priority;

			// CTor
			WorkItem() : Asset(NULL), Priority(0) {}
			WorkItem(AssetBase* asset, int priority) : Asset(asset), Priority(priority) {}
			inline bool operator< (const WorkItem& input) const { return this->Priority > input.Priority; } // Higher priority = earlier sorting
		};
		typedef priority_queue<WorkItem>			WorkItemQueue;

		// Provider Registrations
		struct ProviderRegistration
		{
			AssetProvider* Provider;
			int Priority;
			bool AutoLookup;

			// CTor
			ProviderRegistration(AssetProvider* provider, int priority, bool autoLookup) : Provider(provider), Priority(priority), AutoLookup(autoLookup) {};
			inline bool operator< (const ProviderRegistration& input) const { return this->Priority > input.Priority; }  // Higher priority = earlier sorting
		};
		typedef SortedVector<ProviderRegistration>		ProviderRegistrationSet;
	
	public:

		// Public Methods
		// --------------------------------

		// CTor: Creates a new AssetManager. 
		//	 backgroundWorkerThreads:		Number of threads to use for background (Async) asset loading 
		//	 useBaseFileAssetProvider:		If true (default), a basic FileAssetProvider for the 'Base' asset folder will be added to the manager
		AssetManager(
			int backgroundWorkerThreads = 1,
			bool useBaseFileAssetProvider = true
			);

		// Dtor
		~AssetManager();

		// Provider Registration & Access
		void RegisterProvider(AssetProvider* provider, int priority = 0, bool autoLookup = true);
		void UnRegisterProvider(AssetProvider* provider);
		const ProviderRegistrationSet& GetRegisteredProviders() { return this->registeredProviders; }

		// Loads an asset immediately (halting the current thread until the asset is loaded)
		//   name:				name/path of the asset
		//	 assetPool:			AssetPool that will manage the lifetime of the asset  (if NULL, the asset will exist until ReleaseAll() is called or the asset manager is destroyed)
		//   specificProvider:	provider to use when loading the asset. passing NULL will tell the asset manager to query the registered providers, in priority order.  (default = NULL)
		template<class T>
		T* Load(const char* name, AssetPool* assetPool, AssetProvider* specificProvider = NULL);

		// Loads an asset immediately (halting the current thread until the asset is loaded)
		//   name:				name/path of the asset
		//   specificProvider:	provider to use when loading the asset. passing NULL will tell the asset manager to query the registered providers, in priority order.  (default = NULL)
		//
		template<class T>
		Asset<T>* Load(const char* name, AssetProvider* specificProvider = NULL);

		// Loads the asset asyncronously.  The asset will be queued if it isnt already loaded.
		//   name:				name/path of the asset
		//	 priority:			priority of the asset load.  Higher priority items will be loaded first by the background load threads
		//   specificProvider:	provider to use when loading the asset. passing NULL will tell the asset manager to query the registered providers, in priority order.  (default = NULL)
		//
		template<class T>
		Asset<T>* LoadAsync(const char* name, int priority = 0, AssetProvider* specificProvider = NULL);

		// Stop the background worker threads
		void StopBackgroundWorkers();

		// Singleton
		static AssetManager* GetSingleton();

	protected:

		// Application Interface
		// ---------------------------------------

		friend class Application;
		static void InitSingleton(int numBackgroundThreads);
		static void ShutdownSingleton();
		static AssetManager* singleton;

		// Internal Methods
		// ---------------------------------------

		// Background worker thread function
		static void* BackgroundWorker(void* data);

		// Loads the asset from the an asset provider.  Puts the asset into the "Ready" state
		void PerformAssetLoad(AssetBase* asset);

		// Releases all the assets
		void ReleaseAll();

		// Destroys the asset.  Removing it from all internal maps & deleting it. 
		//   NOTE: This should ONLY be called by the Asset->Release() method when refcount = 0
		void Destroy(AssetBase* asset);
		friend class AssetBase;

		// Looks up the given asset in the asset map.  Returns NULL if the asset doesnt exist
		template<class T>
		Asset<T>* LookupAsset(const char* name);

		// Internal Properties
		// ----------------------------------

		// Thread sync
		Mutex* assetMapMutex;
		Mutex* workItemQueueMutex;
		Event* backgroundThreadSignalEvent;
		bool backgroundWorkerStopRequest;

		vector<Thread> threads;

		// The main assets map
		TypeNamedAssetsMap assetsMap;

		// Priority Queue of work items
		WorkItemQueue workItemQueue;

		// Registered providers
		ProviderRegistrationSet registeredProviders;

		// Background processing threads
		vector<Thread> backgroundThreads;

		// This flag signifies that a "Release All" operation is being performed
		bool performingReleaseAll;
	};	

	// ===================================================================================================
	// Template Implementations
	// ===================================================================================================

	// ******************************************************************
	template<class T>
	Asset<T>* AssetManager::LookupAsset(const char* name)
	{
		Asset<T>* asset = NULL;

		// Get the Type of the asset
		AssetType::Enum assetType = T::ASSET_TYPE;

		// Get the named asset map for the given type   (one will be created if it doesnt exist)
		NamedAssetsMap& assetsByName = assetsMap[(int)assetType];

		// Find the asset
		NamedAssetsMap::Iterator assetIter = assetsByName.Find(name);
		if(assetIter != assetsByName.End())
		{
			// The asset already exists
			asset = (Asset<T>*) assetIter->second;

			// Validate the asset
			ASSERT(asset->type == assetType, L"Asset types do not match after the asset was retrieved by it's name hash");	
		}

		// Return the asset
		return asset;
	}

	// ******************************************************************
	template<class T>
	T* AssetManager::Load(const char* name, AssetPool* assetPool, AssetProvider* specificProvider)
	{
		// Load the asset
		Asset<T>* asset = Load<T>(name, specificProvider);

		// Put the asset into the pool
		if(assetPool != NULL)
			assetPool->Add(asset);

		// Return the asset instance
		return asset->GetInstance();
	}

	// ******************************************************************
	template<class T>
	Asset<T>* AssetManager::Load(const char* name, AssetProvider* specificProvider)
	{
		// lock the asset map mutex
		assetMapMutex->Lock();

		// Lookup the Asset if it already exists
		Asset<T>* asset = LookupAsset<T>(name);

		// Do we already have an Asset* for this asset?
		if(asset != NULL)
		{
			// Add a reference to the asset
			asset->AddRef();

			// Is the asset ready or loading?
			if(asset->state != AssetState::Created)
			{
				// Unlock the mutex
				assetMapMutex->Unlock();

				// Is the asset is currently loading?
				if(asset->state == AssetState::Loading)
				{
					// Wait for the asset to finish loading
					// TODO(P2): Use proper eventing to signal the completion   [Note: this only happens when Load() is called on an asset already queued & being processed by a LoadAsync() call]
					while(asset->state != AssetState::Ready)
					{
						Thread::Sleep(1);
					}
				}

				// Return the existing asset 
				return asset;
			}

			// The asset is created, but hasnt been processed yet.  So we'll process it
		}
		else
		{
			// Create the new Asset* for this asset
			asset = GdkNew Asset<T>(this, name);
			asset->provider = specificProvider;
			asset->AddRef();
			
			// Add the asset to the map
			NamedAssetsMap& assetsByName = assetsMap[(int)asset->GetType()];
			assetsByName.Add(name, asset);
		}

		// Set the state to "Loading"
		asset->state = AssetState::Loading;

		// Unlock the mutex
		assetMapMutex->Unlock();

		// Do the actual asset loading
		PerformAssetLoad( asset );

		return asset;
	}

	// ******************************************************************
	template<class T>
	Asset<T>* AssetManager::LoadAsync(const char* name, int priority, AssetProvider* specificProvider)
	{
		// lock the asset map mutex
		assetMapMutex->Lock();

		// Lookup the Asset if it already exists
		Asset<T>* asset = LookupAsset<T>(name);
		
		// Does the asset already exist?
		if(asset != NULL)
		{
			// Add a reference to the asset
			asset->AddRef();

			// Unlock the mutex
			assetMapMutex->Unlock();

			// Return the asset
			return asset;
		}

		// Create the new Asset* for this asset
		asset = GdkNew Asset<T>(this, name);
		asset->provider = specificProvider; 
		asset->state = AssetState::Created;
		asset->AddRef();
		
		// Add the asset to the map
		NamedAssetsMap& assetsByName = assetsMap[(int)asset->GetType()];
		assetsByName.Add(name, asset);

		// Unlock the mutex
		assetMapMutex->Unlock();

		// Queue the work item
		// -------------------------

		// Lock the work-item queue mutex
		workItemQueueMutex->Lock();

		// Add the asset to the work items queue
		workItemQueue.push(WorkItem(asset, priority));

		// Unlock the work-item queue mutex
		workItemQueueMutex->Unlock();

		// Signal that async work is available
		backgroundThreadSignalEvent->Set();

		return asset;
	}

	// ******************************************************************

} // namespace 
