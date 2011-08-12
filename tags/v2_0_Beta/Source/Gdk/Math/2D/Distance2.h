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
    // The Distance2 class contains methods finding distances between 2D primitives
    //
    
    // =============================================
	class Distance2
	{
    private:
		// Internal CTor
		Distance2() {}

	public:
        
		// Point Distance Methods
		// ---------------------------
        
        static float PointToRay(const Vector2& point, const Ray2& ray);
        static float PointToLineSegment(const Vector2& point, const LineSegment2& lineSegment);
        static float PointToBox(const Vector2& point, const Box2& box);
        static float PointToRectangle(const Vector2& point, const Rectangle2& rect);
        static float PointToCircle(const Vector2& point, const Circle2& circle);
	
    };

} // namespace
