/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Circle2.h"

using namespace Gdk;

// instantiations of static constants
const Circle2 Circle2::ZERO(0.0f, 0.0f, 0.0f);

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Circle2::Circle2()
{
	Center = Vector2::ZERO;
	Radius = 0.0f;
}

// ***********************************************************************
Circle2::Circle2(float x, float y, float radius)
{
	Center.X = x;
	Center.Y = y;
	Radius = radius;
}

// ***********************************************************************
Circle2::Circle2(const Vector2& position, float radius)
{
	Center = position;
	Radius = radius;
}

// ***********************************************************************
Circle2::Circle2(const Circle2& input)
{
	Center = input.Center;
	Radius = input.Radius;
}

// ===================================================================================
// Utility Methods
// ===================================================================================

// ***********************************************************************
Vector2 Circle2::GetParametricPointInArea(float tAngle, float tRadius)
{
	// Return a point in the circle using the given parametric coordinates within circle space
	float theta = tAngle * Math::TWO_PI;
	float sinTheta = Math::Sin(theta);
	float cosTheta = Math::Cos(theta);
	float radius = tRadius * this->Radius;

	return Vector2(
		this->Center.X + cosTheta * radius, 
		this->Center.Y + sinTheta * radius
		);
}

// ***********************************************************************
Vector2 Circle2::GetParametricPointOnPerimeter(float tAngle)
{
	// Return a point on the perimeter of the circle using a parametric coordinate
	float theta = tAngle * Math::TWO_PI;
	float sinTheta = Math::Sin(theta);
	float cosTheta = Math::Cos(theta);

	return Vector2(
		this->Center.X + cosTheta * this->Radius, 
		this->Center.Y + sinTheta * this->Radius
		);
}
