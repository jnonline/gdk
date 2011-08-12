/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Intersection3.h"

using namespace Gdk;

// ===================================================================================
// Ray -> Plane
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Ray3& ray, const Plane3& plane)
{
	float directionDotNormal = ray.Direction.Dot(plane.Normal);
	float shortestDistance = plane.DistanceTo(ray.Origin);

	// Are the plane & ray direction parallel?
	if(Math::Abs(directionDotNormal) <= Math::ZERO_TOLERANCE)
	{
		// Is the ray is ON the plane?
		if(Math::Abs(shortestDistance) <= Math::ZERO_TOLERANCE)
			return true;

		// They ray is parallel & not on the plane, no intersection
		return false;
	}

	// Does the ray point towards the plane?
	float rayT = -shortestDistance / directionDotNormal;
	if(rayT >= 0.0f)
	{
		// The ray goes through the plane
		return true;
	}

    // No intersection
    return false;
}

// *************************************************************************************
bool Intersection3::Find(const Ray3& ray, const Plane3& plane, float& distance)
{
	float directionDotNormal = ray.Direction.Dot(plane.Normal);
	float shortestDistance = plane.DistanceTo(ray.Origin);

	// Are the plane & ray direction parallel?
	if(Math::Abs(directionDotNormal) <= Math::ZERO_TOLERANCE)
	{
		// Is the ray is ON the plane?
		if(Math::Abs(shortestDistance) <= Math::ZERO_TOLERANCE)
		{
			// Return the ray origin as the intersection point
			distance = 0.0f;
			return true;
		}

		// They ray is parallel & not on the plane, no intersection
		return false;
	}

	// Does the ray point towards the plane?
	distance = -shortestDistance / directionDotNormal;
	if(distance >= 0.0f)
	{
		// The ray goes through the plane
		return true;
	}

    // No intersection
    return false;
}

// ===================================================================================
// Ray -> Sphere
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Ray3& ray, const Sphere3& sphere)
{
	// Get the Ray.Origin -> Sphere.Center vector
	Vector3 diff = sphere.Center - ray.Origin;

	float distSq = diff.LengthSquared();
	float radSq = sphere.Radius * sphere.Radius;

	// Check if the ray origin is inside the sphere
	if(distSq <= radSq)
		return true;	// The ray origin is in the sphere

	// Project the difference vector onto the ray
	float directionDotDiff = ray.Direction.Dot(diff);

	// Is the ray pointing away from the sphere?
	if(directionDotDiff < 0.0f)
		return false;
    
	// Quadratic has a real root if the discriminant is non-negative
	return directionDotDiff * directionDotDiff >= (distSq - radSq);
}

// *************************************************************************************
bool Intersection3::Find(const Ray3& ray, const Sphere3& sphere, float& distance)
{
	// Get the Ray.Origin -> Sphere.Center vector
	Vector3 diff = sphere.Center - ray.Origin;

	float distSq = diff.LengthSquared();
	float radSq = sphere.Radius * sphere.Radius;

	// Check if the ray origin is inside the sphere
	if(distSq <= radSq)
	{
		// The ray origin is in the sphere
		distance = 0.0f;
		return true;	
	}

	// Is the ray is pointing away from the sphere?
	float diffDotDirection = diff.Dot(ray.Direction);
	if(diffDotDirection < 0.0f)
	{
		// No intersection
		return false;
	}

	float a = distSq - diffDotDirection * diffDotDirection;
	if(a > radSq)
		return false;

    distance = diffDotDirection - Math::Sqrt(radSq - a);
	return true;
}

// ===================================================================================
// Ray -> Box
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Ray3& ray, const Box3& box)
{
    float WdU[3], AWdU[3], DdU[3], ADdU[3], AWxDdU[3], RHS;
    Vector3 diff = ray.Origin - box.Center;
    
    WdU[0] = ray.Direction.Dot(box.Axis[0]);
    AWdU[0] = Math::Abs(WdU[0]);
    DdU[0] = diff.Dot(box.Axis[0]);
    ADdU[0] = Math::Abs(DdU[0]);
    if (ADdU[0] > box.Extent[0] && DdU[0]*WdU[0] >= 0.0f)
    {
        return false;
    }
    
    WdU[1] = ray.Direction.Dot(box.Axis[1]);
    AWdU[1] = Math::Abs(WdU[1]);
    DdU[1] = diff.Dot(box.Axis[1]);
    ADdU[1] = Math::Abs(DdU[1]);
    if (ADdU[1] > box.Extent[1] && DdU[1]*WdU[1] >= 0.0f)
    {
        return false;
    }
    
    WdU[2] = ray.Direction.Dot(box.Axis[2]);
    AWdU[2] = Math::Abs(WdU[2]);
    DdU[2] = diff.Dot(box.Axis[2]);
    ADdU[2] = Math::Abs(DdU[2]);
    if (ADdU[2] > box.Extent[2] && DdU[2]*WdU[2] >= 0.0f)
    {
        return false;
    }
    
    Vector3 WxD = ray.Direction.Cross(diff);
    
    AWxDdU[0] = Math::Abs(WxD.Dot(box.Axis[0]));
    RHS = box.Extent[1]*AWdU[2] + box.Extent[2]*AWdU[1];
    if (AWxDdU[0] > RHS)
    {
        return false;
    }
    
    AWxDdU[1] = Math::Abs(WxD.Dot(box.Axis[1]));
    RHS = box.Extent[0]*AWdU[2] + box.Extent[2]*AWdU[0];
    if (AWxDdU[1] > RHS)
    {
        return false;
    }
    
    AWxDdU[2] = Math::Abs(WxD.Dot(box.Axis[2]));
    RHS = box.Extent[0]*AWdU[1] + box.Extent[1]*AWdU[0];
    if (AWxDdU[2] > RHS)
    {
        return false;
    }
    
    return true;
}

// *************************************************************************************
bool Intersection3::Find(const Ray3& ray, const Box3& box, float& distance)
{
    // Do a line/box intersection using ray based T values
    float t[2];
    IntersectionType::Enum intersectionType =
        Intersect_Line_Box(0.0f, FLT_MAX, ray.Origin, ray.Direction, box, t);
    
    // If we have 2 intersection points, return the closer of the 2
    if(intersectionType == IntersectionType::LineSegment)
        distance = t[0] < t[1] ? t[0] : t[1];
    else 
        distance = t[0];
    
    return intersectionType != IntersectionType::None;
}

// ===================================================================================
// Ray -> Frustrum
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Ray3& ray, const Frustrum3& frustrum)
{
    float distance;
    return Find(ray, frustrum, distance);
}

// *************************************************************************************
bool Intersection3::Find(const Ray3& ray, const Frustrum3& frustrum, float& distance)
{
    distance = 0.0f;
    
    // Check if the ray origin is inside the frustrum
    if(frustrum.Contains(ray.Origin))
        return true;
    
    float minValue = FLT_MIN;
    float maxValue = FLT_MAX;
    
    // Loop through the 6 planes of the frustrum
    for(int planeIndex=0; planeIndex<6; planeIndex++)
    {
        const Plane3& plane = frustrum.Planes[planeIndex];
        
        // Get the dot product of the ray direction/origin and the plane normal
        float dirDotNormal = ray.Direction.Dot(plane.Normal);
        float origDotNormal = ray.Origin.Dot(plane.Normal);
        origDotNormal += plane.D;
        
        if(Math::Abs(dirDotNormal) <= Math::ZERO_TOLERANCE)
        {
            if(origDotNormal > 0.0f)
            {
                return false;
            }
        }
        else
        {
            float inv = -origDotNormal / dirDotNormal;
            if(dirDotNormal < 0.0f)
            {
                if(inv > maxValue)
                    return false;
                if(inv > minValue)
                    minValue = inv;
            }
            else
            {
                if(inv < minValue)
                    return false;
                if(inv < maxValue)
                    maxValue = inv;
            }
        }
    } // for
    
    float final = (minValue >= 0.0f) ? minValue : maxValue;
    if(final >= 0.0f)
    {
        distance = final;
        return true;
    }
    
    return false;
}


// ===================================================================================
// Frustrum -> Plane
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Frustrum3& frustrum, const Plane3& plane)
{
    int sides = 0;
    
    // Loop through the 8 corners of the frustrum
	for(int i=0; i<8; i++)
    {
        // Get the distance from the corner to the plane
        float dist = frustrum.Corners[i].Dot(plane.Normal);
        if((dist + plane.D) > 0.0f)
           sides |= 1;  // At least 1 corner is on the front side of the plane
        else
           sides |= 2;  // At least 1 corner is on the back side of the plane
        
        // Have we found both a corner in front & behind the plane?
        if(sides == 3)
           return true;
    }
    
    // All the corners lay on 1 side of the plane
    return false;
}

// ===================================================================================
// Frustrum -> Sphere
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Frustrum3& frustrum, const Sphere3& sphere)
{
    // Loop through the planes of the frustrum
    for(int i=0; i<6; i++)
    {
        const Plane3& plane = frustrum.Planes[i];
        
        // Make sure the sphere intersects or is behind this plane
        float dist = plane.Normal.Dot(sphere.Center) + plane.D;
        if(dist > sphere.Radius)
            return false;
    }
    
    // The sphere intersects or is behind all 6 frustrum planes (and thusly inside the frustrum)
    return true;
}

// ===================================================================================
// Plane -> Plane
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Plane3& plane1, const Plane3& plane2)
{
    // Check if the planes have the same orientation
    float dot = plane1.Normal.Dot(plane2.Normal);
    if(Math::Abs(dot) < 1.0f - Math::ZERO_TOLERANCE)
        return true;
    
    // Planes are parallel, check if they are co-planar
    if(plane1.D == plane2.D)
        return true; // Co-planar
    
    return false;
}

// ===================================================================================
// Plane -> Sphere
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Plane3& plane, const Sphere3& sphere)
{
    // Get the distance from the plane to the sphere center
    float distance = plane.DistanceTo(sphere.Center);
    
    // If the distance is less than the radius, the sphere & plane intersect
    return Math::Abs(distance) <= sphere.Radius;
}

// ===================================================================================
// Plane -> Box
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Plane3& plane, const Box3& box)
{
    // Get the distance from the box center, to the corner/edge closest to the plane
    float radius = 
        Math::Abs( box.Extent[0] * plane.Normal.Dot(box.Axis[0])) +
        Math::Abs( box.Extent[1] * plane.Normal.Dot(box.Axis[1])) +
        Math::Abs( box.Extent[2] * plane.Normal.Dot(box.Axis[2]));
    
    // Get the total distance from the plane to the box center
    float distance = plane.DistanceTo(box.Center);
    
    // Check if the box distance is within the corner/edge radius
    return Math::Abs(distance) <= radius;
}

// ===================================================================================
// Sphere -> Sphere
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Sphere3& sphere1, const Sphere3& sphere2)
{
    // Get the distance (squared) between the sphere centers
    float diffSquared = (sphere1.Center - sphere2.Center).LengthSquared();
    
    // Check if the distance is less than the total radii
    float totalRadius = sphere1.Radius + sphere2.Radius;
    return diffSquared <= (totalRadius * totalRadius);
}

// ===================================================================================
// Sphere -> Box
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Sphere3& sphere, const Box3& box)
{
    // Get the distance from the box to the sphere center
    float distance = Distance3::PointToBox(sphere.Center, box);
    
    // Is the distance within the sphere radius?
    return distance <= sphere.Radius;
}

// ===================================================================================
// Box -> Box
// ===================================================================================

// *************************************************************************************
bool Intersection3::Test(const Box3& box1, const Box3& box2)
{
    const float cutoff = 1.0f - Math::ZERO_TOLERANCE;
    bool existsParallelPair = false;
    int i;
    
    // Convenience variables.
    const Vector3* A = box1.Axis;
    const Vector3* B = box2.Axis;
    const float* EA = box1.Extent;
    const float* EB = box2.Extent;
    
    // Compute difference of box centers, D = C1-C0.
    Vector3 D = box2.Center - box1.Center;
    
    float C[3][3];
    float AbsC[3][3];
    float AD[3];
    float r0, r1, r;
    float r01;
    
    // axis C0+t*A0
    for (i = 0; i < 3; ++i)
    {
        C[0][i] = A[0].Dot(B[i]);
        AbsC[0][i] = Math::Abs(C[0][i]);
        if (AbsC[0][i] > cutoff)
        {
            existsParallelPair = true;
        }
    }
    AD[0] = A[0].Dot(D);
    r = Math::Abs(AD[0]);
    r1 = EB[0]*AbsC[0][0] + EB[1]*AbsC[0][1] + EB[2]*AbsC[0][2];
    r01 = EA[0] + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A1
    for (i = 0; i < 3; ++i)
    {
        C[1][i] = A[1].Dot(B[i]);
        AbsC[1][i] = Math::Abs(C[1][i]);
        if (AbsC[1][i] > cutoff)
        {
            existsParallelPair = true;
        }
    }
    AD[1] = A[1].Dot(D);
    r = Math::Abs(AD[1]);
    r1 = EB[0]*AbsC[1][0] + EB[1]*AbsC[1][1] + EB[2]*AbsC[1][2];
    r01 = EA[1] + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A2
    for (i = 0; i < 3; ++i)
    {
        C[2][i] = A[2].Dot(B[i]);
        AbsC[2][i] = Math::Abs(C[2][i]);
        if (AbsC[2][i] > cutoff)
        {
            existsParallelPair = true;
        }
    }
    AD[2] = A[2].Dot(D);
    r = Math::Abs(AD[2]);
    r1 = EB[0]*AbsC[2][0] + EB[1]*AbsC[2][1] + EB[2]*AbsC[2][2];
    r01 = EA[2] + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*B0
    r = Math::Abs(B[0].Dot(D));
    r0 = EA[0]*AbsC[0][0] + EA[1]*AbsC[1][0] + EA[2]*AbsC[2][0];
    r01 = r0 + EB[0];
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*B1
    r = Math::Abs(B[1].Dot(D));
    r0 = EA[0]*AbsC[0][1] + EA[1]*AbsC[1][1] + EA[2]*AbsC[2][1];
    r01 = r0 + EB[1];
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*B2
    r = Math::Abs(B[2].Dot(D));
    r0 = EA[0]*AbsC[0][2] + EA[1]*AbsC[1][2] + EA[2]*AbsC[2][2];
    r01 = r0 + EB[2];
    if (r > r01)
    {
        return false;
    }
    
    // At least one pair of box axes was parallel, so the separation is
    // effectively in 2D where checking the "edge" normals is sufficient for
    // the separation of the boxes.
    if (existsParallelPair)
    {
        return true;
    }
    
    // axis C0+t*A0xB0
    r = Math::Abs(AD[2]*C[1][0] - AD[1]*C[2][0]);
    r0 = EA[1]*AbsC[2][0] + EA[2]*AbsC[1][0];
    r1 = EB[1]*AbsC[0][2] + EB[2]*AbsC[0][1];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A0xB1
    r = Math::Abs(AD[2]*C[1][1] - AD[1]*C[2][1]);
    r0 = EA[1]*AbsC[2][1] + EA[2]*AbsC[1][1];
    r1 = EB[0]*AbsC[0][2] + EB[2]*AbsC[0][0];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A0xB2
    r = Math::Abs(AD[2]*C[1][2] - AD[1]*C[2][2]);
    r0 = EA[1]*AbsC[2][2] + EA[2]*AbsC[1][2];
    r1 = EB[0]*AbsC[0][1] + EB[1]*AbsC[0][0];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A1xB0
    r = Math::Abs(AD[0]*C[2][0] - AD[2]*C[0][0]);
    r0 = EA[0]*AbsC[2][0] + EA[2]*AbsC[0][0];
    r1 = EB[1]*AbsC[1][2] + EB[2]*AbsC[1][1];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A1xB1
    r = Math::Abs(AD[0]*C[2][1] - AD[2]*C[0][1]);
    r0 = EA[0]*AbsC[2][1] + EA[2]*AbsC[0][1];
    r1 = EB[0]*AbsC[1][2] + EB[2]*AbsC[1][0];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A1xB2
    r = Math::Abs(AD[0]*C[2][2] - AD[2]*C[0][2]);
    r0 = EA[0]*AbsC[2][2] + EA[2]*AbsC[0][2];
    r1 = EB[0]*AbsC[1][1] + EB[1]*AbsC[1][0];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A2xB0
    r = Math::Abs(AD[1]*C[0][0] - AD[0]*C[1][0]);
    r0 = EA[0]*AbsC[1][0] + EA[1]*AbsC[0][0];
    r1 = EB[1]*AbsC[2][2] + EB[2]*AbsC[2][1];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A2xB1
    r = Math::Abs(AD[1]*C[0][1] - AD[0]*C[1][1]);
    r0 = EA[0]*AbsC[1][1] + EA[1]*AbsC[0][1];
    r1 = EB[0]*AbsC[2][2] + EB[2]*AbsC[2][0];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    // axis C0+t*A2xB2
    r = Math::Abs(AD[1]*C[0][2] - AD[0]*C[1][2]);
    r0 = EA[0]*AbsC[1][2] + EA[1]*AbsC[0][2];
    r1 = EB[0]*AbsC[2][1] + EB[1]*AbsC[2][0];
    r01 = r0 + r1;
    if (r > r01)
    {
        return false;
    }
    
    return true;
}


// ===================================================================================
// Internal Utility Methods
// ===================================================================================

// These methods are used to find intersections between like primitives.
//  For instance, most line/ray/lineSegment tests can be done as lines.



// *************************************************************************************
IntersectionType::Enum  Intersection3::Intersect_Line_Box(
    float startT, float endT,
    const Vector3& origin, const Vector3& direction,
    const Box3& box,
    float t[2]
    )
{
    // Convert linear component to box coordinates.
    Vector3 diff = origin - box.Center;
    Vector3 boxOrigin(
        diff.Dot(box.Axis[0]),
        diff.Dot(box.Axis[1]),
        diff.Dot(box.Axis[2])
        );
    Vector3 boxDirection(
        direction.Dot(box.Axis[0]),
        direction.Dot(box.Axis[1]),
        direction.Dot(box.Axis[2])
        );
    
    // Clip the line against the 6 box planes
    bool notAllClipped =
        Clip_Box_Line(+boxDirection.X, -boxOrigin.X - box.Extent[0], startT, endT) &&
        Clip_Box_Line(-boxDirection.X, +boxOrigin.X - box.Extent[0], startT, endT) &&
        Clip_Box_Line(+boxDirection.Y, -boxOrigin.Y - box.Extent[1], startT, endT) &&
        Clip_Box_Line(-boxDirection.Y, +boxOrigin.Y - box.Extent[1], startT, endT) &&
        Clip_Box_Line(+boxDirection.Z, -boxOrigin.Z - box.Extent[2], startT, endT) &&
        Clip_Box_Line(-boxDirection.Z, +boxOrigin.Z - box.Extent[2], startT, endT);
    
    // Did we find any intersections?
    if (notAllClipped)
    {
        t[0] = startT;
        t[1] = endT;
        
        // Did we find 2 intersection points?
        if (endT > startT)
            return IntersectionType::LineSegment;
        return IntersectionType::Point;
    }
    
    // No intersection
    return IntersectionType::None;
}

// *************************************************************************************
bool Intersection3::Clip_Box_Line(float denominator, float numerator, float& t0, float& t1)
{
    // Return value is 'true' if line segment intersects the current test
    // plane.  Otherwise 'false' is returned in which case the line segment
    // is entirely clipped.
    
    if (denominator > 0.0f)
    {
        if (numerator > denominator * t1)
        {
            return false;
        }
        if (numerator > denominator * t0)
        {
            t0 = numerator / denominator;
        }
        return true;
    }
    else if (denominator < 0.0f)
    {
        if (numerator > denominator * t0)
        {
            return false;
        }
        if (numerator > denominator * t1)
        {
            t1 = numerator / denominator;
        }
        return true;
    }
    else
    {
        return numerator <= 0.0f;
    }
}