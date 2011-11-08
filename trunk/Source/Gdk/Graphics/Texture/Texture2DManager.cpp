/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Texture2DManager.h"

using namespace Gdk;

// Static Instantiations
Texture2DManager* Texture2DManager::singleton = NULL;

// *****************************************************************
/// @brief
///     Constructor
// *****************************************************************
Texture2DManager::Texture2DManager()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
Texture2DManager::~Texture2DManager()
{
}
    
// *****************************************************************
/// @brief
///     Creates a new Texture2D resource with the given width, height, and pixel format
/// @param name
///     Unique name of the resource for this Texture
/// @param width
///     Width of the texture
/// @param height
///     Height of the texture 
/// @param pixelFormat
///     Pixel format of the texture
/// @remarks
///     The texture memory will be allocated in the devices video memory.
///     The filtering mode will default to Linear
///     The wrap mode will default to Repeat for PoT textures and Clamp for NPoT textures
// *****************************************************************
Texture2D* Texture2DManager::Create(const char* name, int width, int height, PixelFormat::Enum pixelFormat)
{
    // Create the Texture Resource
    Texture2D* texture = (Texture2D*) singleton->CreateResource(name);
    
    // Initialize the texture
    texture->Initialize(width, height, pixelFormat);
    
    // Set the state to 'Ready'
    texture->State = ResourceState::Ready;
    
    return texture;
}

// *****************************************************************
/// @brief
///     Creates a Texture2D resource from the given Gdk Image asset 
/// @param name
///     Name of the Gdk Image asset.  This will also be the resource name.
/// @param async
///     If true, the asset will be loaded by a background thread.  (Default = false)
/// @param height
///     The priority for an asyncronous load.  Higher priority items are processed first.
// *****************************************************************
Texture2D* Texture2DManager::FromAsset(const char *name, bool async, int asyncPriority)
{
    return (Texture2D*) singleton->LoadUtility(name, async, asyncPriority, &Texture2DManager::PerformLoadFromAsset);
}

// *****************************************************************
/// @brief
///     Gets a new Texture2D instance for the base ResourceManager.
/// @remarks
///     GDK Internal use only
// *****************************************************************
Resource* Texture2DManager::OnCreateNewResourceInstance()
{
    return new Texture2D();
}

// *****************************************************************
void Texture2DManager::PerformLoadFromAsset(Resource* resource)
{
    // Get the Texture*
    Texture2D* texture = (Texture2D*)resource;
    
    // Load the texture from it's corresponding asset
    texture->LoadFromAsset();
}



