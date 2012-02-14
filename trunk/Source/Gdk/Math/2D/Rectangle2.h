/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Vector2.h"
#include "Segment2.h"
#include "Box2.h"

namespace Gdk 
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _2D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents an axis aligned 2D rectangle.
    // =================================================================================
	class Rectangle2
	{
	public:
		
        // Public Properties
		// =====================================================
        
        /// The position of the rectangle
		Vector2 Position;
        
        /// The size (width & height) of the rectangle
		Vector2 Size;

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Rectangle2();
		Rectangle2(float x, float y, float width, float height);
		Rectangle2(const Vector2& position, const Vector2& size);
		Rectangle2(const Rectangle2& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Rectangle2& operator= (const Rectangle2& input);
		inline bool operator== (const Rectangle2& input) const;
		inline bool operator!= (const Rectangle2& input) const;
        
        /// @}
        // ---------------------------------
        /// @name Containment Methods
        /// @{
        
		inline bool Contains(float x, float y) const;
		inline bool Contains(const Vector2& point) const;

        /// @}
        // ---------------------------------
        /// @name Geometric Methods
        /// @{
        
        Vector2 GetCenter() const;
        Vector2 GetExtents() const;
        
		Vector2 GetParametricPointInArea(float tx, float ty) const;
		Vector2 GetParametricPointOnPerimeter(float t) const;

		Segment2 GetLeftLineSegment() const;
		Segment2 GetTopLineSegment() const;
		Segment2 GetRightLineSegment() const;
		Segment2 GetBottomLineSegment() const;
        
        Box2 GetAsBox() const;
        
        static Rectangle2 FromPoints(size_t numPoints, const Vector2* points);
		static Rectangle2 Merge(const Rectangle2& r1, const Rectangle2& r2);
        static Rectangle2 Intersection(const Rectangle2& r1, const Rectangle2& r2);
        
        /// @}

        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{
        
		/// Zero rectangle: position=(0,0), size=(0,0)
		static const Rectangle2 ZERO;
        
        /// Unit rectangle: position=(0,0), size=(1,1)
		static const Rectangle2 UNIT;
        
        /// @}
	};
    
	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignement operator: copies the values of the given rectangle to this rectangle
    // *****************************************************************
	inline Rectangle2& Rectangle2::operator= (const Rectangle2& input)
	{
		Position = input.Position;
		Size = input.Size;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two rectangles have the same position and size
    // *****************************************************************
    inline bool Rectangle2::operator== (const Rectangle2& input) const
	{
		return 
			Position == input.Position && Size == input.Size;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two rectangles have different positions or sizes
    // *****************************************************************
	inline bool Rectangle2::operator!= (const Rectangle2& input) const
	{
		return 
			Position != input.Position || Size != input.Size;
	}

    // *****************************************************************
    /// @brief
    ///     Checks if the rectangle contains a given point
    // *****************************************************************
    inline bool Rectangle2::Contains(float x, float y) const
	{
		return x >= Position.X
			&& y >= Position.Y
			&& x <= Position.X + Size.X
			&& y <= Position.Y + Size.Y
			;
	}

    // *****************************************************************
    /// @brief
    ///     Checks if the rectangle contains a given point
    // *****************************************************************
	inline bool Rectangle2::Contains(const Vector2& point) const
	{
		return Contains(point.X, point.Y);
	}
    
} // namespace
