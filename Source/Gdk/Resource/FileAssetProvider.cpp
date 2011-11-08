/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "FileAssetProvider.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Constructor
/// @param rootPath
///     Path to a folder on the file-system where provided assets will be relative to.
// *****************************************************************
FileAssetProvider::FileAssetProvider(const char* rootPath)
{
	SetRootPath(rootPath);
}

// *****************************************************************
/// @brief
///     Changes the Root Folder of the provider to the given string
/// @param rootPath
///     Path to a folder on the file-system where provided assets will be relative to.
// *****************************************************************
void FileAssetProvider::SetRootPath(const char* rootPath)
{
	// Save the root path
	this->rootPath = rootPath;

	// Update the name
	this->name = string("File Assets: ");
	this->name.append(this->rootPath);
}

// *****************************************************************
/// @brief
///     Gets the root path of this provider, where assets are provided from.
// *****************************************************************
const string& FileAssetProvider::GetRootPath()
{
	return this->rootPath;
}

// *****************************************************************
/// @brief
///     Gets a display friend name for this provider.  Ex: "File System: /Assets/Base"
// *****************************************************************
const string& FileAssetProvider::GetName()
{
	return this->name;
}

// *****************************************************************
/// @brief
///     Checks if this provider has an asset at the given path
/// @param assetPath 
///     Path to the asset.  This path is assumed to be relative to the root folder.
// *****************************************************************
bool FileAssetProvider::HasAsset(const char* assetPath)
{
    // Build the path to the asset
    string fullAssetPath = Path::Combine(rootPath.c_str(), assetPath);
    
    // Does this asset file exist?
    return File::Exists(fullAssetPath.c_str());
}
    
// *****************************************************************
/// @brief
///     Gets a Read-Only Stream* to the asset.
/// @param assetPath 
///     Path to the asset.  This path is assumed to be relative to the root folder.
/// @remarks
///     If the asset doesnt exist, the method returns NULL.
// *****************************************************************
Stream* FileAssetProvider::GetAssetStream(const char* assetPath)
{
    // Build the path to the asset
    string fullAssetPath = Path::Combine(rootPath.c_str(), assetPath);
    
    // Does this asset file exist?
    if(File::Exists(fullAssetPath.c_str()) == true)
    {
        // Found the asset, so lets open a stream to it & return
        Stream* stream = GdkNew FileStream(fullAssetPath.c_str(), Gdk::FileMode::Read);
        return stream;
    }
    
    return NULL;
}