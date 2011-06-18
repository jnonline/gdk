#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Frustrum3
	{
	public:
		// Components
		Plane3 Planes[6];

		// Constructors
		Frustrum3();
		Frustrum3(const Matrix3D& wvpMatrix);
		Frustrum3(const Frustrum3& input);

		// Asssignment Operator
		inline Frustrum3& operator= (const Frustrum3& input);

		// Utilities
		void SetMatrix(const Matrix3D& wvpMatrix);

		// Containment
		bool Contains(const Sphere3& sphere) const;
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

} // namespace

