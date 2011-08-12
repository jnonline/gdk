/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Plane3.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Plane3::Plane3()
{
}

// ***********************************************************************
Plane3::Plane3(const Vector3& normal, float d)
{
	Normal = normal;
	D = d;
}

// ***********************************************************************
Plane3::Plane3(const Vector3& point1, const Vector3& point2, const Vector3& point3)
{
	Vector3 v1 = point2 - point1;
	Vector3 v2 = point3 - point1;
	Normal = v1.Cross(v2);
	Normal.Normalize();
	D = Normal.Dot(point1);
}

// ***********************************************************************
Plane3::Plane3(const Vector3& normal, const Vector3& point)
{
	Normal = normal;
	D = Normal.Dot(point);
}

// ***********************************************************************
Plane3::Plane3(const Plane3& input)
{
	Normal = input.Normal;
	D = input.D;
}


// ===================================================================================
// Planar Methods
// ===================================================================================

// ***********************************************************************
int Plane3::WhichSide(const Vector3& point) const
{
	float distance = DistanceTo(point);

	if(distance < 0.0f)
		return -1;
	if(distance > 0.0f)
		return 1;
	return 0;
}

// ***********************************************************************
float Plane3::DistanceTo(const Vector3& point) const
{
	return Normal.Dot(point) - D;
}
