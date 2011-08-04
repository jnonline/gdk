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
    // This class contains methods for testing & finding intersections between 2D math primitives
    
	class Intersection2
	{
    protected:
		// Internal CTor
		Intersection2();

	public:
        
		// Intersection methods
		// ------------------------------------------------

        // Ray -> Ray
        //static bool Test(const Ray2& ray1, const Ray2& ray2);
        //static bool Find(const Ray2& ray1, const Ray2& ray2, Intersection2Result& result);
        
        // Ray -> Circle
        //static bool Test(const Ray2& ray1, const Circle2& circle1);
        //static bool Find(const Ray2& ray1, const Circle2& circle1, Intersection2Result& result);
        
        // Ray -> Rectangle
        
        // Ray -> Box
	
        // Ray -> Triangle
        
        // Circle -> Circle
        static bool Test(const Circle2& circle1, const Circle2& circle1);
        
        // Circle -> Rectangle
        
        // Circle -> Triangle
        
        // Rectangle -> Rectangle
        static bool Test(const Rectangle2& rect1, const Rectangle2& rect2);
        
        // Box -> Box
        static bool Test(const Box2& box1, const Box2& box2);
        

	};

} // namespace
