/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Asset.h"

using namespace Gdk;


// ***********************************************************************
AssetBase::AssetBase(AssetManager* assetManager, const char* assetName, AssetType::Enum assetType) 
	: manager(assetManager), name(assetName), type(assetType)
{
	state = AssetState::Created;
	referenceCount = 0;
}

// ***********************************************************************
int AssetBase::AddRef()
{
	return ++this->referenceCount;
}

// ***********************************************************************
int AssetBase::Release()
{
	// Decrement the ref count
	this->referenceCount--;
	int tempRefCount = this->referenceCount;

	// Tell the asset to release it's child assets
	ReleaseChildAssets();

	// Did the ref count hit 0?
	if(this->referenceCount <= 0)
	{
		// Tell the asset manager to destroy the asset
		manager->Destroy(this);
	}

	// Return the ref count  (which is in temp, since 'this' would be destroyed if the object his 0 ref counts)
	return tempRefCount;
}
