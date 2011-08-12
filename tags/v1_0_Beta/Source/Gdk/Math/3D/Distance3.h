/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Ray3.h"
#include "Box3.h"

namespace Gdk
{
    // =============================================
    // The Distance3 class contains methods finding distances between 2D primitives
    //
    
    // =============================================
	class Distance3
	{
    private:
		// Internal CTor
		Distance3() {}

	public:
        
		// Point Distance Methods
		// ---------------------------
        
        static float PointToRay(const Vector3& point, const Ray3& ray);
        static float PointToPlane(const Vector3& point, const Plane3& plane);
        static float PointToBox(const Vector3& point, const Box3& box);
        static float PointToSphere(const Vector3& point, const Sphere3& sphere);
    };

} // namespace
