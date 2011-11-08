/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Resource.h"
#include "../System/Containers/StringHashMap.h"

namespace Gdk
{
	/// @addtogroup Resources & Assets
    /// @{
    
    // =================================================================================
    ///	@brief
    ///	    Base class for all resource managers. 
    ///	@remarks
    ///		GDK resources are intended to be loaded and shared by multiple game systems.  Such as 
    ///     fonts that are used by various UI systems or textures that are used by different meshes.  
    ///     ResourceManager derived classes are responsible for managing different types of resources.
    ///   @par
    ///     Resource managers provide functionality to Create in memory resources, Load resources from 
    ///     asset files (syncronously & asyncronously), and Sharing of those resources
    // =================================================================================
	class ResourceManager
	{
	public:

        // Public Types
		// ================================
		
        // =================================================================================
        ///	@brief
        ///	    Map of resources, hashed by the resource name
        // =================================================================================
		typedef StringHashMap<Resource*> ResourcesByNameMap;
     
        
        // Public Methods
		// ================================
        
        // -----------------------------------
        /// @name Resource Information Methods 
        /// @{

        size_t GetResourceCount();
        size_t GetTotalMemoryUsed();
        
        /// @}
                  
    protected:
        
        // Protected Methods
        // ================================
        
        // -----------------------------------
        /// @name Common Methods 
        /// @{
        
        ResourceManager();
        virtual ~ResourceManager();
        
        // Utility methods for derived managers
        Resource* CreateResource(const char* name);
        Resource* GetOrCreateResource(const char* name, bool& alreadyExists);
        
        Resource* LoadUtility(const char *name, bool async, int asyncPriority, void (*loadFunction)(Resource*));
        void QueueBackgroundTask(Resource* resource, int asyncPriority, void (*loadFunction)(Resource*));
        
        // Derived managers must implement this, and it must return a new RESOURCETYPE* 
        virtual Resource* OnCreateNewResourceInstance() = 0;
        
    private:
        
        // Private Types
        // ================================
        
        // ***********************************************************************
        struct BackgroundWorkItem
        {
        public:
            void (*WorkerFunction)(Resource*);
            Resource* Res;
            
            BackgroundWorkItem() {}
            BackgroundWorkItem(Resource* resource, void (*workerFunction)(Resource*)) 
            { 
                Res = resource;
                WorkerFunction = workerFunction; 
            }
        };
        
        
        // ***********************************************************************
        class BackgroundWorkQueue : public ThreadedWorkQueue<BackgroundWorkItem>
        {
        public:
            BackgroundWorkQueue(int numThreads) : ThreadedWorkQueue<BackgroundWorkItem>(numThreads)  {}
            
        protected:
            virtual void OnProcessWorkItem(BackgroundWorkItem item)
            {
                (*(item.WorkerFunction))(item.Res);
            }
        };

        
        // Private Properties
		// ================================
        
		// Thread sync
        Mutex* resourceMapMutex;
        
        // Resources (by name hash)
        ResourcesByNameMap resourcesByName;
        
        // Static background work queue
        static BackgroundWorkQueue* BGWorkQueue;
        
        // Private Methods
		// ================================
        
        // Removes the resource from this resource manager
        void RemoveResource(Resource* resource);
        
		// Application Interface        
		static void Init(int numBackgroundThreads);
		static void Shutdown();
    
        // Friends
        friend class Resource;
        friend class Application;
	};	
    
    /// @}
    
} // namespace 
