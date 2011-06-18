#include "BasePCH.h"
#include "AssetPool.h"

using namespace Gdk;

// ***********************************************************************
AssetPool::AssetPool()
{
}

// ***********************************************************************
AssetPool::~AssetPool()
{
	// Release all the assets in this pool
	Release();
}

// ***********************************************************************
void AssetPool::Add(AssetBase* asset)
{
	// Find the asset if it already exists in the map
	AssetMap::iterator iter = assetMap.find(asset);
	if(iter != assetMap.end())
	{
		// Asset already exists, add a reference
		iter->second++;
		return;
	}

	// Add the asset to the map (with a ref-count of 1)
	assetMap[asset] = 1;
}

// ***********************************************************************
void AssetPool::Release()
{
	// Loop through all the assets in the map
	for(AssetMap::iterator iter = assetMap.begin(); iter != assetMap.end(); iter++)
	{
		// Release this asset for as many times as we have a ref count
		while(iter->second > 0)
		{
			iter->first->Release();
			iter->second--;
		}
	}
	
	// Clear the map
	assetMap.clear();
}

