/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Resource.h"

using namespace Gdk;


// *****************************************************************
/// @brief
///     Constructor
// *****************************************************************
Resource::Resource()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
Resource::~Resource()
{
}

// *****************************************************************
/// @brief
///     Gets the name of this resource
// *****************************************************************
const string& Resource::GetName()
{ 
    return this->name; 
}

// *****************************************************************
/// @brief
///     Checks if this resource is in the Ready state.  Meaning it has finished loading
// *****************************************************************
bool Resource::IsReady()
{ 
    return this->State == ResourceState::Ready; 
}

// *****************************************************************
/// @brief
///     Gets a unique hash code of the name of this resource.
// *****************************************************************
UInt32 Resource::GetHashCode()
{ 
    return StringUtilities::FastHash(this->name.c_str()); 
}

// *****************************************************************
/// @brief
///     Gets the current reference count on the resource
// *****************************************************************
int Resource::GetReferenceCount()
{ 
    return this->referenceCount; 
}

// *****************************************************************
/// @brief
///     Adds a reference to the resource.
// *****************************************************************
int Resource::AddRef()
{ 
    return this->referenceCount++; 
}

// *****************************************************************
/// @brief
///     Releases a reference to the resource.  If the reference count becomes 0, the resource will be destroyed
// *****************************************************************
int Resource::Release()
{
    // Decrement the ref count
    this->referenceCount--;
    int tempRefCount = this->referenceCount;
    
    // Did the ref count hit 0?
    if(this->referenceCount <= 0)
    {
        // Tell the resource manager it no longer manages this resource
        if(manager != NULL)
        {
            manager->RemoveResource(this);
            manager = NULL;
        }
        
        // Delete ourself
        Resource* me = this;
        GdkDelete( me );
    }
    
    // Return the ref count  (which is in temp, since 'this' would be destroyed if the object his 0 ref counts)
    return tempRefCount;
}

// *****************************************************************
/// @brief
///     Gets the amount of memory used by this resource.
/// @remarks
///     This virtual method should be overriden in derived classes, and should retur
///     the amount of memory used by that resource.
// *****************************************************************
size_t Resource::GetMemoryUsed()
{
    return 0; 
}
