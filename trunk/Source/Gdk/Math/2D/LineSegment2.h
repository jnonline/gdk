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
    ///     Represents a 2D line segment between two points.
    // =================================================================================
	class LineSegment2
	{
	public:
        
		// Public Properties
		// =====================================================
        
        /// Starting point of the line segment
		Vector2 Start;
        
        /// Ending point of the line segment
		Vector2 End;

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		LineSegment2();
		LineSegment2(const Vector2& start, const Vector2& end);
		LineSegment2(const LineSegment2& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline LineSegment2& operator= (const LineSegment2& input);
		inline bool operator== (const LineSegment2& input) const;
		inline bool operator!= (const LineSegment2& input) const;
        
        /// @}
        // ---------------------------------
        /// @name Geometric Methods
        /// @{
        
		Vector2 GetParametricPoint(float t);
        
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
	inline LineSegment2& LineSegment2::operator= (const LineSegment2& input)
	{
		Start = input.Start;
		End = input.End;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two line segments share the same start and end points
    // *****************************************************************
    inline bool LineSegment2::operator== (const LineSegment2& input) const
	{
		return 
			Start == input.Start && End == input.End;
	}

    // *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two line segments have different start or end points
    // *****************************************************************
	inline bool LineSegment2::operator!= (const LineSegment2& input) const
	{
		return 
			Start != input.Start || End != input.End;
	}

} // namespace
