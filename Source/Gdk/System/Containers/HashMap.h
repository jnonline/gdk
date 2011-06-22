/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	// ============================================================================
	// This custom map is used for storing items using a UInt32 unique hash code for each object.
	//   The object being used for the TValue must expose the following methods:
	//
	// UInt32 GetHashCode();
	//

	template<typename TValue>
	class HashMap
	{
	public:
		// Public Types
		// ===============================

		typedef  TValue					ValueType;
		typedef map<UInt32, TValue>	Map;
		typedef typename Map::iterator			Iterator;

		// The actual Map
		// ===============================

		Map Items;

		// Public Methods
		// ===============================

		// Adds an item to the map by its hash
		void Add(TValue value)
		{
			UInt32 hash = value.GetHashCode();
			Items[hash] = value;
		}

		// Removes an item from the map 
		void Remove(TValue value)
		{
			UInt32 hash = value.GetHashCode();
			Items.erase(hash);
		}

		// Removes an item from the map by it's iterator
		Iterator Remove(Iterator iter)
		{
			return Items.erase(iter);
		}

		// Finds an item in the map
		Iterator Find(TValue value)
		{
			UInt32 hash = value.GetHashCode();
			return Items.find(hash);
		}	

		// Finds an item in the map, by the given hash
		Iterator FindByHash(UInt32 hash)
		{
			return Items.find(hash);
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

	// ============================================================================
	// Partial specialization for * types

	template<typename TValue>
	class HashMap<TValue*>
	{
	public:
		// Public Types
		// ===============================

		typedef TValue*				ValueType;
		typedef map<UInt32, TValue*>	Map;
		typedef typename Map::iterator			Iterator;

		// The actual Map
		// ===============================

		Map Items;

		// Public Methods
		// ===============================

		// Adds an item to the map by its hash
		void Add(TValue* value)
		{
			UInt32 hash = value->GetHashCode();
			Items[hash] = value;
		}

		// Removes an item from the map 
		void Remove(TValue* value)
		{
			UInt32 hash = value->GetHashCode();
			Items.erase(hash);
		}

		// Removes an item from the map by it's iterator
		Iterator Remove(Iterator iter)
		{
			return Items.erase(iter);
		}

		// Finds an item in the map
		Iterator Find(TValue* value)
		{
			UInt32 hash = value->GetHashCode();
			return Items.find(hash);
		}	

		// Finds an item in the map, by the given hash
		Iterator FindByHash(UInt32 hash)
		{
			return Items.find(hash);
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
