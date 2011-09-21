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
    ///     Represents a 3D frustrum.
    /// @remarks
    ///     A frustrum is made of 6 planes defining the 6 sides of the frustrum
	// =================================================================================
	class Frustrum3
	{
	public:

        // Public Properties
		// =====================================================
        
        /// The planar 6 sides of the frustrum.
		Plane3 Planes[6];
        
        /// The 8 corners of the frustrum
        Vector3 Corners[8];

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Frustrum3(const Matrix3D& matrix);
		Frustrum3(const Frustrum3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Frustrum3& operator= (const Frustrum3& input);
        
        /// @}
        // ---------------------------------
        /// @name Containment Methods
        /// @{
        
        inline bool Contains(float x, float y, float z) const;
		inline bool Contains(const Vector3& point) const;
        
        /// @}
        // ---------------------------------
        /// @name Utility Methods
        /// @{
        
        // Calculates the frustrum planes & corners from the given matrix
        void SetMatrix(const Matrix3D& matrix);
        
        /// @}
        
    private:
        // Internal methods
        void CalculateCorners();
        Ray3 GetIntersectionRay(const Plane3& plane1, const Plane3& plane2);
        Vector3 GetIntersectionPoint(const Plane3& plane, const Ray3& ray);

	};
    
    /// @}  _3D
    /// @}  Math

	// Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies the values of the given frustrum to this frustrum
    // *****************************************************************
	inline Frustrum3& Frustrum3::operator= (const Frustrum3& input)
	{
		Planes[0] = input.Planes[0];
		Planes[1] = input.Planes[1];
		Planes[2] = input.Planes[2];
		Planes[3] = input.Planes[3];
		Planes[4] = input.Planes[4];
		Planes[5] = input.Planes[5];
		
		return *this;
	}
    
	// *****************************************************************
    /// @brief
    ///     Checks if the given 3D point is inside this frustrum
    // *****************************************************************
    bool Frustrum3::Contains(float x, float y, float z) const
    {
        // Loop through the planes
        for(int i=0; i<6; i++)
        {
            float distance = 
                Planes[i].Normal.X * x +
                Planes[i].Normal.Y * y +
                Planes[i].Normal.Z * z +
                Planes[i].D;
            
            // Is the point outside of this plane?
            if(distance > Math::ZERO_TOLERANCE)
                return false;
        }
        
        return true;
    }
    
    // *****************************************************************
    /// @brief
    ///     Checks if the given 3D point is inside this frustrum
    // *****************************************************************
    bool Frustrum3::Contains(const Vector3& point) const
    {
        return Contains(point.X, point.Y, point.Z);
    }

} // namespace

