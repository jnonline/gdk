/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Sphere3.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Sphere3::Sphere3()
{
}

// *****************************************************************
/// @brief
///     Constructs a new sphere from a center point and a radius
// *****************************************************************
Sphere3::Sphere3(float x, float y, float z, float radius)
{
	Center.X = x;
	Center.Y = y;
	Center.Z = z;
	Radius = radius;
}

// *****************************************************************
/// @brief
///     Constructs a new sphere from a center point and a radius
// *****************************************************************
Sphere3::Sphere3(const Vector3& center, float radius)
{
	Center = center;
	Radius = radius;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Sphere3::Sphere3(const Sphere3& input)
{
	Center = input.Center;
	Radius = input.Radius;
}


// *****************************************************************
/// @brief
///     Creates an approximate-fit sphere around a set of points.
/// @param numPoints
///     The number of points in the [points] array
/// @param points
///     An array of points to be used for finding a fitting sphere
// *****************************************************************
Sphere3 Sphere3::FromPoints(size_t numPoints, const Vector3* points)
{
    if(numPoints == 0)
        return Sphere3(Vector3::ZERO, 0.0f);
    
    // Start with a 0 radius sphere, centered on the first point.
    Sphere3 result(points[0], 0.0f);
    
    // Loop through the rest of the points
    for(size_t pointIndex=1; pointIndex < numPoints; pointIndex++)
    {
        const Vector3& point = points[pointIndex];
        
        // Get the vector from the center of the sphere, to this point
        Vector3 pointToCenter = result.Center - point;
        
        // If the point is already in the sphere, move on to the next point
        float lenSquared = pointToCenter.LengthSquared();
        if(lenSquared < (result.Radius * result.Radius))
            continue;
        
        // Calculate the edge of the sphere if we extended it to encapsulate the point
        Vector3 sphereEdge = pointToCenter;
        sphereEdge.Normalize();
        sphereEdge *= result.Radius;
        sphereEdge += result.Center;
        
        // Calculate a new center for the sphere, halfway between the edge of the sphere & this point
        result.Center = (point + sphereEdge) * 0.5f;
        result.Radius = (point - sphereEdge).Length() * 0.5f;
    }
    
    return result;
}


// *****************************************************************
/// @brief
///     Creates a best-fit sphere that encapsulates two given spheres
/// @param c1
///     First sphere to encapsulate
/// @param c2
///     Second sphere to encapsulate
// *****************************************************************
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

	// Are the spheres on different centers?
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
