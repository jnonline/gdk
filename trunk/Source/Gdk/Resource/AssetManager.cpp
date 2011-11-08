/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "AssetManager.h"

using namespace Gdk;

// Static Instantiations
AssetManager::ProviderRegistrationSet AssetManager::registeredProviders;

// *****************************************************************
/// @brief
///     Static Initialization of the AssetManager
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void AssetManager::Init()
{
    // Setup the "Base" FileAssetProvider
    // ----------------------------------------
    
    // Get the root "Assets" folder
    string assetsFolder = Path::GetCommonPath(CommonPaths::AssetFolder);
    
    // Create a File System provider for the "Base" bundle
    string baseFolder = Path::Combine(assetsFolder.c_str(), "Base");
    FileAssetProvider* provider = GdkNew FileAssetProvider(baseFolder.c_str());
    RegisterProvider(provider, 0);
}

// *****************************************************************
/// @brief
///     Static Shutdown of the AssetManager
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void AssetManager::Shutdown()
{
    // Loop through the registered providers
	for(ProviderRegistrationSet::iterator iter = registeredProviders.begin(); iter != registeredProviders.end(); iter++)
	{
        // Delete the provider
        GdkDelete( iter->Provider );
    }
    registeredProviders.clear();
}

// *****************************************************************
/// @brief
///     Registers a new provider to be used when searching for assets
/// @param provider
///     The provider to register.  The AssetManager assumes ownership of the provider, and will delete it when shutting down
/// @param priority
///     The priority of the provider registration.  Higher priority providers are searched first.
// *****************************************************************
void AssetManager::RegisterProvider(AssetProvider* provider, int priority)
{
	// Add the provider (sorted by priority)
	registeredProviders.AddSorted( ProviderRegistration(provider, priority) );
}

// *****************************************************************
/// @brief
///     Unregisters the given provider
/// @param provider
///     The provider to unregister.
/// @param deleteProvider
///     If true, the provider will be deleted after unregistering.  
///     If false, the caller is assumed to take ownership of the provider pointer.
// *****************************************************************
void AssetManager::UnRegisterProvider(AssetProvider* provider, bool deleteProvider)
{
	// Loop through the registered providers
	for(ProviderRegistrationSet::iterator iter = registeredProviders.begin(); iter != registeredProviders.end(); iter++)
	{
		// Is this the provider?
		if(iter->Provider == provider)
		{
			// Remove the provider
			registeredProviders.erase(iter);
            
            // Does the caller want to delete the provider?
            if(deleteProvider)
                GdkDelete( provider );
            
			return; 
		}
	}
}

// *****************************************************************
/// @brief
///     Gets a vector of all the providers registered with the asset manager
// *****************************************************************
const AssetManager::ProviderRegistrationSet& AssetManager::GetRegisteredProviders() 
{ 
    return registeredProviders; 
}

// *****************************************************************
/// @brief
///     Gets a Stream* to the asset at the given asset path.
/// @param assetPath
///     Path to the asset.  This path is assumed to be relative to the source AssetProvider.
///     For FileAssetProvider's, the path is relative to the RootFolder
///     For ZipAssetProvider's, the path is relative to the ZIP root.
// *****************************************************************
Stream* AssetManager::GetAssetStream(const char* assetPath)
{
    // Loop through the registered providers in priority order
    for(ProviderRegistrationSet::iterator providerIter = registeredProviders.begin(); providerIter != registeredProviders.end(); providerIter++)
    {
        // Attempt to get the stream through this provider
        AssetProvider* provider = providerIter->Provider;
        Stream* stream = provider->GetAssetStream(assetPath);
        
        // Did we get the stream?
        if(stream != NULL)
        {
            return stream;
        }
        
        // Otherwise, continue on to the next provider
    }
    
    return NULL;
}
