/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Intersection2.h"

#include "Distance2.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Tests if two rays intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Ray2& ray1, const Ray2& ray2)
{
    float distance;
    return Find(ray1, ray2, distance);
}

// *****************************************************************
/// @brief
///     Finds the intersection between two rays
/// @param[in] ray1
///     One of the rays to intersect
/// @param[in] ray2
///     One of the rays to intersect
/// @param[out] distance
///     The distance from the origin of ray 1 to the point of intersection.  This value is undefined if the method returns false.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Find(const Ray2& ray1, const Ray2& ray2, float& distance)
{
    // Intersect the 2 rays as 2 lines
    float t[2];
    IntersectionType::Enum intersectionType = Intersect_Line_Line(
        ray1.Origin, ray1.Direction, 
        ray2.Origin, ray2.Direction, 
        t
        );
    
    // Do the lines intersect at a point?
    if (intersectionType == IntersectionType::Point)
    {
        // Make sure the rays are pointing towards the intersection point
        if (t[0] >= 0.0f && t[1] >= 0.0f)
        {
            // Distance along the casting ray
            distance = t[0];
            return true;
        }
        else
        {
            return false;   // At least 1 ray is pointing away from the intersection point
        }
    }
    
    distance = 0.0f;
    
    // Was there an intersection?
    return intersectionType != IntersectionType::None;
}

// *****************************************************************
/// @brief
///     Tests if a ray and a line segment intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Ray2& ray, const Segment2& segment)
{
    float distance;
    return Find(ray, segment, distance);
}

// *****************************************************************
/// @brief
///     Finds the intersection between a ray and a line segment
/// @param[in] ray
///     The ray component to intersect
/// @param[in] lineSegment
///     The line segment to intersect
/// @param[out] distance
///     The distance from the origin of the ray to the point of intersection.  This value is undefined if the method returns false.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Find(const Ray2& ray, const Segment2& segment, float& distance)
{
    // Test the ray & the line segment as lines
    float t[2];
    IntersectionType::Enum intersectionType = Intersect_Line_Line(
        ray.Origin, ray.Direction, 
        segment.Center, segment.Direction, 
        t
        );
    
    // Do the lines intersect at a point?
    if (intersectionType == IntersectionType::Point)
    {
        // Make sure the ray is pointing towards the intersection point 
        //  & the intersection point lies on the line segment
        if (t[0] >= 0.0f && 
            t[1] >= -segment.Extent && t[1] <= segment.Extent)
        {
            // Distance along the casting ray
            distance = t[0];
            return true;
        }
        else
        {
            return false;
        }
    }
    
    // Are the ray & line-segment co-linear
    if(intersectionType == IntersectionType::Line)
    {
        // TODO(P2):  this should probly return true & calculate the distance to the segment
        return false;
    }
    
    // No intersection
    return false;
}

// *****************************************************************
/// @brief
///     Tests if a ray and a circle intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Ray2& ray, const Circle2& circle)
{
    // Get the Ray.Origin -> Circle.Center vector
	Vector2 diff = circle.Center - ray.Origin;
    
	float distSq = diff.LengthSquared();
	float radSq = circle.Radius * circle.Radius;
    
	// Check if the ray origin is inside the circle
	if(distSq <= radSq)
		return true;	// The ray origin is in the circle
    
	// Get the dot product of the difference & ray direction vectors
	float diffDotDirection = diff.Dot(ray.Direction);
    
	// Is the ray pointing away from the circle?
	if(diffDotDirection < 0.0f)
		return false;
    
	// Quadratic has a real root if the discriminant is non-negative
	return diffDotDirection * diffDotDirection >= (distSq - radSq);
}

// *****************************************************************
/// @brief
///     Finds the first point of intersection between a ray and a circle
/// @param[in] ray
///     The ray component to intersect
/// @param[in] circle
///     The circle to intersect
/// @param[out] distance
///     The distance from the origin of the ray to the point of intersection.  This value is undefined if the method returns false.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Find(const Ray2& ray, const Circle2& circle, float& distance)
{
    // Get the Ray.Origin -> Circle.Center vector
	Vector2 diff = circle.Center - ray.Origin;
    
	float distSq = diff.LengthSquared();
	float radSq = circle.Radius * circle.Radius;
    
	// Check if the ray origin is inside the circle
	if(distSq <= radSq)
	{
		// The ray origin is in the circle
		distance = 0.0f;
		return true;	
	}
    
	// Is the ray is pointing away from the circle?
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

// *****************************************************************
/// @brief
///     Tests if a ray and a rectangle intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Ray2& ray, const Rectangle2& rect)
{
    // Test the ray against the rect (as a box)
    return Test(ray, rect.GetAsBox());
}

// *****************************************************************
/// @brief
///     Finds the first point of intersection between a ray and a rectangle
/// @param[in] ray
///     The ray component to intersect
/// @param[in] rectangle
///     The retangle to intersect
/// @param[out] distance
///     The distance from the origin of the ray to the point of intersection.  This value is undefined if the method returns false.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Find(const Ray2& ray, const Rectangle2& rect, float& distance)
{
    // Intersect the ray against the rect (as a box)
    return Find(ray, rect.GetAsBox(), distance);
}


// *****************************************************************
/// @brief
///     Tests if a ray and an oriented box intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Ray2& ray, const Box2& box)
{
    float WdU[2], AWdU[2], DdU[2], ADdU[2];
    
    Vector2 diff = ray.Origin - box.Center;
    
    WdU[0] = ray.Direction.Dot(box.Axis[0]);
    AWdU[0] = Math::Abs(WdU[0]);
    DdU[0] = diff.Dot(box.Axis[0]);
    ADdU[0] = Math::Abs(DdU[0]);
    if (ADdU[0] > box.Extent[0] && DdU[0] * WdU[0] >= 0.0f)
    {
        return false;
    }
    
    WdU[1] = ray.Direction.Dot(box.Axis[1]);
    AWdU[1] = Math::Abs(WdU[1]);
    DdU[1] = diff.Dot(box.Axis[1]);
    ADdU[1] = Math::Abs(DdU[1]);
    if (ADdU[1] > box.Extent[1] && DdU[1] * WdU[1] >= 0.0f)
    {
        return false;
    }
    
    Vector2 perp = ray.Direction.GetPerpendicular();
    float LHS = Math::Abs(perp.Dot(diff));
    float part0 = Math::Abs(perp.Dot(box.Axis[0]));
    float part1 = Math::Abs(perp.Dot(box.Axis[1]));
    float RHS = box.Extent[0] * part0 + box.Extent[1] * part1;
    return LHS <= RHS;
}

// *****************************************************************
/// @brief
///     Finds the first point of intersection between a ray and an oriented box
/// @param[in] ray
///     The ray component to intersect
/// @param[in] box
///     The oriented box to intersect
/// @param[out] distance
///     The distance from the origin of the ray to the point of intersection.  This value is undefined if the method returns false.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Find(const Ray2& ray, const Box2& box, float& distance)
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


// *****************************************************************
/// @brief
///     Tests if two circles intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Circle2& circle1, const Circle2& circle2)
{
	// Get the distance (squared) between the two circle centers 
    float distanceSquared = (circle1.Center - circle2.Center).LengthSquared();
    
    // Get the total radii of the circles
    float totalRadii = circle1.Radius + circle2.Radius;
    
    // If the distance between the centers is less than the total radii, then the circles intersect
    return distanceSquared <= totalRadii * totalRadii;
}

// *****************************************************************
/// @brief
///     Tests if a circle and a rectangle intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Circle2& circle, const Rectangle2& rect)
{
    // Get the distance from the center of the circle to the rectangle
    float distance = Distance2::PointToRectangle(circle.Center, rect);
    
    // Compare the distance against the circle radius
    return distance <= circle.Radius;
}


// *****************************************************************
/// @brief
///     Tests if a circle and an oriented box intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Circle2& circle, const Box2& box)
{
    // Get the distance from the center of the circle to the box
    float distance = Distance2::PointToBox(circle.Center, box);
    
    // Compare the distance against the circle radius
    return distance <= circle.Radius;
}



// *****************************************************************
/// @brief
///     Tests if two rectangles intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Rectangle2& rect1, const Rectangle2& rect2)
{
    // Test if the rectangles intersect
    return rect1.Position.X <= (rect2.Position.X + rect2.Size.X)
        && rect1.Position.X + rect1.Size.X >= rect2.Position.X
        && rect1.Position.Y <= (rect2.Position.Y + rect2.Size.Y)
        && rect1.Position.Y + rect1.Size.Y >= rect2.Position.Y
        ;
}

// *****************************************************************
/// @brief
///     Tests if a rectangle and an oriented box intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Rectangle2& rect, const Box2& box)
{
    // Test the rectangle (as a box) against the box
    return Test(rect.GetAsBox(), box);
}

// *****************************************************************
/// @brief
///     Tests if two oriented boxes intersect.
/// @return
///     true if the primitives intersect, otherwise false.
// *****************************************************************
bool Intersection2::Test(const Box2& box1, const Box2& box2)
{
    // Get the difference vector between the box centers
    Vector2 diff = box1.Center - box2.Center;
    
    // Axis C0+t*A0
    float absA0dotB0 = Math::Abs(box2.Axis[0].Dot(box1.Axis[0]));
    float absA0dotB1 = Math::Abs(box2.Axis[0].Dot(box1.Axis[1]));
    float absA0dotDiff = Math::Abs(box2.Axis[0].Dot(diff));
    float sum = box2.Extent[0] + box1.Extent[0]*absA0dotB0 + box1.Extent[1]*absA0dotB1;
    if (absA0dotDiff > sum)
    {
        return false;
    }
    
    // Axis C0+t*A1
    float absA1dotB0 = Math::Abs(box2.Axis[1].Dot(box1.Axis[0]));
    float absA1dotB1 = Math::Abs(box2.Axis[1].Dot(box1.Axis[1]));
    float absA1dotDiff = Math::Abs(box2.Axis[1].Dot(diff));
    sum = box2.Extent[1] + box1.Extent[0]*absA1dotB0 + box1.Extent[1]*absA1dotB1;
    if (absA1dotDiff > sum)
    {
        return false;
    }
    
    // Axis C0+t*B0
    float absB0dotDiff = Math::Abs(box1.Axis[0].Dot(diff));
    sum = box1.Extent[0] + box2.Extent[0]*absA0dotB0 + box2.Extent[1]*absA1dotB0;
    if (absB0dotDiff > sum)
    {
        return false;
    }
    
    // Axis C0+t*B1
    float absB1dotDiff = Math::Abs(box1.Axis[1].Dot(diff));
    sum = box1.Extent[1] + box2.Extent[0]*absA0dotB1 + box2.Extent[1]*absA1dotB1;
    if (absB1dotDiff > sum)
    {
        return false;
    }
    
    return true;
}

// *****************************************************************
/// @brief
///     Utility method for intersecting two linear primitive
/// @param[in] origin1
///     A point of origin for the first linear primitive
/// @param[in] direction1
///     The vector direction for the first linear primitive
/// @param[in] origin2
///     A point of origin for the second linear primitive
/// @param[in] direction2
///     The vector direction for the second linear primitive
/// @param[out] t[2]
///     The distance from the origin of each linear primitive to the point of intersection.
/// @return
///     The type of intersection (or non-intersection) for the two primitives
// *****************************************************************
IntersectionType::Enum  Intersection2::Intersect_Line_Line(
    const Vector2& origin1, const Vector2& direction1,
    const Vector2& origin2, const Vector2& direction2,
    float t[2]
    )
{
    // This method checks for an intersection of two lines.  The lines are in the following form:
    //   Line = P + D*t
    //
    // The results returned in the 't' out-param are t values along each line (with respect to the direction vector) 
    // The return value will be:  None, Point, or Line (if the lines are colinear)
    
    
    // Get the difference vector between the 2 origins of the lines
    Vector2 difference = origin2 - origin1;
    
    // Get the dot product of line direction 1 & the perpendicular of line direction 2
    float dir1DotPerpDir2 = direction1.Dot(direction2.GetPerpendicular());
    
    // If the dot product is non-zero, the lines intersect at a point
    if (Math::Abs(dir1DotPerpDir2) > Math::ZERO_TOLERANCE)
    {
        // Calculate the T values of the intersection along each line
        float invDir1DotPerpDir2 = 1.0f / dir1DotPerpDir2;
        float diffDotPerpD1 = difference.Dot(direction1.GetPerpendicular());
        float diffDotPerpD2 = difference.Dot(direction2.GetPerpendicular());
        t[0] = diffDotPerpD2 * invDir1DotPerpDir2;
        t[1] = diffDotPerpD1 * invDir1DotPerpDir2;

        return IntersectionType::Point;
    }
    
    // Normalize the difference vector
    difference.Normalize();
    
    // Check if the linear components are co-linear
    float normDiffDotPerpDir2 = difference.Dot(direction2.GetPerpendicular());
    if (Math::Abs(normDiffDotPerpDir2) <= Math::ZERO_TOLERANCE)
    {
        // Linear components are colinear.
        return IntersectionType::Line;
    }
    
    // Linear components are parallel and not touching
    return IntersectionType::None;
}

// *****************************************************************
/// @brief
///     Utility method for intersecting a linear primitive and a circle
/// @param[in] origin
///     A point of origin for the linear primitive
/// @param[in] direction
///     The vector direction for the linear primitive
/// @param[in] center
///     The center of the circle
/// @param[in] radius
///     The radius of the circle
/// @param[out] t[2]
///     The result in t depends on the following intersection types:
///     @li None - t is undefined
///     @li Point - t[0] is the distance from the origin of the ray to the intersection point.
///     @li LineSegment - t[0] & t[1] are the distances to the first and last points of intersection.
/// @return
///     The type of intersection (or non-intersection) for the two primitives
// *****************************************************************
IntersectionType::Enum  Intersection2::Intersect_Line_Circle(
    const Vector2& origin, const Vector2& direction,
    const Vector2& center, float radius,
    float t[2]
    )
{
    // This method checks for an intersection of a line & a circle.
    //   Line = P + D*t
    //
    // The results returned in the 't' out-param are t values along the casting ray.  The number of t values
    //   is directly corrolated to the intersection type returned.  
    // The return value will be:  None, Point, or LineSegment (if the line has 2 intersection points)
    
    
    // Get the difference vector from the linear origin to the circle center
    Vector2 difference = origin - center;
    
    // Project the linear direction vector onto the difference vector, and compare the length of the projection to the circle radius
    float a0 = difference.LengthSquared() - radius * radius;
    float a1 = direction.Dot(difference);
    float discriminant = a1 * a1 - a0;
    if (discriminant > Math::ZERO_TOLERANCE)
    {
        // The ray goes through the circle, so there are 2 intersection points
        discriminant = Math::Sqrt(discriminant);
        t[0] = -a1 - discriminant;
        t[1] = -a1 + discriminant;
        return IntersectionType::LineSegment;
    }
    else if (discriminant < -Math::ZERO_TOLERANCE)
    {
        // They ray does not touch the circle
        return IntersectionType::None;
    }
    
    // The ray touches the circle at a tangent edge
    t[0] = -a1;
    return IntersectionType::Point;
}

// *****************************************************************
/// @brief
///     Utility method for intersecting a linear primitive and an oriented box
/// @param startT
///     The starting t-value along the linear component to start testing
/// @param endT
///     The ending t-value along the linear component to stop testing
/// @param[in] origin
///     A point of origin for the linear primitive
/// @param[in] direction
///     The vector direction for the linear primitive
/// @param[in] box
///     The oriented box
/// @param[out] t[2]
///     The result in t depends on the following intersection types:
///     @li None - t is undefined
///     @li Point - t[0] is the distance from the origin of the ray to the intersection point.
///     @li LineSegment - t[0] & t[1] are the distances to the first and last points of intersection.
/// @return
///     The type of intersection (or non-intersection) for the two primitives
// *****************************************************************
IntersectionType::Enum  Intersection2::Intersect_Line_Box(
    float startT, float endT,
    const Vector2& origin, const Vector2& direction,
    const Box2& box,
    float t[2]
    )
{
    // Get the ray in box coordinate space
    Vector2 diff = origin - box.Center;
    Vector2 boxOrigin(
        diff.Dot(box.Axis[0]),
        diff.Dot(box.Axis[1])
        );
    Vector2 boxDirection(
        direction.Dot(box.Axis[0]),
        direction.Dot(box.Axis[1])
        );
    
    // Clip the ray against the 4 box edges
    bool notAllClipped =
        Clip_Box_Line(+boxDirection.X, -boxOrigin.X - box.Extent[0], startT, endT) &&
        Clip_Box_Line(-boxDirection.X, +boxOrigin.X - box.Extent[0], startT, endT) &&
        Clip_Box_Line(+boxDirection.Y, -boxOrigin.Y - box.Extent[1], startT, endT) &&
        Clip_Box_Line(-boxDirection.Y, +boxOrigin.Y - box.Extent[1], startT, endT);
    
    // Did any of the box edges clip our line?
    if (notAllClipped)
    {
        t[0] = startT;
        t[1] = endT;
        
        // Where there 2 clip points?
        if (endT > startT)
            return IntersectionType::LineSegment;
        return IntersectionType::Point;
    }

    // No clipping, so no intersections
    return IntersectionType::None;
}

// *****************************************************************
/// @brief
///     Internal utility method for quickly clipping a line along a box edge
// *****************************************************************
bool Intersection2::Clip_Box_Line(float denominator, float numerator, float& t0, float& t1)
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

