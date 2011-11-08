/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "AssetProvider.h"

namespace Gdk
{
	/// @addtogroup Resources & Assets
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Provides access to asset files managed & created by the GDK asset pipeline.
    ///	@remarks
    ///		Assets are basically any files that were created through the GDK asset pipeline
    ///     (or copied to the Assets build folder).  This static class provides methods to get
    ///     at those assets.
    ///   @par
    ///     Be default, all assets are stored in the /Assets/BUNDLE_NAME folder of the project.
    ///     The AssetManager only recognizes bundles that are registered to the asset manager.  This class
    ///     Also provides static method to Add, Remove, and Enumerate the registered asset bundles
    // =================================================================================
	class AssetManager
	{
	public:

		// Public Types
		// ================================
        
        
		// =================================================================================
        ///	@brief
        ///		Contains information about a registered asset provider
        // =================================================================================
        struct ProviderRegistration
		{
            /// The asset provider registered to the asset manager
			AssetProvider* Provider;
            
            /// Priority of this registered asset provider.  Higher priority providers are searched first when looking for an asset
			int Priority;

            // *****************************************************************
            /// @brief
            ///     Constructor
            // *****************************************************************
			ProviderRegistration(AssetProvider* provider, int priority) 
                : Provider(provider), Priority(priority)
            {
            }
            
            // *****************************************************************
            /// @brief
            ///     Less-than comparison operator
            // *****************************************************************
			inline bool operator< (const ProviderRegistration& input) const 
            { 
                // Higher priority = earlier sorting
                return this->Priority > input.Priority; 
            }  
		};
        
        // =================================================================================
        ///	@brief
        ///		A sorted vector of provider registrations.  Sorted by Priority
        // =================================================================================
		typedef SortedVector<ProviderRegistration>		ProviderRegistrationSet;
	
	
        // Public Methods
		// ================================

        // -----------------------------------
        /// @name Asset Methods
        /// @{
        
        static Stream* GetAssetStream(const char* assetPath);
        
        /// @}
        // -----------------------------------
        /// @name Registered Provider Methodss
        /// @{
        
        static void RegisterProvider(AssetProvider* provider, int priority = 0);
		static void UnRegisterProvider(AssetProvider* provider, bool deleteProvider);
		static const ProviderRegistrationSet& GetRegisteredProviders();
        
        /// @}

    private:

        // Private Methods
		// =====================================================
        
        friend class Application;
        
		static void Init();
		static void Shutdown();
        
        static ProviderRegistrationSet registeredProviders;
	};

    /// @}
    
} // namespace 
