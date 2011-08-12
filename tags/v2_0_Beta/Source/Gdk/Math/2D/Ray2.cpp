/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Ray2.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Ray2::Ray2()
{
}

// ***********************************************************************
Ray2::Ray2(const Vector2& origin, const Vector2& direction)
{
	Origin = origin;
	Direction = direction;
}

// ***********************************************************************
Ray2::Ray2(const Ray2& input)
{
	Origin = input.Origin;
	Direction = input.Direction;
}



