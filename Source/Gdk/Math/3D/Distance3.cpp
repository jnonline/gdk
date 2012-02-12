/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Distance3.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Calculates the distance between two points
// *****************************************************************
float Distance3::PointToPoint(const Vector3& point1, const Vector3& point2)
{
    return Vector3::Distance(point1, point2);
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a ray
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a segment
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::PointToSegment(const Vector3& point, const Segment3& segment)
{
    // Get the vector from the segment center to the point
    Vector3 diff = point - segment.Center;
    Vector3 closest;
    
    // Project the segment direction on to the first vector
    float projection = segment.Direction.Dot(diff);
    
    // Is the projection before the segment start point?
    if(projection < -segment.Extent)
    {
        // The closest point is the segment start
        closest = segment.Center - segment.Extent * segment.Direction;
    }
    // Is the projection after the segment end?
    else if(projection > segment.Extent)
    {
        // The closest point is the segment end
        closest = segment.Center + segment.Extent * segment.Direction;
    }
    else
    {
        // The closest point is the projection onto the line segment vector
        closest = segment.Center + projection * segment.Direction;
    }
    
    // Return the distance from the point to the closest projection point
    return (closest - point).Length();
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a plane
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::PointToPlane(const Vector3& point, const Plane3& plane)
{
    // Get the distance from the point to the plane
    return Math::Abs(plane.Normal.Dot(point) - plane.D);
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and an oriented box
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a sphere
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::PointToSphere(const Vector3& point, const Sphere3& sphere)
{
    // Get the distance from the point to the circle center
    float diff = (point - sphere.Center).Length();
    
    // Subtract the radius  (and cap the distance as a minimum of 0)
    float distance = diff - sphere.Radius;
    return distance >= 0.0f ? distance : 0.0f;
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a point and a capsule
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::PointToCapsule(const Vector3& point, const Capsule3& capsule)
{
    // Get the distance from the point to the capsule's segment
    float distToSegment = PointToSegment(point, capsule.Segment);
    
    // Subtract the radius of the capsule (and cap the distance at a minimum of 0.0)
    float distance = distToSegment - capsule.Radius;
    return distance >= 0.0f ? distance : 0.0f;
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a ray and a segment.
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::RayToSegment(const Ray3& ray, const Segment3& segment)
{
    // Project the vector from the ray origin to the line segment, onto the ray itself
    Vector3 diff = ray.Origin - segment.Center;
    float a01 = -ray.Direction.Dot(segment.Direction);
    float b0 = diff.Dot(ray.Direction);
    float b1 = -diff.Dot(segment.Direction);
    float c = diff.LengthSquared();
    float det = Math::Abs(1.0f - a01 * a01);
    float s0, s1, sqrDist, extDet;
    
    // Are the ray and line segment parallel?
    if (det < Math::ZERO_TOLERANCE)
    {
        if (a01 > 0.0f)
        {
            // Opposite direction vectors.
            s1 = -segment.Extent;
        }
        else
        {
            // Same direction vectors.
            s1 = segment.Extent;
        }
        
        s0 = -(a01 * s1 + b0);
        if (s0 > 0.0f)
        {
            sqrDist = -s0 * s0 + s1 * (s1 + 2.0f * b1) + c;
        }
        else
        {
            s0 = 0.0f;
            sqrDist = s1 * (s1 + 2.0f * b1) + c;
        }
    }
    else
    {
        s0 = a01 * b1 - b0;
        s1 = a01 * b0 - b1;
        extDet = segment.Extent * det;
        
        if (s0 >= 0.0f)
        {
            if (s1 >= -extDet)
            {
                if (s1 <= extDet)
                {
                    float invDet = 1.0f / det;
                    s0 *= invDet;
                    s1 *= invDet;
                    sqrDist = s0 * (s0 + a01 * s1 + 2.0f * b0) +
                              s1 * (a01 * s0 + s1 + 2.0f * b1) + c;
                }
                else 
                {
                    s1 = segment.Extent;
                    s0 = -(a01 * s1 + b0);
                    if (s0 > 0.0f)
                    {
                        sqrDist = -s0 * s0 + s1 * (s1 + 2.0f * b1) + c;
                    }
                    else
                    {
                        s0 = 0.0f;
                        sqrDist = s1*(s1 + 2.0f * b1) + c;
                    }
                }
            }
            else
            {
                s1 = -segment.Extent;
                s0 = -(a01 * s1 + b0);
                if (s0 > 0.0f)
                {
                    sqrDist = -s0*s0 + s1*(s1 + 2.0f * b1) + c;
                }
                else
                {
                    s0 = 0.0f;
                    sqrDist = s1 * (s1 + 2.0f * b1) + c;
                }
            }
        }
        else
        {
            if (s1 <= -extDet)
            {
                s0 = -(-a01 * segment.Extent + b0);
                if (s0 > 0.0f)
                {
                    s1 = -segment.Extent;
                    sqrDist = -s0 * s0 + s1*(s1 + 2.0f * b1) + c;
                }
                else
                {
                    s0 = 0.0f;
                    s1 = -b1;
                    if (s1 < -segment.Extent)
                    {
                        s1 = -segment.Extent;
                    }
                    else if (s1 > segment.Extent)
                    {
                        s1 = segment.Extent;
                    }
                    sqrDist = s1 * (s1 + 2.0f * b1) + c;
                }
            }
            else if (s1 <= extDet)
            {
                s0 = 0.0f;
                s1 = -b1;
                if (s1 < -segment.Extent)
                {
                    s1 = -segment.Extent;
                }
                else if (s1 > segment.Extent)
                {
                    s1 = segment.Extent;
                }
                sqrDist = s1 * (s1 + 2.0f * b1) + c;
            }
            else
            {
                s0 = -(a01 * segment.Extent + b0);
                if (s0 > 0.0f)
                {
                    s1 = segment.Extent;
                    sqrDist = -s0 * s0 + s1 * (s1 + 2.0f * b1) + c;
                }
                else
                {
                    s0 = 0.0f;
                    s1 = -b1;
                    if (s1 < -segment.Extent)
                    {
                        s1 = -segment.Extent;
                    }
                    else if (s1 > segment.Extent)
                    {
                        s1 = segment.Extent;
                    }
                    sqrDist = s1 * (s1 + 2.0f * b1) + c;
                }
            }
        }
    }
        
    /*
    Closest Point On Ray          = ray.Origin + ray.Direction * s0
    Closest Point On Line Segment = segmentCenter + segmentDir * s1
    */
    
    // Dont allow negative distances
    if (sqrDist < 0.0f)
        sqrDist = 0.0f;
    
    return Math::Sqrt(sqrDist);
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a ray and a sphere.
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::RayToSphere(const Gdk::Ray3 &ray, const Gdk::Sphere3 &sphere)
{
    // Get the distance from the ray to the sphere center
    float dist = PointToRay(sphere.Center, ray);
    
    // Subtract the sphere radius
    dist -= sphere.Radius;
    return dist <= 0.0f ? 0.0f : dist;
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between a ray and a capsule.
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::RayToCapsule(const Ray3& ray, const Capsule3& capsule)
{
    // Get the distance from the ray to the segment of the capsule
    float dist = RayToSegment(ray, capsule.Segment);
    
    // Subtract the capsule radius
    dist -= capsule.Radius;
    return dist <= 0.0f ? 0.0f : dist;
}

// *****************************************************************
/// @brief
///     Calculates the shortest distance between two segments
/// @return
///     The shortest distance between the two primitives
// *****************************************************************
float Distance3::SegmentToSegment(const Gdk::Segment3 &segment1, const Gdk::Segment3 &segment2)
{
    // Adapted from GeometricTools.com
    
    Vector3 diff = segment1.Center - segment2.Center;
    float a01 = -segment1.Direction.Dot(segment2.Direction);
    float b0 = diff.Dot(segment1.Direction);
    float b1 = -diff.Dot(segment2.Direction);
    float c = diff.LengthSquared();
    float det = Math::Abs(1 - a01*a01);
    float s0, s1, sqrDist, extDet0, extDet1, tmpS0, tmpS1;

    if (det >= Math::ZERO_TOLERANCE)
    {
        // Segments are not parallel.
        s0 = a01*b1 - b0;
        s1 = a01*b0 - b1;
        extDet0 = segment1.Extent*det;
        extDet1 = segment2.Extent*det;
        
        if (s0 >= -extDet0)
        {
            if (s0 <= extDet0)
            {
                if (s1 >= -extDet1)
                {
                    if (s1 <= extDet1)  // region 0 (interior)
                    {
                        // Minimum at interior points of segments.
                        float invDet = (1)/det;
                        s0 *= invDet;
                        s1 *= invDet;
                        sqrDist = s0*(s0 + a01*s1 + (2)*b0) +
                        s1*(a01*s0 + s1 + (2)*b1) + c;
                    }
                    else  // region 3 (side)
                    {
                        s1 = segment2.Extent;
                        tmpS0 = -(a01*s1 + b0);
                        if (tmpS0 < -segment1.Extent)
                        {
                            s0 = -segment1.Extent;
                            sqrDist = s0*(s0 - (2)*tmpS0) +
                            s1*(s1 + (2)*b1) + c;
                        }
                        else if (tmpS0 <= segment1.Extent)
                        {
                            s0 = tmpS0;
                            sqrDist = -s0*s0 + s1*(s1 + (2)*b1) + c;
                        }
                        else
                        {
                            s0 = segment1.Extent;
                            sqrDist = s0*(s0 - (2)*tmpS0) +
                            s1*(s1 + (2)*b1) + c;
                        }
                    }
                }
                else  // region 7 (side)
                {
                    s1 = -segment2.Extent;
                    tmpS0 = -(a01*s1 + b0);
                    if (tmpS0 < -segment1.Extent)
                    {
                        s0 = -segment1.Extent;
                        sqrDist = s0*(s0 - (2)*tmpS0) +
                        s1*(s1 + (2)*b1) + c;
                    }
                    else if (tmpS0 <= segment1.Extent)
                    {
                        s0 = tmpS0;
                        sqrDist = -s0*s0 + s1*(s1 + (2)*b1) + c;
                    }
                    else
                    {
                        s0 = segment1.Extent;
                        sqrDist = s0*(s0 - (2)*tmpS0) +
                        s1*(s1 + (2)*b1) + c;
                    }
                }
            }
            else
            {
                if (s1 >= -extDet1)
                {
                    if (s1 <= extDet1)  // region 1 (side)
                    {
                        s0 = segment1.Extent;
                        tmpS1 = -(a01*s0 + b1);
                        if (tmpS1 < -segment2.Extent)
                        {
                            s1 = -segment2.Extent;
                            sqrDist = s1*(s1 - (2)*tmpS1) +
                            s0*(s0 + (2)*b0) + c;
                        }
                        else if (tmpS1 <= segment2.Extent)
                        {
                            s1 = tmpS1;
                            sqrDist = -s1*s1 + s0*(s0 + (2)*b0) + c;
                        }
                        else
                        {
                            s1 = segment2.Extent;
                            sqrDist = s1*(s1 - (2)*tmpS1) +
                            s0*(s0 + (2)*b0) + c;
                        }
                    }
                    else  // region 2 (corner)
                    {
                        s1 = segment2.Extent;
                        tmpS0 = -(a01*s1 + b0);
                        if (tmpS0 < -segment1.Extent)
                        {
                            s0 = -segment1.Extent;
                            sqrDist = s0*(s0 - (2)*tmpS0) +
                            s1*(s1 + (2)*b1) + c;
                        }
                        else if (tmpS0 <= segment1.Extent)
                        {
                            s0 = tmpS0;
                            sqrDist = -s0*s0 + s1*(s1 + (2)*b1) + c;
                        }
                        else
                        {
                            s0 = segment1.Extent;
                            tmpS1 = -(a01*s0 + b1);
                            if (tmpS1 < -segment2.Extent)
                            {
                                s1 = -segment2.Extent;
                                sqrDist = s1*(s1 - (2)*tmpS1) +
                                s0*(s0 + (2)*b0) + c;
                            }
                            else if (tmpS1 <= segment2.Extent)
                            {
                                s1 = tmpS1;
                                sqrDist = -s1*s1 + s0*(s0 + (2)*b0) + c;
                            }
                            else
                            {
                                s1 = segment2.Extent;
                                sqrDist = s1*(s1 - (2)*tmpS1) +
                                s0*(s0 + (2)*b0) + c;
                            }
                        }
                    }
                }
                else  // region 8 (corner)
                {
                    s1 = -segment2.Extent;
                    tmpS0 = -(a01*s1 + b0);
                    if (tmpS0 < -segment1.Extent)
                    {
                        s0 = -segment1.Extent;
                        sqrDist = s0*(s0 - (2)*tmpS0) +
                        s1*(s1 + (2)*b1) + c;
                    }
                    else if (tmpS0 <= segment1.Extent)
                    {
                        s0 = tmpS0;
                        sqrDist = -s0*s0 + s1*(s1 + (2)*b1) + c;
                    }
                    else
                    {
                        s0 = segment1.Extent;
                        tmpS1 = -(a01*s0 + b1);
                        if (tmpS1 > segment2.Extent)
                        {
                            s1 = segment2.Extent;
                            sqrDist = s1*(s1 - (2)*tmpS1) +
                            s0*(s0 + (2)*b0) + c;
                        }
                        else if (tmpS1 >= -segment2.Extent)
                        {
                            s1 = tmpS1;
                            sqrDist = -s1*s1 + s0*(s0 + (2)*b0) + c;
                        }
                        else
                        {
                            s1 = -segment2.Extent;
                            sqrDist = s1*(s1 - (2)*tmpS1) +
                            s0*(s0 + (2)*b0) + c;
                        }
                    }
                }
            }
        }
        else 
        {
            if (s1 >= -extDet1)
            {
                if (s1 <= extDet1)  // region 5 (side)
                {
                    s0 = -segment1.Extent;
                    tmpS1 = -(a01*s0 + b1);
                    if (tmpS1 < -segment2.Extent)
                    {
                        s1 = -segment2.Extent;
                        sqrDist = s1*(s1 - (2)*tmpS1) +
                        s0*(s0 + (2)*b0) + c;
                    }
                    else if (tmpS1 <= segment2.Extent)
                    {
                        s1 = tmpS1;
                        sqrDist = -s1*s1 + s0*(s0 + (2)*b0) + c;
                    }
                    else
                    {
                        s1 = segment2.Extent;
                        sqrDist = s1*(s1 - (2)*tmpS1) +
                        s0*(s0 + (2)*b0) + c;
                    }
                }
                else  // region 4 (corner)
                {
                    s1 = segment2.Extent;
                    tmpS0 = -(a01*s1 + b0);
                    if (tmpS0 > segment1.Extent)
                    {
                        s0 = segment1.Extent;
                        sqrDist = s0*(s0 - (2)*tmpS0) +
                        s1*(s1 + (2)*b1) + c;
                    }
                    else if (tmpS0 >= -segment1.Extent)
                    {
                        s0 = tmpS0;
                        sqrDist = -s0*s0 + s1*(s1 + (2)*b1) + c;
                    }
                    else
                    {
                        s0 = -segment1.Extent;
                        tmpS1 = -(a01*s0 + b1);
                        if (tmpS1 < -segment2.Extent)
                        {
                            s1 = -segment2.Extent;
                            sqrDist = s1*(s1 - (2)*tmpS1) +
                            s0*(s0 + (2)*b0) + c;
                        }
                        else if (tmpS1 <= segment2.Extent)
                        {
                            s1 = tmpS1;
                            sqrDist = -s1*s1 + s0*(s0 + (2)*b0) + c;
                        }
                        else
                        {
                            s1 = segment2.Extent;
                            sqrDist = s1*(s1 - (2)*tmpS1) +
                            s0*(s0 + (2)*b0) + c;
                        }
                    }
                }
            }
            else   // region 6 (corner)
            {
                s1 = -segment2.Extent;
                tmpS0 = -(a01*s1 + b0);
                if (tmpS0 > segment1.Extent)
                {
                    s0 = segment1.Extent;
                    sqrDist = s0*(s0 - (2)*tmpS0) +
                    s1*(s1 + (2)*b1) + c;
                }
                else if (tmpS0 >= -segment1.Extent)
                {
                    s0 = tmpS0;
                    sqrDist = -s0*s0 + s1*(s1 + (2)*b1) + c;
                }
                else
                {
                    s0 = -segment1.Extent;
                    tmpS1 = -(a01*s0 + b1);
                    if (tmpS1 < -segment2.Extent)
                    {
                        s1 = -segment2.Extent;
                        sqrDist = s1*(s1 - (2)*tmpS1) +
                        s0*(s0 + (2)*b0) + c;
                    }
                    else if (tmpS1 <= segment2.Extent)
                    {
                        s1 = tmpS1;
                        sqrDist = -s1*s1 + s0*(s0 + (2)*b0) + c;
                    }
                    else
                    {
                        s1 = segment2.Extent;
                        sqrDist = s1*(s1 - (2)*tmpS1) +
                        s0*(s0 + (2)*b0) + c;
                    }
                }
            }
        }
    }
    else
    {
        // The segments are parallel.  The average b0 term is designed to
        // ensure symmetry of the function.  That is, dist(seg0,seg1) and
        // dist(seg1,seg0) should produce the same number.
        float e0pe1 = segment1.Extent + segment2.Extent;
        float sign = (float)(a01 > 0 ? -1 : 1);
        float b0Avr = (0.5f)*(b0 - sign*b1);
        float lambda = -b0Avr;
        if (lambda < -e0pe1)
        {
            lambda = -e0pe1;
        }
        else if (lambda > e0pe1)
        {
            lambda = e0pe1;
        }
        
        s1 = -sign*lambda*segment2.Extent/e0pe1;
        s0 = lambda + sign*s1;
        sqrDist = lambda*(lambda + (2)*b0Avr) + c;
    }

    /*
    Closest Point on Segment 1 = segment1.Center + s0 * segment1.Direction;
    Closest Point on Segment 1 = segment1.Center + s0 * segment1.Direction;
    */
    
    // Account for numerical round-off errors.
    if (sqrDist < 0.0f)
        sqrDist = 0.0f;
    return Math::Sqrt(sqrDist);
}
