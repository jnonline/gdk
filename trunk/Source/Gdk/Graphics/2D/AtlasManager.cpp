/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "AtlasManager.h"

using namespace Gdk;

// Static Instantiations
AtlasManager* AtlasManager::singleton = NULL;

// *****************************************************************
/// @brief
///     Constructor
// *****************************************************************
AtlasManager::AtlasManager()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
AtlasManager::~AtlasManager()
{
}

// *****************************************************************
/// @brief
///     Creates an Atlas resource from the given Gdk Atlas asset 
/// @param name
///     Name of the Gdk Atlas asset.  This will also be the resource name.
/// @param async
///     If true, the asset will be loaded by a background thread.  (Default = false)
/// @param height
///     The priority for an asyncronous load.  Higher priority items are processed first.
// *****************************************************************
Atlas* AtlasManager::FromAsset(const char *name, bool async, int asyncPriority)
{
    return (Atlas*) singleton->LoadUtility(name, async, asyncPriority, &AtlasManager::PerformLoadFromAsset);
}

// *****************************************************************
/// @brief
///     Gets a new Atlas instance for the base ResourceManager.
/// @remarks
///     GDK Internal use only
// *****************************************************************
Resource* AtlasManager::OnCreateNewResourceInstance()
{
    return new Atlas();
}

// *****************************************************************
void AtlasManager::PerformLoadFromAsset(Resource* resource)
{
    // Get the Atlas*
    Atlas* atlas = (Atlas*)resource;
    
    // Load the atlas from it's corresponding asset
    atlas->LoadFromAsset();
}

