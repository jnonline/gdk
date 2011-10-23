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
    ///     Provides static methods for testing and finding intersections between 2D primitives
    /// @remarks
    ///     @li Test() - The Test methods simply test for an intersection between two primitives, 
    ///                  which is generally much faster than finding the actual intersection.
    ///
    ///     @li Find() - The Find methods find the actual intersection between two primitives, which
    ///                  may be a complex shape in itself.  This operation is generally slower than
	///                  simply testing for an intersection
    // =================================================================================
	class Intersection2
	{
	public:
        
        // Public Methods
		// =====================================================
        
        // --------------------------------------------
        /// @name Test Intersection Methods
        /// @{
		
        static bool Test(const Ray2& ray1, const Ray2& ray2);
        static bool Test(const Ray2& ray, const Segment2& segment);
        static bool Test(const Ray2& ray, const Circle2& circle);
        static bool Test(const Ray2& ray, const Rectangle2& rect);
        static bool Test(const Ray2& ray, const Box2& box);
	
        static bool Test(const Circle2& circle1, const Circle2& circle2);
        static bool Test(const Circle2& circle, const Rectangle2& rect);
        static bool Test(const Circle2& circle, const Box2& box);
        
        static bool Test(const Rectangle2& rect1, const Rectangle2& rect2);
        static bool Test(const Rectangle2& rect, const Box2& box);
        
        static bool Test(const Box2& box1, const Box2& box2);
        
        /// @}
        
        // --------------------------------------------
        /// @name Find Intersection Methods
        /// @{
		
        static bool Find(const Ray2& ray1, const Ray2& ray2, float& distance);
        static bool Find(const Ray2& ray1, const Segment2& segment, float& distance);
        static bool Find(const Ray2& ray1, const Circle2& circle, float& distance);
        static bool Find(const Ray2& ray1, const Rectangle2& rect, float& distance);
        static bool Find(const Ray2& ray1, const Box2& box, float& distance);
        
        /// @}
        // --------------------------------------------
        /// @name Internal Utility Methods
        /// @{
        
        static IntersectionType::Enum Intersect_Line_Line(
            const Vector2& origin1, const Vector2& direction1,
            const Vector2& origin2, const Vector2& direction2,
            float t[2]
            );
        
        static IntersectionType::Enum Intersect_Line_Circle(
            const Vector2& origin, const Vector2& direction,
            const Vector2& center, float radius,
            float t[2]
            );
                                                            
        static IntersectionType::Enum Intersect_Line_Box(
            float startT, float endT,
            const Vector2& origin, const Vector2& direction,
            const Box2& box,
            float t[2]
            );
        static bool Clip_Box_Line(float denominator, float numerator, float& t0, float& t1);
        
        /// @}
	};
    
    /// @}  _2D
    /// @}  Math

} // namespace
