/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Asset.h"

namespace Gdk
{
	// ============================================================================

	class AssetPool
	{
	public:

		// Public Types
		// -------------------------------

		typedef map<AssetBase*, int>	AssetMap;

		// Public Methods
		// -------------------------------
		
		// CTor/DTor
		AssetPool();
		virtual ~AssetPool();

		// Add an asset to this pool (the pool assumes ownership of this asset reference, the caller should NOT call Release() on the asset)
		virtual void Add(AssetBase* asset);

		// Releases all the assets managed by this pool
		virtual void Release();

		// Gets the map of assets managed by this pool
		const AssetMap& GetAssetMap() { return assetMap; }

	protected:
		
		// Internal Properties
		// -------------------------------

		AssetMap assetMap;
	};

} // namespace 
