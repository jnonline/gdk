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
#include "Frustum3.h"

namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _3D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Provides static methods for testing and finding intersections between 3D primitives
    /// @remarks
    ///     @li Test() - The Test methods simply test for an intersection between two primitives, 
    ///                  which is generally much faster than finding the actual intersection.
    ///
    ///     @li Find() - The Find methods find the actual intersection between two primitives, which
    ///                  may be a complex shape in itself.  This operation is generally slower than
	///                  simply testing for an intersection
    // =================================================================================
	class Intersection3
	{
    protected:
        
		// Internal CTor
		Intersection3() {}
        
	public:
        
        // Public Methods
		// =====================================================
        
        // --------------------------------------------
        /// @name Test Intersection Methods
        /// @{
        
        static bool Test(const Ray3& ray, const Plane3& plane);
		static bool Test(const Ray3& ray, const Sphere3& sphere);
		static bool Test(const Ray3& ray, const Box3& box);
        static bool Test(const Ray3& ray, const Capsule3& capsule);
		static bool Test(const Ray3& ray, const Frustum3& frustum);

        static bool Test(const Frustum3& frustum, const Plane3& plane);
        static bool Test(const Frustum3& frustum, const Sphere3& sphere);
        
        // Frustum -> Box
        // TODO(P2):  Articles to checkout:
        //      "Intersection of orthogonal view Frustum and oriented bounding box using seperation axis testing" : Eberly
        //      "Optimized view Frustum culling algorithms for bounding boxes" : Assarsson & Moller
        //static bool Test(const Frustum3& Frustum, const Box3& box);
        
        static bool Test(const Plane3& plane1, const Plane3& plane2);
        static bool Test(const Plane3& plane, const Sphere3& sphere);
        static bool Test(const Plane3& plane, const Box3& box);
        static bool Test(const Plane3& plane, const Capsule3& capsule);
        
        static bool Test(const Sphere3& sphere1, const Sphere3& sphere2);
        static bool Test(const Sphere3& sphere, const Box3& box);
        static bool Test(const Sphere3& sphere, const Capsule3& capsule);
        
        static bool Test(const Box3& box1, const Box3& box2);
        
        static bool Test(const Capsule3& capsule1, const Capsule3& capsule2);
        
        /// @}
        // --------------------------------------------
        /// @name Find Intersection Methods
        /// @{
        
        static bool Find(const Ray3& ray, const Plane3& plane, float& distance);
        static bool Find(const Ray3& ray, const Sphere3& sphere, float& distance);
        static bool Find(const Ray3& ray, const Box3& box, float& distance);
        static bool Find(const Ray3& ray, const Capsule3& capsule, float& distance);
        static bool Find(const Ray3& ray, const Frustum3& frustum, float& distance);

        /// @}
        // --------------------------------------------
        /// @name Utility methods
        /// @{
        
        static IntersectionType::Enum Intersect_Line_Box(
            float startT, float endT,
            const Vector3& origin, const Vector3& direction,
            const Box3& box,
            float t[2]
            );
        static bool Clip_Box_Line(float denominator, float numerator, float& t0, float& t1);
        
        static IntersectionType::Enum Intersect_Line_Capsule(
            const Vector3& origin, const Vector3& direction,
            const Capsule3& capsule,
            float t[2]
            );
        
        /// @}
	};
    
    /// @}  _3D
    /// @}  Math

} // namespace
