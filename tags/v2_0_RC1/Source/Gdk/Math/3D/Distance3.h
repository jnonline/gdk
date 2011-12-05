/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Vector3.h"
#include "Ray3.h"
#include "Segment3.h"
#include "Plane3.h"
#include "Box3.h"
#include "Sphere3.h"
#include "Capsule3.h"

namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _3D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Provides static methods for finding distances between 3D primitives
	// =================================================================================
	class Distance3
	{
    private:
		// Internal CTor
		Distance3() {}

	public:
        
		// Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        static float PointToPoint(const Vector3& point1, const Vector3& point2);
        static float PointToRay(const Vector3& point, const Ray3& ray);
        static float PointToSegment(const Vector3& point, const Segment3& segment);
        static float PointToPlane(const Vector3& point, const Plane3& plane);
        static float PointToBox(const Vector3& point, const Box3& box);
        static float PointToSphere(const Vector3& point, const Sphere3& sphere);
        static float PointToCapsule(const Vector3& point, const Capsule3& capsule);
        
        static float RayToSegment(const Ray3& ray, const Segment3& segment);
        static float RayToSphere(const Ray3& ray, const Sphere3& sphere);
        static float RayToCapsule(const Ray3& ray, const Capsule3& capsule);

        static float SegmentToSegment(const Segment3& segment1, const Segment3& segment2);
        
        /// @}
    };
    
    /// @}  _3D
    /// @}  Math

} // namespace
