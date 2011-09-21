/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Triangle2.h"
#include "Rectangle2.h"

namespace Gdk 
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _2D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents a 2D circle.
    // =================================================================================
	class Circle2
	{
	public:

        // Public Properties
		// =====================================================
        
        /// Center of the circle
		Vector2 Center;
        
        /// Radius of the circle
		float Radius;

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Circle2();
		Circle2(float x, float y, float radius);
		Circle2(const Vector2& center, float radius);
		Circle2(const Circle2& input);
        
        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Circle2& operator= (const Circle2& input);
		inline bool operator== (const Circle2& input) const;
		inline bool operator!= (const Circle2& input) const;
        
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
        
        Vector2 GetParametricPointInArea(float tAngle, float tRadius);
		Vector2 GetParametricPointOnPerimeter(float tAngle);
        
		static Circle2 FromPoints(size_t numPoints, const Vector2* points);
		static Circle2 Merge(const Circle2& c1, const Circle2& c2);
        
        static Circle2 CircumscribeTriangle(const Triangle2& triangle);
        static Circle2 CircumscribeTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3);
        static Circle2 InscribeTriangle(const Triangle2& triangle);
        static Circle2 InscribeTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3);
        static Circle2 CircumscribeRectangle(const Rectangle2& rect);

        /// @}
        
        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{
        
        /// Zero circle centered at (0,0) with radius 0
		static const Circle2 ZERO;    // Center(0,0) Radius(0)
        
        /// Unit circle centered at (0,0) with radius 1.0
        static const Circle2 UNIT;
        
        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignement operator: copies the values of the given circle to this circle
    // *****************************************************************
	inline Circle2& Circle2::operator= (const Circle2& input)
	{
		Center = input.Center;
		Radius = input.Radius;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two circles have the same center and radius
    // *****************************************************************
    inline bool Circle2::operator== (const Circle2& input) const
	{
		return 
			Center == input.Center && Radius == input.Radius;
	}

    // *****************************************************************
    /// @brief
    ///     Inequality operator: returns true if two circles have a different center or radius
    // *****************************************************************
	inline bool Circle2::operator!= (const Circle2& input) const
	{
		return 
			Center != input.Center || Radius != input.Radius;
	}

    // *****************************************************************
    /// @brief
    ///     Checks if the circle contains a given point
    // *****************************************************************
    inline bool Circle2::Contains(float x, float y) const
	{
		// Get the distance squared between the circle center & the point
		float xDiff = x - this->Center.X;
		float yDiff = y - this->Center.Y;
		float distanceSquared = xDiff * xDiff + yDiff * yDiff;
		return distanceSquared <= this->Radius * this->Radius;
	}

	// *****************************************************************
    /// @brief
    ///     Checks if the circle contains a given point
    // *****************************************************************
	inline bool Circle2::Contains(const Vector2& point) const
	{
		return Contains(point.X, point.Y);
	}


} // namespace
