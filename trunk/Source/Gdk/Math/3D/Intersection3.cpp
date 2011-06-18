
#include "BasePCH.h"
#include "Intersection3.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Intersection3::Intersection3()
{
}


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
bool Intersection3::Find(const Ray3& ray, const Plane3& plane, Vector3& intersectionPoint)
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
			intersectionPoint = ray.Origin;
			return true;
		}

		// They ray is parallel & not on the plane, no intersection
		return false;
	}

	// Does the ray point towards the plane?
	float rayT = -shortestDistance / directionDotNormal;
	if(rayT >= 0.0f)
	{
		// The ray goes through the plane, so get the point
		intersectionPoint = ray.Origin + ray.Direction * rayT;
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

	// Get the dot product of the difference & ray direction vectors
	float diffDotDirection = diff.Dot(ray.Direction);

	// Is the ray pointing away from the sphere?
	if(diffDotDirection < 0.0f)
		return false;

	// Quadratic has a real root if the discriminant is non-negative
	return diffDotDirection * diffDotDirection >= (distSq - radSq);
}

// *************************************************************************************
bool Intersection3::Find(const Ray3& ray, const Sphere3& sphere, Vector3& intersectionPoint)
{
	// Get the Ray.Origin -> Sphere.Center vector
	Vector3 diff = sphere.Center - ray.Origin;

	float distSq = diff.LengthSquared();
	float radSq = sphere.Radius * sphere.Radius;

	// Check if the ray origin is inside the sphere
	if(distSq <= radSq)
	{
		// The ray origin is in the sphere
		intersectionPoint = ray.Origin;
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

	float distance = diffDotDirection - Math::Sqrt(radSq - a);
	intersectionPoint = ray.Origin + ray.Direction * distance;
	return true;
}
