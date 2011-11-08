/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Resource.h"

namespace Gdk
{
	/// @addtogroup Resources & Assets
    /// @{
    
    // =================================================================================
    ///	@brief
    ///	    A pool of managed resources.
    ///	@remarks
    ///		This utility class allows you to hold reference counts to a common pool of resources with 1 single object.  
    ///     The entire pool can be released by simply calling ResourcePool::Release()
    // =================================================================================
    class ResourcePool
	{
	public:

		// Public Types
		// ================================
		
        // =================================================================================
        ///	@brief
        ///	    Map of resources and the number of references stored in the pool
        // =================================================================================
		typedef map<Resource*, int>	ResourceMap;

		// Public Methods
		// ================================
		
        // -----------------------------------
        /// @name Common Methods 
        /// @{

        
		ResourcePool();
		virtual ~ResourcePool();

		virtual void Add(Resource* resource);
		virtual void Release();

		const ResourceMap& GetResourceMap() { return resourceMap; }

        /// @}
        
	private:
		
		// Private Properties
		// ================================
        
		ResourceMap resourceMap;
	};
    
    /// @}

} // namespace 
