/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

namespace Gdk
{
	/// @addtogroup Resources & Assets
    /// @{
    
    // =================================================================================
    ///	@brief
    ///	    Abstract interface class for Asset Providers.
    ///	@remarks
    ///		Asset providers are used to expose a set of assets (files) in relative paths to GDK and Game code.
    ///     The providers are expected to provider an interface to get an asset file from a given relative path.
    // =================================================================================
	class AssetProvider
	{
	public:

        // Public Methods
		// ================================
        
        // -----------------------------------
        /// @name Abstract Interface Methods
        /// @{

        // *****************************************************************
        /// @brief
        ///     Gets a display friendly name of this asset provider.
        /// @remarks
        ///     Examples:  
        ///     @li "File System: ./Assets/Base"
        ///     @li "ZIP:  /Assets/HiRez.zip"
        // *****************************************************************
		virtual const string& GetName() = 0;

        // *****************************************************************
        /// @brief
        ///     Checks if this provider has an asset at the given path
        /// @param assetPath 
        ///     Path to the asset.  This path is assumed to be relative to the AssetProvider root.
        // *****************************************************************
		virtual bool HasAsset(const char* assetPath) = 0;
        
        // *****************************************************************
        /// @brief
        ///     Gets a Read-Only Stream* to the asset.
        /// @param assetPath 
        ///     Path to the asset.  This path is assumed to be relative to the AssetProvider root.
        /// @remarks
        ///     If the asset doesnt exist, the method returns NULL.
        // *****************************************************************
		virtual Stream* GetAssetStream(const char* assetPath) = 0;
        
        /// @}
	};
    
    /// @}

} // namespace 
