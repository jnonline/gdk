/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Ray2.h"
#include "Triangle2.h"
#include "Box2.h"
#include "Circle2.h"
#include "Rectangle2.h"
#include "Segment2.h"

namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _2D
    /// @{

    // =================================================================================
    /// @brief
    ///     Provides static methods for finding distances between 2D primitives
	// =================================================================================
    class Distance2
	{
	public:

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        static float PointToPoint(const Vector2& point1, const Vector2& point2); 
        static float PointToRay(const Vector2& point, const Ray2& ray);
        static float PointToSegment(const Vector2& point, const Segment2& segment);
        static float PointToBox(const Vector2& point, const Box2& box);
        static float PointToRectangle(const Vector2& point, const Rectangle2& rect);
        static float PointToCircle(const Vector2& point, const Circle2& circle);
	
        /// @}
    };
    
    /// @}  _2D
    /// @}  Math

} // namespace
