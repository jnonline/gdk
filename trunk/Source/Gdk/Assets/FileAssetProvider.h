/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "AssetProvider.h"
#include "../System/Containers/SortedVector.h"

namespace Gdk
{
	// ============================================================================

	class FileAssetProvider : public AssetProvider
	{
	public:
		// Types
		// ---------------------
		
		// Bundle
		struct Bundle
		{
			string Name;
			int Priority;

			// CTor
			Bundle(const char* name, int priority) : Name(name), Priority(priority) {};
			inline bool operator< (const Bundle& input) const { return this->Priority > input.Priority; }  // Higher priority = earlier sorting
		};

		// BundleQueue
		typedef SortedVector<Bundle>		BundleSet;

	protected:

		// Internals
		// ---------------------
		
		string		rootPath;
		string		name;
		BundleSet	bundles;
		
	public:
		// Publics
		// ---------------------

		// CTor
		FileAssetProvider(const char* baseFolderPath); 

		// Path management
		void SetRootPath(const char* baseFolderPath);
		const string& GetRootPath();

		// Bundles
		void AddBundle(const char* bundleName, int priority);
		void RemoveBundle(const char* bundleName);
		const BundleSet& GetBundles() { return this->bundles; }
		
		// AssetProvider - Interfaces
		virtual const string& GetName();
		virtual Stream* GetStream(const char* assetName, AssetType::Enum assetType);
	};

} // namespace 
