/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "AssetProvider.h"
#include "../System/Containers/SortedVector.h"

namespace Gdk
{
	/// @addtogroup Resources & Assets
    /// @{
    
    // =================================================================================
    ///	@brief
    ///	    Provides access to assets in a folder on the device file system.
    ///	@remarks
    ///		Typically this provider has its RootFolder set to a bundle folder within the
    ///     main /Assets folder of the game.
    // =================================================================================
    class FileAssetProvider : public AssetProvider
	{
	public:
		// Public Methods
		// ================================
        
        // -----------------------------------
        /// @name Common Methods 
        /// @{
        
		FileAssetProvider(const char* rootPath); 

        void SetRootPath(const char* rootPath);
		const string& GetRootPath();
		
		virtual const string& GetName();
        virtual bool HasAsset(const char* assetPath);
		virtual Stream* GetAssetStream(const char* assetPath);
        
        /// @}
        
    private:
        
		// Internals
		// ---------------------
		
		string		rootPath;
		string		name;
	};
    
    
    /// @}

} // namespace 
