/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Sphere3.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Sphere3::Sphere3()
{
}

// ***********************************************************************
Sphere3::Sphere3(float x, float y, float z, float radius)
{
	Center.X = x;
	Center.Y = y;
	Center.Z = z;
	Radius = radius;
}

// ***********************************************************************
Sphere3::Sphere3(const Vector3& center, float radius)
{
	Center = center;
	Radius = radius;
}

// ***********************************************************************
Sphere3::Sphere3(const Sphere3& input)
{
	Center = input.Center;
	Radius = input.Radius;
}


// ===================================================================================
// Encapsulation Methods
// ===================================================================================

// ***********************************************************************
Sphere3 Sphere3::Merge(const Sphere3& sphere1, const Sphere3& sphere2)
{
	// Get the difference vector between the centers
	Vector3 diff = sphere2.Center - sphere1.Center;
	float lenSquared = diff.LengthSquared();
	float radiusDiff = sphere2.Radius - sphere1.Radius;
	float radiusDiffSqr = radiusDiff * radiusDiff;

	// Does one sphere already encapsulate the other?
	if(radiusDiffSqr >= lenSquared)
	{
		// Return the bigger (Encapsulating) sphere
		if(radiusDiff >= 0)
			return sphere2;
		else
			return sphere1;
	}

	// Get the actual length of the difference of sphere centers
	float len = Math::Sqrt(lenSquared);
	Sphere3 result;

	// Are the sphere on different centers?
	if(len > Math::ZERO_TOLERANCE)
	{
		// Calculate the new center of the encapsulating sphere
		float coeff = (len + radiusDiff) / (2.0f * len);
		result.Center = sphere1.Center + coeff * diff;
	}
	else
	{
		result.Center = sphere1.Center;
	}

	// Set the new radius
	result.Radius = 0.5f * (len + sphere1.Radius + sphere2.Radius);
	
	return result;
}
