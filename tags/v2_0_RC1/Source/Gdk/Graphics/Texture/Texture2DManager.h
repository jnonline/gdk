/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Texture2D.h"
#include "../../Resource/ResourceManager.h"

namespace Gdk
{
	/// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Manages Texture2D Resources
    // =================================================================================
    class Texture2DManager : public ResourceManager
	{
    public:
        // Publics
		// =====================================================
        
        virtual ~Texture2DManager();
        
        // ---------------------------------
        /// @name Creation methods
        /// @{
        
        static Texture2D* Create(const char *name, int width, int height, PixelFormat::Enum pixelFormat);
        static Texture2D* FromAsset(const char *name, bool async = false, int asyncPriority = 1);
        
        /// @}

    protected:
        // Protecteds
		// =====================================================

        // ---------------------------------
        /// @name Resource overrides
        /// @{
        
        virtual Resource* OnCreateNewResourceInstance();
        
        /// @}
        
    private:
        // Privates
		// =====================================================
        
        // CTor/DTor
        Texture2DManager();
        
        // Creation Worker Methods
        static void PerformLoadFromAsset(Resource* resource);
        
        // Singleton
        static Texture2DManager* singleton;
        
        friend class ResourceManager;
	};
    
    /// @}

} // namespace 

