/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Rectangle2.h"

using namespace Gdk;

// instantiations of static constants
const Rectangle2 Rectangle2::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Rectangle2 Rectangle2::UNIT(0.0f, 0.0f, 1.0f, 1.0f);

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Rectangle2::Rectangle2()
{
}

// *****************************************************************
/// @brief
///     Constructs a new rectangle from a given position and size
// *****************************************************************
Rectangle2::Rectangle2(float x, float y, float width, float height)
{
	Position.X = x;
	Position.Y = y;
	Size.X = width;
	Size.Y = height;
}

// *****************************************************************
/// @brief
///     Constructs a new rectangle from a given position and size
// *****************************************************************
Rectangle2::Rectangle2(const Vector2& position, const Vector2& size)
{
	Position = position;
	Size = size;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Rectangle2::Rectangle2(const Rectangle2& input)
{
	Position = input.Position;
	Size = input.Size;
}

// *****************************************************************
/// @brief
///     Calculates the center point of the rectangle
// *****************************************************************
Vector2 Rectangle2::GetCenter() const
{
    return Position + Size * 0.5f;
}

// *****************************************************************
/// @brief
///     Calculates the extents (half-sizes) of the rectangle
// *****************************************************************
Vector2 Rectangle2::GetExtents() const
{
    return Size * 0.5f;
}

// *****************************************************************
/// @brief
///     Gets a point within the area of the rectangle using parametric inputs
/// @param tAngle
///     Unit t-value (0.0 to 1.0) along the rectangle width
/// @param tRadius
///     Unit t-value (0.0 to 1.0) along the rectangle height
// *****************************************************************
Vector2 Rectangle2::GetParametricPointInArea(float tx, float ty) const
{
	// Return a point in the rectangle using the given parametric coordinates within rectangle space
	return Vector2(
		this->Position.X + this->Size.X * tx, 
		this->Position.Y + this->Size.Y * ty
		);
}

// *****************************************************************
/// @brief
///     Gets a point on the perimeter of the rectangle using parametric input
/// @param t
///     Unit t-value (0.0 to 1.0) along the perimeter
// *****************************************************************
Vector2 Rectangle2::GetParametricPointOnPerimeter(float t) const
{
	// The rectangle perimeter starts at the top left, and goes clockwise

	// Is the rectangle flat in the X plane?
	if(this->Size.X <= Math::ZERO_TOLERANCE)
	{
		// Is the rectangle flat in both the X & Y planes?
		if(this->Size.Y <= Math::ZERO_TOLERANCE)
		{
			return this->Position;
		}

		// Return a point along the Y-axis line
		return Vector2(this->Position.X, this->Position.Y + this->Size.Y * t);
	}
	// Is the rectangle flat in the Y plane?
	else if(this->Size.Y <= Math::ZERO_TOLERANCE)
	{
		// Return a point along the X-axis line
		return Vector2(this->Position.X + this->Size.X * t, this->Position.Y);
	}

	// The rectangle is not flat, so we need to determine which side of the rectangle this t is on
	float recipPerimeterLength = 0.5f / (this->Size.X + this->Size.Y);

	// Top edge?
	float widthInT = this->Size.X * recipPerimeterLength;
	if(t < widthInT)
		return Vector2(this->Position.X + this->Size.X * (t / widthInT), this->Position.Y);

	// Right edge?
	float heightInT = this->Size.Y * recipPerimeterLength;
	if(t < 0.5f)
		return Vector2(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y * ((t-widthInT) / heightInT));

	// Bottom edge?
	if(t < 0.5f + widthInT)
		return Vector2(this->Position.X + this->Size.X * ((0.5f + widthInT - t) / widthInT), this->Position.Y + this->Size.Y);

	// Left edge
	return Vector2(this->Position.X, this->Position.Y + this->Size.Y * ((1.0f - t) / heightInT));
}

// *****************************************************************
/// @brief
///     Creates a segment that represents the left side of the rectangle
// *****************************************************************
Segment2 Rectangle2::GetLeftLineSegment() const
{
	return Segment2(
		this->Position,
		Vector2(this->Position.X, this->Position.Y + this->Size.Y)
		);
}

// *****************************************************************
/// @brief
///     Creates a segment that represents the top side of the rectangle
// *****************************************************************
Segment2 Rectangle2::GetTopLineSegment() const
{
	return Segment2(
		this->Position,
		Vector2(this->Position.X + this->Size.X, this->Position.Y)
		);
}

// *****************************************************************
/// @brief
///     Creates a segment that represents the right side of the rectangle
// *****************************************************************
Segment2 Rectangle2::GetRightLineSegment() const
{
	return Segment2(
		Vector2(this->Position.X + this->Size.X, this->Position.Y),
		Vector2(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y)
		);
}

// *****************************************************************
/// @brief
///     Creates a segment that represents the bottom side of the rectangle
// *****************************************************************
Segment2 Rectangle2::GetBottomLineSegment() const
{
	return Segment2(
		Vector2(this->Position.X, this->Position.Y + this->Size.Y),
		Vector2(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y)
		);
}

// *****************************************************************
/// @brief
///     Creates an oriented Box in the same shape as the rectangle
// *****************************************************************
Box2 Rectangle2::GetAsBox() const
{
    Vector2 halfSize = this->Size * 0.5f;
    Vector2 center = this->Position + halfSize;
	return Box2(center, Vector2::UNIT_X, Vector2::UNIT_Y, halfSize.X, halfSize.Y);
}

// *****************************************************************
/// @brief
///     Creates a best-fit rectangle that encapsulates a set of points
/// @param numPoints
///     The number of points in the array
/// @param points
///     An array of points
// *****************************************************************
Rectangle2 Rectangle2::FromPoints(size_t numPoints, const Vector2* points)
{
	// Use the first point to set the initial bounds
	float minX = points[0].X;
	float maxX = points[0].X;
	float minY = points[0].Y;
	float maxY = points[0].Y;
	
	// loop through the points & check for new bounds
	for(size_t i=1; i<numPoints; i++)
	{
		if(points[i].X < minX)
			minX = points[i].X;
		else if(points[i].X > maxX)
			maxX = points[i].X;
        
		if(points[i].Y < minY)
			minY = points[i].Y;
		else if(points[i].Y > maxY)
			maxY = points[i].Y;
	}
    
    return Rectangle2(minX, minY, maxX - minX, maxY - minY);
}

// *****************************************************************
/// @brief
///     Creates a best-fit rectangle that exactly encapsulates two rectangles
// *****************************************************************
Rectangle2 Rectangle2::Merge(const Rectangle2& r1, const Rectangle2& r2)
{
	// Returns the rectangle that encapsulates the 2 given rectangles
	Vector2 points[4] = {r1.Position, r1.Position + r1.Size, r2.Position, r2.Position + r2.Size};
	return FromPoints(4, points);
}


