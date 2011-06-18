
#include "BasePCH.h"
#include "Frustrum3.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Frustrum3::Frustrum3()
{
}

// ***********************************************************************
Frustrum3::Frustrum3(const Matrix3D& wvpMatrix)
{
	SetMatrix(wvpMatrix);
}

// ***********************************************************************
Frustrum3::Frustrum3(const Frustrum3& input)
{
	Planes[0] = input.Planes[0];
	Planes[1] = input.Planes[1];
	Planes[2] = input.Planes[2];
	Planes[3] = input.Planes[3];
	Planes[4] = input.Planes[4];
	Planes[5] = input.Planes[5];
}

// ***********************************************************************
void Frustrum3::SetMatrix(const Matrix3D& wvpMatrix)
{
	Planes[0].Normal.X = -wvpMatrix.M13;
    Planes[0].Normal.Y = -wvpMatrix.M23;
    Planes[0].Normal.Z = -wvpMatrix.M33;
    Planes[0].D = -wvpMatrix.M43;
    Planes[1].Normal.X = -wvpMatrix.M14 + wvpMatrix.M13;
    Planes[1].Normal.Y = -wvpMatrix.M24 + wvpMatrix.M23;
    Planes[1].Normal.Z = -wvpMatrix.M34 + wvpMatrix.M33;
    Planes[1].D = -wvpMatrix.M44 + wvpMatrix.M43;

	Planes[2].Normal.X = -wvpMatrix.M14 - wvpMatrix.M11;
    Planes[2].Normal.Y = -wvpMatrix.M24 - wvpMatrix.M21;
    Planes[2].Normal.Z = -wvpMatrix.M34 - wvpMatrix.M31;
    Planes[2].D = -wvpMatrix.M44 - wvpMatrix.M41;
    Planes[3].Normal.X = -wvpMatrix.M14 + wvpMatrix.M11;
    Planes[3].Normal.Y = -wvpMatrix.M24 + wvpMatrix.M21;
    Planes[3].Normal.Z = -wvpMatrix.M34 + wvpMatrix.M31;
    Planes[3].D = -wvpMatrix.M44 + wvpMatrix.M41;

    Planes[4].Normal.X = -wvpMatrix.M14 + wvpMatrix.M12;
    Planes[4].Normal.Y = -wvpMatrix.M24 + wvpMatrix.M22;
    Planes[4].Normal.Z = -wvpMatrix.M34 + wvpMatrix.M32;
    Planes[4].D = -wvpMatrix.M44 + wvpMatrix.M42;
    Planes[5].Normal.X = -wvpMatrix.M14 - wvpMatrix.M12;
    Planes[5].Normal.Y = -wvpMatrix.M24 - wvpMatrix.M22;
    Planes[5].Normal.Z = -wvpMatrix.M34 - wvpMatrix.M32;
    Planes[5].D = -wvpMatrix.M44 - wvpMatrix.M42;
    
	// Normalize the planar normals
    for (int i = 0; i < 6; i++)
    {
        float length = Planes[i].Normal.Length();
        Planes[i].Normal = (Planes[i].Normal / length);
        Planes[i].D /= length;
    }
}


// ===================================================================================
// Containment Methods
// ===================================================================================

// ***********************************************************************
bool Frustrum3::Contains(const Sphere3& sphere) const
{
	// Loop through the planes
	for(int i=0; i<6; i++)
	{
		float distance = 
			Planes[i].Normal.X * sphere.Center.X +
			Planes[i].Normal.Y * sphere.Center.Y +
			Planes[i].Normal.Z * sphere.Center.Z +
			Planes[i].D;

		// Is there sphere outside of this plane?
		if(distance > sphere.Radius)
			return false;
	}

	return true;
}
