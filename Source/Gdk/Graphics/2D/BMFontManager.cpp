/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "BMFontManager.h"

using namespace Gdk;

// Static Instantiations
BMFontManager* BMFontManager::singleton = NULL;

// *****************************************************************
/// @brief
///     Constructor
// *****************************************************************
BMFontManager::BMFontManager()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
BMFontManager::~BMFontManager()
{
}

// *****************************************************************
/// @brief
///     Creates an BMFont resource from the given Gdk BMFont asset 
/// @param name
///     Name of the Gdk BMFont asset.  This will also be the resource name.
/// @param async
///     If true, the asset will be loaded by a background thread.  (Default = false)
/// @param height
///     The priority for an asyncronous load.  Higher priority items are processed first.
// *****************************************************************
BMFont* BMFontManager::FromAsset(const char *name, bool async, int asyncPriority)
{
    return (BMFont*) singleton->LoadUtility(name, async, asyncPriority, &BMFontManager::PerformLoadFromAsset);
}

// *****************************************************************
/// @brief
///     Gets a new BMFont instance for the base ResourceManager.
/// @remarks
///     GDK Internal use only
// *****************************************************************
Resource* BMFontManager::OnCreateNewResourceInstance()
{
    return new BMFont();
}

// *****************************************************************
void BMFontManager::PerformLoadFromAsset(Resource* resource)
{
    // Get the BMFont*
    BMFont* bmFont = (BMFont*)resource;
    
    // Load the BMFont from it's corresponding asset
    bmFont->LoadFromAsset();
}