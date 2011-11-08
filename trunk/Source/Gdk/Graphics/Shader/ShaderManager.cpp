/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ShaderManager.h"

using namespace Gdk;

// Static Instantiations
ShaderManager* ShaderManager::singleton = NULL;

// *****************************************************************
/// @brief
///     Constructor
// *****************************************************************
ShaderManager::ShaderManager()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
ShaderManager::~ShaderManager()
{
}

// *****************************************************************
/// @brief
///     Creates an Shader resource from the given Gdk Shader asset 
/// @param name
///     Name of the Gdk Shader asset.  This will also be the resource name.
/// @param async
///     If true, the asset will be loaded by a background thread.  (Default = false)
/// @param height
///     The priority for an asyncronous load.  Higher priority items are processed first.
// *****************************************************************
Shader* ShaderManager::FromAsset(const char *name, bool async, int asyncPriority)
{
    return (Shader*) singleton->LoadUtility(name, async, asyncPriority, &ShaderManager::PerformLoadFromAsset);
}

// *****************************************************************
/// @brief
///     Gets a new Shader instance for the base ResourceManager.
/// @remarks
///     GDK Internal use only
// *****************************************************************
Resource* ShaderManager::OnCreateNewResourceInstance()
{
    return new Shader();
}

// *****************************************************************
void ShaderManager::PerformLoadFromAsset(Resource* resource)
{
    // Get the Shader*
    Shader* shader = (Shader*)resource;
    
    // Load the Shader from it's corresponding asset
    shader->LoadFromAsset();
}