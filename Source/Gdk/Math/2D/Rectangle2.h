/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



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

		// Containment / Intersection Methods
		inline bool Contains(float x, float y) const;
		inline bool Contains(const Vector2& point) const;
		inline bool Intersects(const Rectangle2& rectangle) const;

		// Point Conversions
        Vector2 GetCenter();
        Vector2 GetExtents();
		Vector2 GetParametricPointInArea(float tx, float ty);
		Vector2 GetParametricPointOnPerimeter(float t);

        // Line Segment Conversions
		LineSegment2 GetLeftLineSegment();
		LineSegment2 GetTopLineSegment();
		LineSegment2 GetRightLineSegment();
		LineSegment2 GetBottomLineSegment();

        // Statics
        // ----------------------------------------
        
		// Static Creation methods
        static Rectangle2 CreateFromPoints(size_t numPoints, const Vector2* points);
		static Rectangle2 CreateMerged(Rectangle2 r1, Rectangle2 r2);

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
	// Containment & Intersection Methods
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
		return point.X >= Position.X
			&& point.Y >= Position.Y
			&& point.X <= Position.X + Size.X
			&& point.Y <= Position.Y + Size.Y
			;
	}

	// ***********************************************************************
	inline bool Rectangle2::Intersects(const Rectangle2& rectangle) const
	{
		return rectangle.Position.X <= (Position.X + Size.X)
			&& rectangle.Position.X + rectangle.Size.X >= Position.X
			&& rectangle.Position.Y <= (Position.Y + Size.Y)
			&& rectangle.Position.Y + rectangle.Size.Y >= Position.Y
			;
	}
    
} // namespace
