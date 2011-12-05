/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _3D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents a 3D plane.
    // =================================================================================
	class Plane3
	{
	public:
		
        // Public Properties
		// =====================================================
        
        /// Normal of the plane
		Vector3 Normal;
        
        /// Distance from the origin to the plane (along the normal)
		float D;
        
        // Public Methods
		// =====================================================

		// ---------------------------------
        /// @name Constructors
        /// @{
        
		Plane3();
		Plane3(const Vector3& normal, float d);
		Plane3(const Vector3& point1, const Vector3& point2, const Vector3& point3);	// Create a plane containing 3 points
		Plane3(const Vector3& normal, const Vector3& point);								// Create a plane from a normal & a point on the plane
		Plane3(const Plane3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Plane3& operator= (const Plane3& input);

        /// @}
        // ---------------------------------
        /// @name Utility Methods
        /// @{
        
		// Planar Methods
		int WhichSide(const Vector3& point) const;		
		float DistanceTo(const Vector3& point) const;
        
        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies values of a given plane to this plane.
    // *****************************************************************
	inline Plane3& Plane3::operator= (const Plane3& input)
	{
		Normal = input.Normal;
		D = input.D;
		return *this;
	}

} // namespace
