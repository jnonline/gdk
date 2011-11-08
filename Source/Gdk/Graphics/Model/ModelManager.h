/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

namespace Gdk
{
	/// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Manages Model Resources
    // =================================================================================
    class ModelManager : public ResourceManager
	{
    public:
        // Publics
		// =====================================================
        
        virtual ~ModelManager();
        
        // ---------------------------------
        /// @name Creation methods
        /// @{
        
        static Model* FromAsset(const char *name, bool async = false, int asyncPriority = 1);
        
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
        
        // Creation Worker Methods
        static void PerformLoadFromAsset(Resource* resource);
        
        // CTor/DTor
        ModelManager();
        
        // Singleton
        static ModelManager* singleton;
        
        friend class ResourceManager;
        
	};
    
    /// @}

} // namespace 

