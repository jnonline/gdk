/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Triangle2
	{
	public:
		// Components
		Vector2 Vertices[3];
		
		// Constructors
		Triangle2 ();
		Triangle2 (const Vector2& v1, const Vector2& v2, const Vector2& v3);
        Triangle2 (const Vector2* vertices);
		Triangle2 (const Triangle2& input);
        
        // Containment Methods
		inline bool Contains(float x, float y) const;
		inline bool Contains(const Vector2& point) const;

	};
    
    // ===================================================================================
	// Containment Methods
	// ===================================================================================
    
	// ***********************************************************************
	inline bool Triangle2::Contains(float x, float y) const
	{
        return Contains(Vector2(x,y));
	}
    
	// ***********************************************************************
	inline bool Triangle2::Contains(const Vector2& point) const
	{
		// Compute 2 vectors for the triangle edges and 1 for the point.  (all from a basis vertex of the triangle)
        Vector2 v0 = Vertices[2] - Vertices[0];
        Vector2 v1 = Vertices[1] - Vertices[0];
        Vector2 v2 = point - Vertices[0];
        
        // Compute dot products
        float dot00 = v0.Dot(v0);
        float dot01 = v0.Dot(v1);
        float dot02 = v0.Dot(v2);
        float dot11 = v1.Dot(v1);
        float dot12 = v1.Dot(v2);
        
        // Compute barycentric coordinates of the point
        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        
        // Check if point is in triangle
        return (u > 0.0f) && (v > 0.0f) && (u + v < 1.0f);
	}

} // namespace Gdk
