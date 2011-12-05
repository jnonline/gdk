/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _3D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents a 3D line segment
    // =================================================================================
	class Segment3
	{
	public:
        
		// Public Properties
		// =====================================================
        
        /// Center point of the segment
		Vector3 Center;
        
        /// Direction vector of the segment
		Vector3 Direction;
        
        /// Extent (half-size) of the segment
        float Extent;


        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Segment3();
        Segment3(const Vector3& center, const Vector3& direction, float extent);
		Segment3(const Vector3& start, const Vector3& end);
		Segment3(const Segment3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Segment3& operator= (const Segment3& input);
		inline bool operator== (const Segment3& input) const;
		inline bool operator!= (const Segment3& input) const;
        
        /// @}
        // ---------------------------------
        /// @name Geometric Methods
        /// @{

        float GetLength() const;
        Vector3 GetEndpoint1() const;
        Vector3 GetEndpoint2() const;
        
		Vector3 GetParametricPoint(float t) const;
        
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
	inline Segment3& Segment3::operator= (const Segment3& input)
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
    inline bool Segment3::operator== (const Segment3& input) const
	{
		return 
			Center == input.Center && Direction == input.Direction && Extent == input.Extent;
	}

    // *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two line segments have different start or end points
    // *****************************************************************
	inline bool Segment3::operator!= (const Segment3& input) const
	{
		return 
			Center != input.Center || Direction != input.Direction || Extent != input.Extent;
	}

} // namespace
