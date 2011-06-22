/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Plane3
	{
	public:
		// Components
		Vector3 Normal;
		float D;

		// Constructors
		Plane3();
		Plane3(const Vector3& normal, float d);
		Plane3(const Vector3& point1, const Vector3& point2, const Vector3& point3);	// Create a plane containing 3 points
		Plane3(const Vector3& normal, const Vector3& point);								// Create a plane from a normal & a point on the plane
		Plane3(const Plane3& input);

		// Asssignment Operator
		inline Plane3& operator= (const Plane3& input);

		// Planar Methods
		int WhichSide(const Vector3& point) const;		// 1 = front, -1 = backside, 0 = on plane
		float DistanceTo(const Vector3& point) const;	// Shortest distance from the point to the plane
	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Plane3& Plane3::operator= (const Plane3& input)
	{
		Normal = input.Normal;
		D = input.D;
		return *this;
	}

} // namespace
