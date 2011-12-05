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
    ///     Represents a 2D oriented box.
    /// @remarks
    ///     Oriented boxes are defined by a center point, an orthonormal basis for axes, 
    ///     and extents (half-sizes) along the axes
    // =================================================================================
	class Box2
	{
	public:
		
        // Public Properties
		// =====================================================
        
        /// Center of the box
		Vector2 Center;
        
        /// Two orthonormalized axes of the box
		Vector2 Axis[2];
        
        /// Extent (half-size) of the box along each axis
		float Extent[2];

		// Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Box2 ();
		Box2 (const Vector2& center, const Vector2& axis1, const Vector2& axis2, float extent1, float extent2);
		Box2 (const Box2& input);

        /// @}
        // ---------------------------------
        /// @name Containment Methods
        /// @{
        
        inline bool Contains(float x, float y) const;
		inline bool Contains(const Vector2& point) const;
        
        /// @}
        
        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{
        
        /// Zero-size (extents=0,0) box, centered at (0,0)
        static const Box2 ZERO;
        
        /// Unit-size (extents=0.5,0.5) box, centered at (0,0)
        static const Box2 UNIT;
        
        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Checks if the box contains a given point
    // *****************************************************************
	inline bool Box2::Contains(float x, float y) const
	{
		// Get the vector from the center of the box to the point
		Vector2 diff(x - this->Center.X, y - this->Center.Y);
		
		// Loop through the axis
		for(int i=0; i<2; i++)
		{
			// Get the distance from the point to the center along this axis
			float dist = diff.Dot(this->Axis[i]);

			// Is the point outside the extent of this axis?
			if(Math::Abs(dist) > this->Extent[i])
				return false;
		}

		return true;
	}
    
	// *****************************************************************
    /// @brief
    ///     Checks if the box contains a given point
    // *****************************************************************
	inline bool Box2::Contains(const Vector2& point) const
	{
        return Contains(point.X, point.Y);
    }

} // namespace Gdk
