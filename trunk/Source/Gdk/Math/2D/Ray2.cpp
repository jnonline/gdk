/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Ray2.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Ray2::Ray2()
{
}

// *****************************************************************
/// @brief
///     Constructs a ray from the given values
/// @param origin
///     Origin of the ray
/// @param direction
///     Direction of the ray.  (most GDK math assumes the ray direction is normalized)
// *****************************************************************
Ray2::Ray2(const Vector2& origin, const Vector2& direction)
{
	Origin = origin;
	Direction = direction;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Ray2::Ray2(const Ray2& input)
{
	Origin = input.Origin;
	Direction = input.Direction;
}



