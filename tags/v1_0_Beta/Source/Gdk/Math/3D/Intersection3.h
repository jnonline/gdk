/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Vector3.h"
#include "Ray3.h"
#include "Plane3.h"
#include "Box3.h"
#include "Sphere3.h"
#include "Frustrum3.h"

namespace Gdk
{
    // =============================================
    // The Intersection3 class contains methods for testing & finding intersections between 3D math primitives.
    // Use the appropriate method for each intersection type:
    //
    // Test() - Simply returns true if the 2 primitives intersect in some way  (Faster than calling Find())
    // Find() - Finds the actual intersection of 2 primitives.
    
    // =============================================
	class Intersection3
	{
    protected:
        
		// Internal CTor
		Intersection3() {}
        
	public:
        
		// Ray Intersection Methods
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        
        // NOTE: the 'distance' out-parameter in the Find methods, is the length along the casting (first) ray

		// Ray -> Plane
		static bool Test(const Ray3& ray, const Plane3& plane);
		static bool Find(const Ray3& ray, const Plane3& plane, float& distance);

		// Ray -> Sphere
		static bool Test(const Ray3& ray, const Sphere3& sphere);
		static bool Find(const Ray3& ray, const Sphere3& sphere, float& distance);
        
        // Ray -> Box
		static bool Test(const Ray3& ray, const Box3& box);
		static bool Find(const Ray3& ray, const Box3& box, float& distance);

        // Ray -> Frustrum
		static bool Test(const Ray3& ray, const Frustrum3& frustrum);
		static bool Find(const Ray3& ray, const Frustrum3& frustrum, float& distance);

        
        // Primitive Intersection Methods
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        // Frustrum -> Plane
        static bool Test(const Frustrum3& frustrum, const Plane3& plane);
        
        // Frustrum -> Sphere
        static bool Test(const Frustrum3& frustrum, const Sphere3& sphere);
        
        // Frustrum -> Box
        // TODO(P2):  Articles to checkout:
        //      "Intersection of orthogonal view frustrum and oriented bounding box using seperation axis testing" : Eberly
        //      "Optimized view frustrum culling algorithms for bounding boxes" : Assarsson & Moller
        //static bool Test(const Frustrum3& frustrum, const Box3& box);
        
        // -----------------------------
        
        // Plane -> Plane
        static bool Test(const Plane3& plane1, const Plane3& plane2);
        
        // Plane -> Sphere
        static bool Test(const Plane3& plane, const Sphere3& sphere);
        
        // Plane -> Box
        static bool Test(const Plane3& plane, const Box3& box);
        
        // -----------------------------
        
        // Sphere -> Sphere
        static bool Test(const Sphere3& sphere1, const Sphere3& sphere2);
        
        // Sphere -> Box
        static bool Test(const Sphere3& sphere, const Box3& box);
        
        // -----------------------------
    
        // Box -> Box
        static bool Test(const Box3& box1, const Box3& box2);
        
        
        // Utility methods
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        
        static IntersectionType::Enum Intersect_Line_Box(
            float startT, float endT,
            const Vector3& origin, const Vector3& direction,
            const Box3& box,
            float t[2]
            );
        static bool Clip_Box_Line(float denominator, float numerator, float& t0, float& t1);
        
	};

} // namespace
