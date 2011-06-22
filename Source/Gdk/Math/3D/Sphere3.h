/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include "GdkLIB.h"

namespace Gdk
{

	class Sphere3
	{
	public:
		// Components
		Vector3 Center;
		float Radius;

		// Constructors
		Sphere3();
		Sphere3(float x, float y, float z, float radius);
		Sphere3(const Vector3& center, float radius);
		Sphere3(const Sphere3& input);

		// Asssignment Operator
		inline Sphere3& operator= (const Sphere3& input);

		// Comparison Operators
		inline bool operator== (const Sphere3& input) const;
		inline bool operator!= (const Sphere3& input) const;

		// Containment Methods
		inline bool Contains(const Vector3& point) const;

		// Encapsulation Methods
		static Sphere3 Merge(const Sphere3& sphere1, const Sphere3& sphere2);
	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Sphere3& Sphere3::operator= (const Sphere3& input)
	{
		Center = input.Center;
		Radius = input.Radius;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Sphere3::operator== (const Sphere3& input) const
	{
		return 
			Center == input.Center && Radius == input.Radius;
	}

	// ***********************************************************************
	inline bool Sphere3::operator!= (const Sphere3& input) const
	{
		return 
			Center != input.Center || Radius != input.Radius;
	}

	// ===================================================================================
	// Containment Methods
	// ===================================================================================

	// ***********************************************************************
	inline bool Sphere3::Contains(const Vector3& point) const
	{
		// Get the distance squared between the center & the point
		float distanceSquared = (this->Center - point).LengthSquared();
		return distanceSquared <= this->Radius * this->Radius;
	}

} // namespace
