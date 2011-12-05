/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "SharedUniformValueSet.h"

using namespace Gdk;

// ***********************************************************************
SharedUniformValueSet::SharedUniformValueSet()
{
}

// ***********************************************************************
SharedUniformValueSet::~SharedUniformValueSet()
{
	// Loop through all the uniform values
	for(UniformValueNameMap::Iterator iter = ValuesByName.Begin(); iter != ValuesByName.End(); iter++)
	{
		// Delete this uniform value param
		UniformValue* value = iter->second;
		GdkDelete(value);
	}
}


// ***********************************************************************
UniformValue* SharedUniformValueSet::Add(const char* name, UniformTypes::Enum type, int arraySize)
{
	// Check if we already have a uniform value by this name
	UniformValueNameMap::Iterator iter = ValuesByName.Find(name);
	if(iter != ValuesByName.End())
	{
		// Value already exists!
		UniformValue* value = iter->second;

		// Check the type & size match
		ASSERT(value->GetUniformType() == type, L"Attempting to add shared uniform value with same name, but different type");
		ASSERT(value->GetArraySize() == arraySize, L"Attempting to add shared uniform value with same name, but different array size");

		// Return the existing
		return value;
	}

	// Create the new uniform value
	UniformValue* newValue = GdkNew UniformValue(type, arraySize);
	ValuesByName.Add(name, newValue);

	// Return the new uniform value
	return newValue;
}

// ***********************************************************************
void SharedUniformValueSet::Remove(const char* name)
{
	// Get the uniform value with this name
	UniformValueNameMap::Iterator iter = ValuesByName.Find(name);
	if(iter != ValuesByName.End())
	{
		ValuesByName.Remove(iter);
	}
}

// ***********************************************************************
UniformValue* SharedUniformValueSet::Get(const char* name)
{
	// Get the uniform value with this name
	UniformValueNameMap::Iterator iter = ValuesByName.Find(name);
	if(iter == ValuesByName.End())
		return NULL;

	return iter->second;
}