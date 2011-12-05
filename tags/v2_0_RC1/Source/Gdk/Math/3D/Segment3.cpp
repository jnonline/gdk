/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Segment3.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Segment3::Segment3()
{
}

// *****************************************************************
/// @brief
///     Constructs a new segment
// *****************************************************************
Segment3::Segment3(const Vector3& center, const Vector3& direction, float extent)
{
	Center = center;
	Direction = direction;
    Extent = extent;
}

// *****************************************************************
/// @brief
///     Constructs a new segment from the given start and end points
// *****************************************************************
Segment3::Segment3(const Vector3& start, const Vector3& end)
{
	Center = (start + end) * 0.5f;
    Direction = (end - start);
    Extent = Direction.Normalize() * 0.5f;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Segment3::Segment3(const Segment3& input)
{
	Center = input.Center;
    Direction = input.Direction;
    Extent = input.Extent;
}

// *****************************************************************
/// @brief
///     Gets the length of the segment
// *****************************************************************
float Segment3::GetLength() const
{
	return this->Extent * 2.0f;
}

// *****************************************************************
/// @brief
///     Gets the first endpoint of the segment
// *****************************************************************
Vector3 Segment3::GetEndpoint1() const
{
	return (Center - Direction * Extent);
}

// *****************************************************************
/// @brief
///     Gets the second endpoint of the segment
// *****************************************************************
Vector3 Segment3::GetEndpoint2() const
{
	return (Center + Direction * Extent);
}

// *****************************************************************
/// @brief
///     Gets a point on the line segment using a parametric input
/// @param tAngle
///     Unit t-value (0.0 to 1.0) along the line segment.  (0=start, 1=end)
// *****************************************************************
Vector3 Segment3::GetParametricPoint(float t) const
{
    return Center + (t * 2.0f - 1.0f) * Extent * Direction;
}

