/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Ray2
	{
	public:
		// Components
		Vector2 Origin;
		Vector2 Direction;  // Direction is always assumed to be normalized

		// Constructors
		Ray2 ();
		Ray2 (const Vector2& origin, const Vector2& direction);
		Ray2 (const Ray2& input);

		// Asssignment Operator
		inline Ray2& operator= (const Ray2& input);
        
	};
    
	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================
    
	// ***********************************************************************
	inline Ray2& Ray2::operator= (const Ray2& input)
	{
		Origin = input.Origin;
		Direction = input.Direction;
		return *this;
	}
    
} // namespace Gdk
