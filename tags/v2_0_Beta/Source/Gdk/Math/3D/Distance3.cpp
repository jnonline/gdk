/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Distance3.h"

using namespace Gdk;

// ===================================================================================
// Point Distance Methods
// ===================================================================================

// *************************************************************************************
float Distance3::PointToRay(const Vector3& point, const Ray3& ray)
{
    Vector3 diff = point - ray.Origin;
    Vector3 closest;
    
    // Project the difference vector onto the ray vector
    float projection = ray.Direction.Dot(diff);
    if (projection > 0.0f)
    {
        // The closest point is the projection onto the ray vector
        closest = ray.Origin + projection * ray.Direction;
    }
    else
    {
        // The projection is behind the ray, so the ray origin is the closest point
        closest = ray.Origin;
    }
   
    // Return the distance from the point to the closest ray point
    return (closest - point).Length();
}

// *************************************************************************************
float Distance3::PointToPlane(const Vector3& point, const Plane3& plane)
{
    // Get the distance from the point to the plane
    return Math::Abs(plane.Normal.Dot(point) - plane.D);
}

// *************************************************************************************
float Distance3::PointToBox(const Vector3& point, const Box3& box)
{
    // Get the vector from the box center to the point
    Vector3 diff = point - box.Center;
    
    float squaredDistance = 0.0f;
    float delta;
    float closest[3];
    
    // Loop through the 3 axes, calculating the closest point along each axis of the box
    for (int i = 0; i < 3; i++)
    {
        // Find the closest point on this axis
        closest[i] = diff.Dot(box.Axis[i]);
        
        if (closest[i] < -box.Extent[i])
        {
            delta = closest[i] + box.Extent[i];
            squaredDistance += delta * delta;
            closest[i] = -box.Extent[i];
        }
        else if (closest[i] > box.Extent[i])
        {
            delta = closest[i] - box.Extent[i];
            squaredDistance += delta * delta;
            closest[i] = box.Extent[i];
        }
    }
    
    // Return the calculated distance
    return Math::Sqrt(squaredDistance);
}

// *************************************************************************************
float Distance3::PointToSphere(const Vector3& point, const Sphere3& sphere)
{
    // Get the distance from the point to the circle center
    float diff = (point - sphere.Center).Length();
    
    // Subtract the radius  (and cap the distance as a minimum of 0)
    float distance = diff - sphere.Radius;
    return distance >= 0.0f ? distance : 0.0f;
}
