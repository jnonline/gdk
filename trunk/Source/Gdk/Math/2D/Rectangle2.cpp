/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Rectangle2.h"

using namespace Gdk;

// instantiations of static constants
const Rectangle2 Rectangle2::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Rectangle2::Rectangle2()
{
}

// ***********************************************************************
Rectangle2::Rectangle2(float x, float y, float width, float height)
{
	Position.X = x;
	Position.Y = y;
	Size.X = width;
	Size.Y = height;
}

// ***********************************************************************
Rectangle2::Rectangle2(const Vector2& position, const Vector2& size)
{
	Position = position;
	Size = size;
}

// ***********************************************************************
Rectangle2::Rectangle2(const Rectangle2& input)
{
	Position = input.Position;
	Size = input.Size;
}

// ***********************************************************************
Rectangle2::Rectangle2(const Vector2* points, int numPoints)
{
	// Use the first point to set the initial bounds
	float minX = points[0].X;
	float maxX = points[0].X;
	float minY = points[0].Y;
	float maxY = points[0].Y;
	
	// loop through the points & check for new bounds
	for(int i=1; i<numPoints; i++)
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

	this->Position.X = minX;
	this->Position.Y = minY;
	this->Size.X = maxX - minX;
	this->Size.Y = maxY - minY;
}


// ===================================================================================
// Utility Methods
// ===================================================================================

// ***********************************************************************
Vector2 Rectangle2::GetParametricPointInArea(float tx, float ty)
{
	// Return a point in the rectangle using the given parametric coordinates within rectangle space
	return Vector2(
		this->Position.X + this->Size.X * tx, 
		this->Position.Y + this->Size.Y * ty
		);
}

// ***********************************************************************
Vector2 Rectangle2::GetParametricPointOnPerimeter(float t)
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

// ***********************************************************************
LineSegment2 Rectangle2::GetLeftLineSegment()
{
	return LineSegment2(
		this->Position,
		Vector2(this->Position.X, this->Position.Y + this->Size.Y)
		);
}

// ***********************************************************************
LineSegment2 Rectangle2::GetTopLineSegment()
{
	return LineSegment2(
		this->Position,
		Vector2(this->Position.X + this->Size.X, this->Position.Y)
		);
}

// ***********************************************************************
LineSegment2 Rectangle2::GetRightLineSegment()
{
	return LineSegment2(
		Vector2(this->Position.X + this->Size.X, this->Position.Y),
		Vector2(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y)
		);
}

// ***********************************************************************
LineSegment2 Rectangle2::GetBottomLineSegment()
{
	return LineSegment2(
		Vector2(this->Position.X, this->Position.Y + this->Size.Y),
		Vector2(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y)
		);
}

// ===================================================================================
// Static Utility Methods
// ===================================================================================

// ***********************************************************************
Rectangle2 Rectangle2::Merge(Rectangle2 r1, Rectangle2 r2)
{
	// Returns the rectangle that encapsulates the 2 given rectangles
	Vector2 points[4] = {r1.Position, r1.Position + r1.Size, r2.Position, r2.Position + r2.Size};
	return Rectangle2(points, 4);
}