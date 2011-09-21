/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Distance2.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Calculates the distance between two points
// *****************************************************************
float Distance2::PointToPoint(const Vector2& point1, const Vector2& point2)
{
    return Vector2::Distance(point1, point2);
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a ray
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance2::PointToRay(const Vector2& point, const Ray2& ray)
{
    Vector2 diff = point - ray.Origin;
    Vector2 closest;
    
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

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a line segment
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance2::PointToLineSegment(const Vector2& point, const LineSegment2& lineSegment)
{
    // Get the direction vector & length of the line segment
    Vector2 segmentDirection = lineSegment.End - lineSegment.Start;
    float segmentLength = segmentDirection.Normalize();
 
    // Get the vector from the segment starting point to the distance point
    Vector2 diff = point - lineSegment.Start;
    Vector2 closest;
    
    // Project the difference vector onto the line segment vector
    float projection = segmentDirection.Dot(diff);
    
    // Is the projection before the segment start?
    if(projection < 0.0f)
    {
        // The closest point is the segment start
        closest = lineSegment.Start;
    }
    // Is the projection after the segment end?
    else if(projection > segmentLength)
    {
        // The closest point is the segment end
        closest = lineSegment.End;
    }
    else
    {
        // The closest point is the projection onto the line segment vector
        closest = lineSegment.Start + projection * segmentDirection;
    }
        
    // Return the distance from the point to the closest projection point
    return (closest - point).Length();
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and an oriented box
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance2::PointToBox(const Vector2& point, const Box2& box)
{
    // Get the vector from the box center to the point
    Vector2 diff = point - box.Center;
    
    float squaredDistance = 0.0f;
    float delta;
    float closest[2];
    
    // Loop through the 2 axes, calculating the closest point along each axis of the box
    for (int i = 0; i < 2; i++)
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

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a rectangle
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance2::PointToRectangle(const Vector2& point, const Rectangle2& rect)
{
    return PointToBox(point, rect.GetAsBox());
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a circle
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance2::PointToCircle(const Vector2& point, const Circle2& circle)
{
    // Get the distance from the point to the circle center
    float diff = (point - circle.Center).Length();
    
    // Subtract the radius  (and cap the distance as a minimum of 0)
    float distance = diff - circle.Radius;
    return distance >= 0.0f ? distance : 0.0f;
}
