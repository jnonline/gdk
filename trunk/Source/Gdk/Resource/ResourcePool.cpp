/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ResourcePool.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Constructor
// *****************************************************************
ResourcePool::ResourcePool()
{
}

// *****************************************************************
/// @brief
///     Destructor: Releases any resources currently in the pool
// *****************************************************************
ResourcePool::~ResourcePool()
{
	// Release all the resources in this pool
	Release();
}

// *****************************************************************
/// @brief
///     Adds a resource to this pool.  The pool assumes ownership of the resource
///     reference, and will thusly release the reference with the pool is released.
///   @par
///     Since the pool assumes ownership, the caller should not call Release() on the resource.
// *****************************************************************
void ResourcePool::Add(Resource* resource)
{
	// Find the resource if it already exists in the map
	ResourceMap::iterator iter = resourceMap.find(resource);
	if(iter != resourceMap.end())
	{
		// Resource already exists, add a reference
		iter->second++;
		return;
	}

	// Add the resource to the map (with a ref-count of 1)
	resourceMap[resource] = 1;
}

// *****************************************************************
/// @brief
///     Releases all the resource references in the pool
// *****************************************************************
void ResourcePool::Release()
{
	// Loop through all the resources in the map
	for(ResourceMap::iterator iter = resourceMap.begin(); iter != resourceMap.end(); iter++)
	{
		// Release this resource for as many times as we have a ref count
		while(iter->second > 0)
		{
            Resource* resource = iter->first;
            resource->Release();
			iter->second--;
		}
	}
	
	// Clear the map
	resourceMap.clear();
}

