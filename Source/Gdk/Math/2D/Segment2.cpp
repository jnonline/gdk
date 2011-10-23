/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Segment2.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Segment2::Segment2()
{
}

// *****************************************************************
/// @brief
///     Constructs a new segment
// *****************************************************************
Segment2::Segment2(const Vector2& center, const Vector2& direction, float extent)
{
	Center = center;
	Direction = direction;
    Extent = extent;
}

// *****************************************************************
/// @brief
///     Constructs a new segment from the given start and end points
// *****************************************************************
Segment2::Segment2(const Vector2& start, const Vector2& end)
{
    Center = (start + end) * 0.5f;
    Direction = (end - start);
    Extent = Direction.Normalize() * 0.5f;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Segment2::Segment2(const Segment2& input)
{
	Center = input.Center;
    Direction = input.Direction;
    Extent = input.Extent;
}

// *****************************************************************
/// @brief
///     Gets the length of the segment
// *****************************************************************
float Segment2::GetLength() const
{
	return this->Extent * 2.0f;
}

// *****************************************************************
/// @brief
///     Gets the first endpoint of the segment
// *****************************************************************
Vector2 Segment2::GetEndpoint1() const
{
	return (Center - Direction * Extent);
}

// *****************************************************************
/// @brief
///     Gets the second endpoint of the segment
// *****************************************************************
Vector2 Segment2::GetEndpoint2() const
{
	return (Center + Direction * Extent);
}

// *****************************************************************
/// @brief
///     Gets a point on the line segment using a parametric input
/// @param tAngle
///     Unit t-value (0.0 to 1.0) along the line segment.  (0=start, 1=end)
// *****************************************************************
Vector2 Segment2::GetParametricPoint(float t) const
{
    return Center + (t * 2.0f - 1.0f) * Extent * Direction;
}

