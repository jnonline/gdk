/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ResourceManager.h"

using namespace Gdk;

// Static instantiations
ResourceManager::BackgroundWorkQueue* ResourceManager::BGWorkQueue = NULL;

// *****************************************************************
/// @brief
///     Static Initializer for the ResourceManager sub-system
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void ResourceManager::Init(int numBackgroundThreads)
{
    // Create the background work queue
    BGWorkQueue = GdkNew BackgroundWorkQueue(numBackgroundThreads);
    
    // Create the singleton resource managers
    Texture2DManager::singleton = GdkNew Texture2DManager();
    ShaderManager::singleton = GdkNew ShaderManager();
    AtlasManager::singleton = GdkNew AtlasManager();
    BMFontManager::singleton = GdkNew BMFontManager();
    ModelManager::singleton = GdkNew ModelManager();
}

// *****************************************************************
/// @brief
///     Static Shutdown for the ResourceManager sub-system
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void ResourceManager::Shutdown()
{
    // Destroy the background work queue
    GdkDelete( BGWorkQueue );
    
    // Destroy the singleton resource managers
    GdkDelete( ModelManager::singleton );
    GdkDelete( BMFontManager::singleton );
    GdkDelete( AtlasManager::singleton );
    GdkDelete( ShaderManager::singleton );
    GdkDelete( Texture2DManager::singleton );
}

// *****************************************************************
/// @brief
///     Default Constructor
// *****************************************************************
ResourceManager::ResourceManager()
{
    // Create the thread sync objects
    resourceMapMutex = Mutex::Create();
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
ResourceManager::~ResourceManager()
{    
    // Loop through the resources
    for(ResourcesByNameMap::Iterator resourceIter = resourcesByName.Begin(); resourceIter != resourcesByName.End(); resourceIter++)
    {
        Resource* resource = resourceIter->second;
        
        // delete the resource
        GdkDelete( resource );
    }
    
    // Destroy the thread sync objects
    GdkDelete( resourceMapMutex );
}

// *****************************************************************
/// @brief
///     Gets the number of resources managed by this Resource Manager
// *****************************************************************
size_t ResourceManager::GetResourceCount()
{
    // lock the resource map mutex
    resourceMapMutex->Lock();
    
    // Get the number of resources
    size_t count = resourcesByName.Size();
    
    // Unlock the resource map mutex
    resourceMapMutex->Unlock();

    return count;
}

// *****************************************************************
/// @brief
///     Gets the total number of bytes used by this manager's resources
/// @remarks
///     This method only returns valid data for Resource classes
///     that implement the GetMemoryUsed() virtual method
// *****************************************************************
size_t ResourceManager::GetTotalMemoryUsed()
{
    // lock the resource map mutex
    resourceMapMutex->Lock();
    
    size_t totalMemoryUsed;
    
    // Loop through the resources
    for(ResourcesByNameMap::Iterator resourceIter = resourcesByName.Begin(); resourceIter != resourcesByName.End(); resourceIter++)
    {
        totalMemoryUsed += resourceIter->second->GetMemoryUsed();
    }
    
    // Unlock the resource map mutex
    resourceMapMutex->Unlock();
    
    return totalMemoryUsed;
}

// *****************************************************************
/// @brief
///     Creates a new managed resource with the given name.
/// @param name
///     Unique name for the new resource
/// @remarks
///     This thread-safe method creates a new managed resource with
///     the given name.  If the name is already in use, the method asserts
// *****************************************************************
Resource* ResourceManager::CreateResource(const char* name)
{
    // lock the resource map mutex
    resourceMapMutex->Lock();
    
    // Make sure no resource with the given name exists
    ASSERT(resourcesByName.ContainsKey(name) == false, "A resource with the given name already exists");
    
    // Create a new resource intance
    Resource* resource = OnCreateNewResourceInstance();
    
    // Setup the new resource
    resource->manager = this;
    resource->name = name;
    resource->State = ResourceState::Loading;
    resource->referenceCount = 1;
    
    // Add the resource to the map
    resourcesByName.Add(name, resource);
    
    // unlock the resource map mutex
    resourceMapMutex->Unlock();
    
    return resource;
}

// *****************************************************************
/// @brief
///     Gets a resource with the given name, if the resource does not
///     exist, a new one is created.
/// @param name
///     Unique name of the resource
/// @param[out] alreadyExists
///     This out parameter returns true if the resource already existed. (false if a new resource was created)
/// @remarks
///     This thread-safe method gets an existing resource or creates a new managed resource with
///     the given name.
// *****************************************************************
Resource* ResourceManager::GetOrCreateResource(const char* name, bool& alreadyExists)
{
    // lock the resource map mutex
    resourceMapMutex->Lock();
    
    // Lookup the resource by it's name
    Resource* resource = NULL;
    ResourcesByNameMap::Iterator resourceIter = resourcesByName.Find(name);
    if(resourceIter != resourcesByName.End())
    {
        // The resource exists
        alreadyExists = true;
        resource = resourceIter->second;
        
        // Add a reference to the resource
        resource->AddRef();
        
        // Unlock the mutex
        resourceMapMutex->Unlock();

        // Return the resource
        return resource;
    }
    
    // Create a new resource intance
    alreadyExists = false;
    resource = OnCreateNewResourceInstance();
    
    // Setup the new resource
    resource->manager = this;
    resource->name = name;
    resource->State = ResourceState::Loading;
    resource->referenceCount = 1;
    
    // Add the resource to the map
    resourcesByName.Add(name, resource);

    // unlock the resource map mutex
    resourceMapMutex->Unlock();
    
    return resource;
}

// *****************************************************************
/// @brief
///     Removes a resource from the manager's internal maps
/// @param resource
///     Resource to be removed.
/// @remarks
///     This method is expected to ONLY be called by Resource::Release().
///     GDK Internal Use Only
// *****************************************************************
void ResourceManager::RemoveResource(Resource* resource)
{
    // Lock the resource map 
    resourceMapMutex->Lock();
    
    // Find the resource in the name map
    ResourcesByNameMap::Iterator resourceIter = resourcesByName.Find(resource->name.c_str());
    if(resourceIter != resourcesByName.End())
    {
        // Remove the resource from the map
        resourcesByName.Remove(resourceIter);
    }
    
    // Unlock the resource map
    resourceMapMutex->Unlock();
}

// *****************************************************************
/// @brief
///     Utility method for doing sync/async loading of a resource.
/// @param name
///     Name of the resource.
/// @param async
///     If true, the resource will be loaded by a background thread.  (Default = false)
/// @param asyncPriority
///     The priority for an asyncronous load.  Higher priority items are processed first.
/// @param loadFunction
///     A worker method that will do the actual loading of the resource.
// *****************************************************************
Resource* ResourceManager::LoadUtility(const char *name, bool async, int asyncPriority, void (*loadFunction)(Resource*))
{
    // Get the existing resource (or create a new one)
    bool alreadyExists = false;
    Resource* resource = ResourceManager::GetOrCreateResource(name, alreadyExists);
    
    // Did the resource already exist?
    if(alreadyExists)
    {
        // Did the caller request a syncronous load & is the resource currently loading?
        if(resource->State == ResourceState::Loading)
        {
            // TODO(P2): Use proper eventing to signal the completion   [Note: this only happens when Load() is called on a resource already queued & being processed by a LoadAsync() call]
            LOG_WARN("LoadUtility() was called to sync load a resource already queued for async loading.  RESOURCE: \"%s\"" , name);
            
            // Wait for the resource to finish loading
            while(resource->State != ResourceState::Ready)
            {
                Thread::Sleep(1);
            }
        }
        
        // Return the existing resource 
        return resource;
    }
    
    // Does the caller want to load asyncronously?
    if(async)
    {
        // Queue up an async load
        QueueBackgroundTask(resource, asyncPriority, loadFunction);
    }
    else
    {
        // Do the load now
        (*loadFunction)( resource );
    }
    
    return resource;
}

// *****************************************************************
/// @brief
///     Queues up a background task using the resource manager's background work queue
/// @param resource
///     A resource to be passed to the work method
/// @param asyncPriority
///     Priority level of the queue'd work.  Higher priority items are processed first
/// @param loadFunction
///     A worker method that will do the actual loading of the resource.
// *****************************************************************
void ResourceManager::QueueBackgroundTask(Resource* resource, int asyncPriority, void (*loadFunction)(Resource*))
{
    BGWorkQueue->Enqueue(BackgroundWorkItem(resource, loadFunction), asyncPriority);
}
                         
                         
