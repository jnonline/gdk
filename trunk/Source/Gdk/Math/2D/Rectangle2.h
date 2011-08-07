/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Vector2.h"
#include "LineSegment2.h"
#include "Box2.h"

namespace Gdk 
{

	class Rectangle2
	{
	public:
		// Components
		Vector2 Position;
		Vector2 Size;

		// Constructors
		Rectangle2();
		Rectangle2(float x, float y, float width, float height);
		Rectangle2(const Vector2& position, const Vector2& size);
		Rectangle2(const Rectangle2& input);

		// Asssignment Operator
		inline Rectangle2& operator= (const Rectangle2& input);

		// Comparison Operators
		inline bool operator== (const Rectangle2& input) const;
		inline bool operator!= (const Rectangle2& input) const;

		// Containment Methods
		inline bool Contains(float x, float y) const;
		inline bool Contains(const Vector2& point) const;

		// Point Conversions
        Vector2 GetCenter() const;
        Vector2 GetExtents() const;
		Vector2 GetParametricPointInArea(float tx, float ty) const;
		Vector2 GetParametricPointOnPerimeter(float t) const;

        // Line Segment Conversions
		LineSegment2 GetLeftLineSegment() const;
		LineSegment2 GetTopLineSegment() const;
		LineSegment2 GetRightLineSegment() const;
		LineSegment2 GetBottomLineSegment() const;
        
        // Box Conversion
        Box2 GetAsBox() const;

		// Encapsulation Methods
        static Rectangle2 FromPoints(size_t numPoints, const Vector2* points);
		static Rectangle2 Merge(const Rectangle2& r1, const Rectangle2& r2);

		// special constants
		static const Rectangle2 ZERO;    // (0,0)-(0,0)
	};
	
	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Rectangle2& Rectangle2::operator= (const Rectangle2& input)
	{
		Position = input.Position;
		Size = input.Size;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Rectangle2::operator== (const Rectangle2& input) const
	{
		return 
			Position == input.Position && Size == input.Size;
	}

	// ***********************************************************************
	inline bool Rectangle2::operator!= (const Rectangle2& input) const
	{
		return 
			Position != input.Position || Size != input.Size;
	}

	// ===================================================================================
	// Containment Methods
	// ===================================================================================

	// ***********************************************************************
	inline bool Rectangle2::Contains(float x, float y) const
	{
		return x >= Position.X
			&& y >= Position.Y
			&& x <= Position.X + Size.X
			&& y <= Position.Y + Size.Y
			;
	}

	// ***********************************************************************
	inline bool Rectangle2::Contains(const Vector2& point) const
	{
		return Contains(point.X, point.Y);
	}
    
} // namespace
