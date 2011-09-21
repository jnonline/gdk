/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Ray3.h"
#include "Box3.h"

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
        static float PointToPlane(const Vector3& point, const Plane3& plane);
        static float PointToBox(const Vector3& point, const Box3& box);
        static float PointToSphere(const Vector3& point, const Sphere3& sphere);
        
        /// @}
    };
    
    /// @}  _3D
    /// @}  Math

} // namespace
