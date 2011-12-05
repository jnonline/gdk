/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Ray3.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Ray3::Ray3()
{
}

// *****************************************************************
/// @brief
///     Constructs a new ray from a given origin & direction
// *****************************************************************
Ray3::Ray3(const Vector3& origin, const Vector3& direction)
{
	Origin = origin;
	Direction = direction;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Ray3::Ray3(const Ray3& input)
{
	Origin = input.Origin;
	Direction = input.Direction;
}



