/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Frustrum3.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Frustrum3::Frustrum3(const Matrix3D& matrix)
{
    SetMatrix(matrix);
}

// ***********************************************************************
Frustrum3::Frustrum3(const Frustrum3& input)
{
    // Copy the planes
    for(int i=0; i<6; i++)  
        Planes[i] = input.Planes[i];
    
    // Copy the corners
    for(int i=0; i<8; i++)  
        Corners[i] = input.Corners[i];
}

// ***********************************************************************
void Frustrum3::SetMatrix(const Matrix3D& matrix)
{
	Planes[0].Normal.X = -matrix.M13;
    Planes[0].Normal.Y = -matrix.M23;
    Planes[0].Normal.Z = -matrix.M33;
    Planes[0].D = -matrix.M43;
    Planes[1].Normal.X = -matrix.M14 + matrix.M13;
    Planes[1].Normal.Y = -matrix.M24 + matrix.M23;
    Planes[1].Normal.Z = -matrix.M34 + matrix.M33;
    Planes[1].D = -matrix.M44 + matrix.M43;

	Planes[2].Normal.X = -matrix.M14 - matrix.M11;
    Planes[2].Normal.Y = -matrix.M24 - matrix.M21;
    Planes[2].Normal.Z = -matrix.M34 - matrix.M31;
    Planes[2].D = -matrix.M44 - matrix.M41;
    Planes[3].Normal.X = -matrix.M14 + matrix.M11;
    Planes[3].Normal.Y = -matrix.M24 + matrix.M21;
    Planes[3].Normal.Z = -matrix.M34 + matrix.M31;
    Planes[3].D = -matrix.M44 + matrix.M41;

    Planes[4].Normal.X = -matrix.M14 + matrix.M12;
    Planes[4].Normal.Y = -matrix.M24 + matrix.M22;
    Planes[4].Normal.Z = -matrix.M34 + matrix.M32;
    Planes[4].D = -matrix.M44 + matrix.M42;
    Planes[5].Normal.X = -matrix.M14 - matrix.M12;
    Planes[5].Normal.Y = -matrix.M24 - matrix.M22;
    Planes[5].Normal.Z = -matrix.M34 - matrix.M32;
    Planes[5].D = -matrix.M44 - matrix.M42;
    
	// Normalize the planar normals
    for (int i = 0; i < 6; i++)
    {
        float length = Planes[i].Normal.Length();
        Planes[i].Normal = (Planes[i].Normal / length);
        Planes[i].D /= length;
    }
    
    // Calculate the 8 corners
    Ray3 ray = GetIntersectionRay(Planes[0], Planes[2]);
    Corners[0] = GetIntersectionPoint(Planes[4], ray);
    Corners[3] = GetIntersectionPoint(Planes[5], ray);
    ray = GetIntersectionRay(Planes[3], Planes[0]);
    Corners[1] = GetIntersectionPoint(Planes[4], ray);
    Corners[2] = GetIntersectionPoint(Planes[5], ray);
    ray = GetIntersectionRay(Planes[2], Planes[1]);
    Corners[4] = GetIntersectionPoint(Planes[4], ray);
    Corners[7] = GetIntersectionPoint(Planes[5], ray);
    ray = GetIntersectionRay(Planes[1], Planes[3]);
    Corners[5] = GetIntersectionPoint(Planes[4], ray);
    Corners[6] = GetIntersectionPoint(Planes[5], ray);
}


// ***********************************************************************
Ray3 Frustrum3::GetIntersectionRay(const Plane3& plane1, const Plane3& plane2)
{
    // Calculate a ray on the intersection of the given planes
    Vector3 direction = plane1.Normal.Cross(plane2.Normal);
    float lenSq = direction.LengthSquared();
    Vector3 temp = (-plane1.D * plane2.Normal) + (plane2.D * plane1.Normal);
    Vector3 position = temp.Cross(direction) / lenSq;
    return Ray3(position, direction);
}

// ***********************************************************************
Vector3 Frustrum3::GetIntersectionPoint(const Plane3& plane, const Ray3& ray)
{
    // Get the point where the given ray intersects the plane
    float t = (-plane.D - plane.Normal.Dot(ray.Origin)) / plane.Normal.Dot(ray.Direction);
    return ray.Origin + ray.Direction * t;
}



