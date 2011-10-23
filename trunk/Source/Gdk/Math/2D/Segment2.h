/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _2D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents a 2D line segment
    // =================================================================================
	class Segment2
	{
	public:
        
		// Public Properties
		// =====================================================
        
        /// Center point of the segment
		Vector2 Center;
        
        /// Direction vector of the segment
		Vector2 Direction;
        
        /// Extent (half-size) of the segment
        float Extent;

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Segment2();
        Segment2(const Vector2& center, const Vector2& direction, float extent);
		Segment2(const Vector2& start, const Vector2& end);
		Segment2(const Segment2& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Segment2& operator= (const Segment2& input);
		inline bool operator== (const Segment2& input) const;
		inline bool operator!= (const Segment2& input) const;
        
        /// @}
        // ---------------------------------
        /// @name Geometric Methods
        /// @{
        
        float GetLength() const;
        Vector2 GetEndpoint1() const;
        Vector2 GetEndpoint2() const;
        
		Vector2 GetParametricPoint(float t) const;
        
        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignement operator: copies the values of the given line segment to this line segment
    // *****************************************************************
	inline Segment2& Segment2::operator= (const Segment2& input)
	{
		Center = input.Center;
		Direction = input.Direction;
        Extent = input.Extent;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two line segments share the same start and end points
    // *****************************************************************
    inline bool Segment2::operator== (const Segment2& input) const
	{
		return 
			Center == input.Center && Direction == input.Direction && Extent == input.Extent;
	}

    // *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two line segments have different start or end points
    // *****************************************************************
	inline bool Segment2::operator!= (const Segment2& input) const
	{
		return 
			Center != input.Center || Direction != input.Direction || Extent != input.Extent;
	}

} // namespace
