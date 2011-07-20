/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "UniformValue.h"

namespace Gdk
{
	// ============================================================================
	class SharedUniformValueSet
	{
	public:
		// Public Properties
		// -----------------------

		// Hash map of values by name
		UniformValueNameMap		ValuesByName;
		
		// Public Methods
		// -----------------------

		// Adds a uniform value to this set
		UniformValue* Add(const char* name, UniformTypes::Enum type, int arraySize = 1);

		// Removes & destroys the given uniform value (NOTE: Make sure no other UniformValues's are bound to this UniformValue)
		void Remove(const char* name);

		// Looks up a uniform value by name	
		UniformValue* Get(const char* name);

		// CTor/DTor
		SharedUniformValueSet();
		virtual ~SharedUniformValueSet();

	private:
		// Internal Properties
		// -----------------------

		// Internal Methods
		// -----------------------

	};

} // namespace 
