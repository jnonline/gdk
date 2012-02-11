
//----------------------------------------------------------------------------
#ifdef GDK_MEMORY_TRACKING
//----------------------------------------------------------------------------

template <typename T>
T* Memory::New1DArray (const int bound0)
{
    T* data;

    // Insert T[] into memory map.
    data = (T*)CreateBlock(bound0*sizeof(T), 1);

    // Call the default constructors for T.
    T* object = data;
    for (int i = 0; i < bound0; ++i, ++object)
    {
        ::new(object) T;
    }
    
    return data;
}

// ********************************************************************************
template <typename T>
T** Memory::New2DArray (const int bound0, const int bound1)
{
    const int bound01 = bound0*bound1;
    T** data;

    // No constructor for T*, insert T*[] into memory map.
    data = (T**)CreateBlock(bound1*sizeof(T*), 2);

    // Insert T[] into memory map.
    data[0] = (T*)CreateBlock(bound01*sizeof(T), 1);

    // Call default constructors for T.  If T is a pointer type, the
    // compiler will not generate any code for the constructor call.
    T* object = data[0];
    for (int i = 0; i < bound01; ++i, ++object)
    {
        ::new(object) T;
    }

    // Hook up the pointers to form the 2D array.
    for (int i1 = 1; i1 < bound1; ++i1)
    {
        int j0 = bound0*i1;  // = bound0*(i1 + j1) where j1 = 0
        data[i1] = &data[0][j0];
    }

    return data;
}

// ********************************************************************************
template <typename T>
void Memory::Delete (T*& data)
{
	if(data == NULL)
		return;

    MemoryMap::iterator iter = memoryMap->find(data);
    if (iter != memoryMap->end())
    {
        if (iter->second.numDimensions == 0)
        {
            // Call destructor for T.  If T is a pointer type, the
            // compiler will not generate any code for the destructor
            // call.
            data->~T();

            // Remove T from memory map.
            memoryMap->erase(data);
            deallocatorFunc(data, this->file, this->line);
        }
        else
        {
            ASSERT(false, "Mismatch in dimensions.\n");
        }
    }
    else
    {
        ASSERT(false, "Memory block not in map.\n");
    }

	data = NULL;
}

// ********************************************************************************
template <typename T>
void Memory::Delete1DArray (T*& data)
{
    if (data)
    {
        MemoryMap::iterator iter = memoryMap->find(data);
        if (iter != memoryMap->end())
        {
            if (iter->second.numDimensions == 1)
            {
                // Call destructor for T.  If T is a pointer type, the
                // compiler will not generate any code for the destructor
                // call.
                const int numElements = iter->second.numBytes/sizeof(T);
                T* object = data;
                for (int i = 0; i < numElements; ++i, ++object)
                {
                    object->~T();
                }

                // Remove T[] from memory map.
                memoryMap->erase(data);
                deallocatorFunc(data, this->file, this->line);
            }
            else
            {
                ASSERT(false, "Mismatch in dimensions.\n");
            }
        }
        else
        {
            ASSERT(false, "Memory block not in map.\n");
        }

        data = NULL;
    }
}

// ********************************************************************************
template <typename T>
void Memory::Delete2DArray (T**& data)
{
    if (data)
    {
        MemoryMap::iterator iter = memoryMap->find(data);
        if (iter != memoryMap->end())
        {
            if (iter->second.numDimensions == 2)
            {
                // Call destructor for T.  If T is a pointer type, the
                // compiler will not generate any code for the destructor
                // call.
                const int numElements = iter->second.numBytes/sizeof(T);
                T* object = data[0];
                for (int i = 0; i < numElements; ++i, ++object)
                {
                    object->~T();
                }

                // Remove T[] from memory map.
                memoryMap->erase(data[0]);
                deallocatorFunc(data[0], this->file, this->line);

                // No destructor for T*, remove T*[] from memory map.
                memoryMap->erase(data);
                deallocatorFunc(data, this->file, this->line);
            }
            else
            {
                ASSERT(false, "Mismatch in dimensions.\n");
            }
        }
        else
        {
            ASSERT(false, "Memory block not in map.\n");
        }

        data = NULL;
    }
}

//----------------------------------------------------------------------------
#else // GDK_MEMORY_TRACKING
//----------------------------------------------------------------------------

template <typename T>
T* GdkNew1DArray (const int bound0)
{
    return new T[bound0];
}

// ********************************************************************************
template <typename T>
T** GdkNew2DArray (const int bound0, const int bound1)
{
    const int bound01 = bound0*bound1;
    T** data = new T*[bound1];
    data[0] = new T[bound01];

    for (int i1 = 1; i1 < bound1; ++i1)
    {
        int j0 = bound0*i1;  // = bound0*(i1 + j1) where j1 = 0
        data[i1] = &data[0][j0];
    }
    return data;
}

// ********************************************************************************
template <typename T>
void GdkDelete (T*& data)
{
    delete data;
    data = 0;
}

// ********************************************************************************
template <typename T>
void GdkDelete1DArray (T*& data)
{
    delete[] data;
    data = 0;
}

// ********************************************************************************
template <typename T>
void GdkDelete2DArray (T**& data)
{
    if (data)
    {
        delete[] data[0];
        delete[] data;
        data = 0;
    }
}

//----------------------------------------------------------------------------
#endif // GDK_MEMORY_TRACKING
//----------------------------------------------------------------------------
