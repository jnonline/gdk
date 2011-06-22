/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "AssetType.h"

namespace Gdk
{
	// ============================================================================

	class AssetProvider
	{
	public:

		// Virtual interfaces for derived provider classes
		// -----------------------------

		// Returns the Display-friendly-name of the provider (IE: "File System: ./Assets")
		virtual const string& GetName() = 0;

		// Gets a stream for the given asset.  NULL will be returned if the asset doesnt exist
		virtual Stream* GetStream(const char* assetName, AssetType::Enum assetType) = 0;
	};

} // namespace 
