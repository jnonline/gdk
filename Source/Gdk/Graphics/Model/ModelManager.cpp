/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ModelManager.h"

using namespace Gdk;

// Static Instantiations
ModelManager* ModelManager::singleton = NULL;

// *****************************************************************
/// @brief
///     Constructor
// *****************************************************************
ModelManager::ModelManager()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
ModelManager::~ModelManager()
{
}

// *****************************************************************
/// @brief
///     Creates an Model resource from the given Gdk Model asset 
/// @param name
///     Name of the Gdk Model asset.  This will also be the resource name.
/// @param async
///     If true, the asset will be loaded by a background thread.  (Default = false)
/// @param height
///     The priority for an asyncronous load.  Higher priority items are processed first.
// *****************************************************************
Model* ModelManager::FromAsset(const char *name, bool async, int asyncPriority)
{
    return (Model*) singleton->LoadUtility(name, async, asyncPriority, &ModelManager::PerformLoadFromAsset);
}

// *****************************************************************
/// @brief
///     Gets a new Model instance for the base ResourceManager.
/// @remarks
///     GDK Internal use only
// *****************************************************************
Resource* ModelManager::OnCreateNewResourceInstance()
{
    return new Model();
}

// *****************************************************************
void ModelManager::PerformLoadFromAsset(Resource* resource)
{
    // Get the Model*
    Model* model = (Model*)resource;
    
    // Load the Model from it's corresponding asset
    model->LoadFromAsset();
}