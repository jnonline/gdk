#include "BasePCH.h"
#include "FileAssetProvider.h"

using namespace Gdk;

// ***********************************************************************
FileAssetProvider::FileAssetProvider(const char* baseFolderPath)
{
	SetRootPath(baseFolderPath);

	// Reserve space for 5 bundles
	this->bundles.reserve(5);
}

// ***********************************************************************
void FileAssetProvider::SetRootPath(const char* baseFolderPath)
{
	// Save the root path
	this->rootPath = baseFolderPath;

	// Update the name
	this->name = string("File Assets: ");
	this->name.append(this->rootPath);
}

// ***********************************************************************
const string& FileAssetProvider::GetRootPath()
{
	return this->rootPath;
}

// ***********************************************************************
void FileAssetProvider::AddBundle(const char* bundleName, int priority)
{
	// Push the new bundle onto the priority queue
	this->bundles.AddSorted(Bundle(bundleName, priority));
}

// ***********************************************************************
void FileAssetProvider::RemoveBundle(const char* bundleName)
{
	// Loop through the bundles
	for(BundleSet::iterator iter = this->bundles.begin(); iter != this->bundles.end(); iter++)
	{
		// Is this the bundle?
		if(strcmp(iter->Name.c_str(), bundleName) == 0)
		{
			// Remove the bundle
			this->bundles.erase(iter);
			return; 
		}
	}
}

// ***********************************************************************
const string& FileAssetProvider::GetName()
{
	return this->name;
}

// ***********************************************************************
Stream* FileAssetProvider::GetStream(const char* assetName, AssetType::Enum assetType)
{
	// Loop through the bundles  (which are in priority order)
	for(BundleSet::iterator bundleIter = this->bundles.begin(); bundleIter != this->bundles.end(); bundleIter++)
	{
		Bundle& bundle = *bundleIter;

		// Build the full path to the asset
		string bundlePath = Path::Combine(this->rootPath.c_str(), bundle.Name.c_str());
		string fullAssetPath = Path::Combine(bundlePath.c_str(), assetName);

		// Append the asset type extension
		switch(assetType)
		{
			case AssetType::Texture1D:
			case AssetType::Texture2D:
			case AssetType::Texture3D:
			case AssetType::TextureCube:
				fullAssetPath.append(".gdkimage");
				break;

			case AssetType::Atlas:
				fullAssetPath.append(".gdkatlas");
				break;

			case AssetType::BMFont:
				fullAssetPath.append(".gdkfont");
				break;

			case AssetType::Shader:
				fullAssetPath.append(".gdkshader");
				break;

			case AssetType::Material:
				fullAssetPath.append(".gdkmaterial");
				break;

			case AssetType::Model:
				fullAssetPath.append(".gdkmodel");
				break;
		}

		// Does this asset file exist?
		if(File::Exists(fullAssetPath.c_str()) == true)
		{
			// Found the asset, so lets open a stream to it & return
			Stream* stream = GdkNew FileStream(fullAssetPath.c_str(), Gdk::FileMode::Read);
			return stream;
		}
	}

	// We tried every path/bundle combo, the asset does not exist (in this provider)
	return NULL;
}