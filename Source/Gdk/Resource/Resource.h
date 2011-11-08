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
    ///	    An enumeration of states that a resource can be in.
    // =================================================================================
    namespace ResourceState
	{
		enum Enum
		{
            /// The resource is currently being created or loaded.
			Loading,
            
            /// The resource is ready for use.
			Ready,
            
            /// The resource failed to load.
			LoadFailed,
		};
	}

	// =================================================================================
    ///	@brief
    ///	    Base class for all managed resources.  
    ///	@remarks
    ///		GDK resources are objects containing game data which are created or loaded and are shared 
    ///     across various engine or gameplay systems.  These resources are managed by a corresponding 
    ///     ResourceManager derived class.
    ///   @par
    ///     Resources are reference counted in order to provide the functionality for them to be shared. 
    ///     (Like textures that are used by multiple meshes)  You can get a reference to a resource either
    ///     from the ResourceManager or by calling AddRef() on an existing resource, both of which increase
    ///     the reference count by 1.  When you are done with a resource, remember to call Release()
    // =================================================================================
	class Resource
	{
        
	public:

        // Public Properties
		// ================================
        
        // -----------------------------------
        /// @name Properties 
        /// @{

        /// Current state of the resource.   [read-only]
        ResourceState::Enum State;
        
        /// @}
        
		// Public Methods
		// ================================
        
        // -----------------------------------
        /// @name Common Methods 
        /// @{
        
        virtual ~Resource();
        
		const string& GetName();
		bool IsReady();

		UInt32 GetHashCode();
        
		int GetReferenceCount();
        int AddRef();
		int Release();
        
        /// @}
        
    protected:
		
		// Public Methods
		// ================================
        
        // -----------------------------------
        /// @name Common Methods 
        /// @{
        
		Resource();
                
        virtual size_t GetMemoryUsed();
        
        /// @}
        
    private:
    
        // Privates
        // ------------------------
        
		string					name;
		int						referenceCount;
        class ResourceManager*  manager;
        
        friend class ResourceManager;
	};
    
    /// @}
	
} // namespace 
