/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "LineSegment2.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
LineSegment2::LineSegment2()
{
}

// *****************************************************************
/// @brief
///     Constructs a new line segment from the given start and end points
// *****************************************************************
LineSegment2::LineSegment2(const Vector2& start, const Vector2& end)
{
	Start = start;
	End = end;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
LineSegment2::LineSegment2(const LineSegment2& input)
{
	Start = input.Start;
	End = input.End;
}

// *****************************************************************
/// @brief
///     Gets a point on the line segment using a parametric input
/// @param tAngle
///     Unit t-value (0.0 to 1.0) along the line segment.  (0=start, 1=end)
// *****************************************************************
Vector2 LineSegment2::GetParametricPoint(float t)
{
	return Start + t * (End - Start);
}

