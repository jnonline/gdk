/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup System
    /// @{
    /// @addtogroup Containers
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		A custom map container that uses string hashes for keys
    // =================================================================================
	template<typename TValue>
	class StringHashMap
	{
	public:
		// Public Types
		// =====================================================

        /// The type of the container values
		typedef TValue                      ValueType;
        
        /// The type of the underlying map
		typedef map<UInt32, TValue>         Map;
        
        /// The iterator type for the underlying map
		typedef typename Map::iterator		Iterator;

		// Public Properties
		// =====================================================
        
        /// The underlying map
		Map Items;

		// Public Methods
		// =====================================================

        // *****************************************************************
        /// @brief
        ///     Adds an object to the map
        /// @param key
        ///     Unique key string (which will be hashed)
        /// @param value
        ///     The object to add to the map
        // *****************************************************************
		void Add(const char* key, TValue value)
		{
			UInt32 hash = StringUtilities::FastHash(key);
			Items[hash] = value;
		}

		// *****************************************************************
        /// @brief
        ///     Removes an item from the map by its key
        /// @param key
        ///     Unique key string
        // *****************************************************************
		void Remove(const char* key)
		{
			UInt32 hash = StringUtilities::FastHash(key);
			Items.erase(hash);
		}

		// *****************************************************************
        /// @brief
        ///     Removes an item from the map by its iterator
        /// @param iter
        ///     Iterator of the object to be removed
        // *****************************************************************
		void Remove(Iterator iter)
		{
            Items.erase(iter);
        }

		// *****************************************************************
        /// @brief
        ///     Finds an object in the map that is assigned to a specific key
        /// @param key
        ///     Unique key string
        /// @return
        ///     An Iterator to the map entry or End() if the object wasnt found
        // *****************************************************************
		Iterator Find(const char* key)
		{
			UInt32 hash = StringUtilities::FastHash(key);
            Iterator result = Items.find(hash);
			return result;
        }	
        
        // *****************************************************************
        /// @brief
        ///     Checks if an object with the given key exists in the map
        /// @param key
        ///     Unique key string
        /// @return
        ///     true if an item with the given key is in the map.
        // *****************************************************************
		bool ContainsKey(const char* key)
		{
			UInt32 hash = StringUtilities::FastHash(key);
            return Items.find(hash) != Items.end();
        }	

		// *****************************************************************
        /// @brief
        ///     Gets an iterator at the beginning of the map
        // *****************************************************************
		Iterator Begin()
		{
			return Items.begin();
		}

		// *****************************************************************
        /// @brief
        ///     Gets an iterator to the end of the map
        // *****************************************************************
		Iterator End()
		{
			return Items.end();
		}

        // *****************************************************************
        /// @brief
        ///     Clears all the items from the map
        // *****************************************************************
		void Clear()
		{
			Items.clear();
		}
        
        // *****************************************************************
        /// @brief
        ///     Gets the number of items in the map
        // *****************************************************************
		int Size()
		{
			return Items.size();
		}
	};
    
    /// @}
    /// @}

} // namespace Gdk
