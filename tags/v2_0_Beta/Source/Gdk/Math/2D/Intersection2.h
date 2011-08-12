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
#include "LineSegment2.h"

namespace Gdk
{
    // =============================================
    // The Intersection2 class contains methods for testing & finding intersections between 2D math primitives.
    // Use the appropriate method for each intersection type:
    //
    // Test() - Simply returns true if the 2 primitives intersect in some way  (Faster than calling Find())
    // Find() - Finds the actual intersection of 2 primitives.

    // =============================================
	class Intersection2
	{
    protected:
        
		// Internal CTor
		Intersection2() {}

	public:
        
		// Ray Intersection methods
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        
        // NOTE: the 'distance' out-parameter in the Find methods, is the length along the casting (first) ray
        
        // Ray -> Ray
        static bool Test(const Ray2& ray1, const Ray2& ray2);
        static bool Find(const Ray2& ray1, const Ray2& ray2, float& distance);
        
        // Ray -> LineSegment
        static bool Test(const Ray2& ray, const LineSegment2& lineSegment);
        static bool Find(const Ray2& ray1, const LineSegment2& lineSegment, float& distance);
        
        // Ray -> Circle
        static bool Test(const Ray2& ray, const Circle2& circle);
        static bool Find(const Ray2& ray1, const Circle2& circle, float& distance);
        
        // Ray -> Rectangle
        static bool Test(const Ray2& ray, const Rectangle2& rect);
        static bool Find(const Ray2& ray1, const Rectangle2& rect, float& distance);

        // Ray -> Box
        static bool Test(const Ray2& ray, const Box2& box);
        static bool Find(const Ray2& ray1, const Box2& box, float& distance);
	
        
        // Primitive Intersection Methods
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        
        // Circle -> Circle
        static bool Test(const Circle2& circle1, const Circle2& circle2);
        
        // Circle -> Rectangle
        static bool Test(const Circle2& circle, const Rectangle2& rect);
        
        // Circle -> Box
        static bool Test(const Circle2& circle, const Box2& box);
        
        // -------------------------
        
        // Rectangle -> Rectangle
        static bool Test(const Rectangle2& rect1, const Rectangle2& rect2);
        
        // Rectangle -> Box
        static bool Test(const Rectangle2& rect, const Box2& box);
        
        // -------------------------
        
        // Box -> Box
        static bool Test(const Box2& box1, const Box2& box2);
        

        // Utility methods
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        
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
	};

} // namespace
