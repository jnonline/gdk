/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	// ============================================================================
	// This custom map is used for storing items using a 32-bit string hash code for each object.
	//

	template<typename TValue>
	class StringHashMap
	{
	public:
		// Public Types
		// ===============================

		typedef TValue                      ValueType;
		typedef map<UInt32, TValue>         Map;
		typedef typename Map::iterator		Iterator;

		// The actual Map
		// ===============================

		Map Items;

		// Public Methods
		// ===============================

		// Adds an item to the map by its hash
		void Add(const char* key, TValue value)
		{
			UInt32 hash = StringUtilities::FastHash(key);
			Items[hash] = value;
		}

		// Removes an item from the map 
		void Remove(const char* key)
		{
			UInt32 hash = StringUtilities::FastHash(key);
			Items.erase(hash);
		}

		// Removes an item from the map by it's iterator
		void Remove(Iterator iter)
		{
            Items.erase(iter);
        }

		// Finds an item in the map
		Iterator Find(const char* key)
		{
			UInt32 hash = StringUtilities::FastHash(key);
            Iterator result = Items.find(hash);
			return result;
        }	

		// Gets the beginning iterator
		Iterator Begin()
		{
			return Items.begin();
		}

		// Gets the end iterator
		Iterator End()
		{
			return Items.end();
		}

		// Clears the hash map
		void Clear()
		{
			Items.clear();
		}
	};

} // namespace Gdk
