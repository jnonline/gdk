/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Capsule3.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Capsule3::Capsule3()
{
}

// *****************************************************************
/// @brief
///     Constructs a new sphere from a center point and a radius
// *****************************************************************
Capsule3::Capsule3(const Segment3& segment, float radius)
    : Segment(segment), Radius(radius)
{
}

// *****************************************************************
/// @brief
///     Constructs a new sphere from a center point and a radius
// *****************************************************************
Capsule3::Capsule3(const Vector3& endpoint1, const Vector3& endpoint2, float radius)
    : Segment(endpoint1, endpoint2), Radius(radius)
{
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Capsule3::Capsule3(const Capsule3& input)
{
	Segment = input.Segment;
	Radius = input.Radius;
}

// *****************************************************************
/// @brief
///     Checks if the capsule contains a given point
// *****************************************************************
bool Capsule3::Contains(float x, float y, float z) const
{
    return Contains(Vector3(x,y,z));
}

// *****************************************************************
/// @brief
///     Checks if the capsule contains a given point
// *****************************************************************
bool Capsule3::Contains(const Vector3& point) const
{
    // Get the distance from the point to the capsule's segment
    float distance = Distance3::PointToSegment(point, Segment);
    
    // Check if the distance is within the capsule radius
    return distance <= Radius;
}

// *****************************************************************
/// @brief
///     Gets a perfect fit sphere that encapsulates this capsule
// *****************************************************************
Sphere3 Capsule3::GetBoundingSphere() const
{
    return Sphere3(
        Segment.Center,
        Radius + Segment.Extent * 0.5f
        );
}

// *****************************************************************
/// @brief
///     Gets a perfect fit oriented box that encapsulates this capsule
// *****************************************************************
Box3 Capsule3::GetBoundingBox() const
{
    // Get an orthonormal basis for the capsule's segment
    Vector3 u,v,w;
    Segment.Direction.GenerateOrthonormalBasis(u,v,w);
    
    // Create a box, centered on the capsule
    return Box3(
        Segment.Center,
        u, v, w,
        Radius, Radius, Radius + Segment.Extent
        );
}

