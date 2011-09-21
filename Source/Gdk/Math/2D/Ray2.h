/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _2D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents a 2D ray primitive
    // =================================================================================
	class Ray2
	{
	public:
        
        // Public Properties
		// =====================================================
        
        /// Origin of the ray
		Vector2 Origin;
        
        /// Direction vector of the ray (most GDK math operations assume the direction is normalized)
		Vector2 Direction;
        
        // Public Methods
		// =====================================================

        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Ray2 ();
		Ray2 (const Vector2& origin, const Vector2& direction);
		Ray2 (const Ray2& input);
        
        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Ray2& operator= (const Ray2& input);
        
        /// @}
        
	};

    /// @}
    /// @}

    // Inline Implementations
	// ====================================

	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies values of a given ray to this ray.
    // *****************************************************************
	inline Ray2& Ray2::operator= (const Ray2& input)
	{
		Origin = input.Origin;
		Direction = input.Direction;
		return *this;
	}
    
} // namespace Gdk
