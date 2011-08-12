/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	class Frustrum3
	{
	public:
		// Components
		Plane3 Planes[6];
        Vector3 Corners[8];

		// Constructors
		Frustrum3(const Matrix3D& matrix);
		Frustrum3(const Frustrum3& input);

		// Asssignment Operator
		inline Frustrum3& operator= (const Frustrum3& input);

		// Containment
        inline bool Contains(float x, float y, float z) const;
		inline bool Contains(const Vector3& point) const;
        
        // Calculates the frustrum planes & corners from the given matrix
        Frustrum3 SetMatrix(const Matrix3D& matrix);
        
    private:
        // Internal methods
        Ray3 GetIntersectionRay(const Plane3& plane1, const Plane3& plane2);
        Vector3 GetIntersectionPoint(const Plane3& plane, const Ray3& ray);

	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
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
    
    // ===================================================================================
    // Containment Methods
    // ===================================================================================
    
    // ***********************************************************************
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
    }
    
    // ***********************************************************************
    bool Frustrum3::Contains(const Vector3& point) const
    {
        return Contains(point.X, point.Y, point.Z);
    }

} // namespace

