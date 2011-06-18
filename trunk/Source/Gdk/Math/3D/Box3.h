
#pragma once

#include "GdkLIB.h"

namespace Gdk 
{

	class Box3
	{
	public:
		// Components
		Vector3 Center;
		Vector3 Axis[3];
		float Extent[3];

		// Constructors
		Box3 ();
		Box3 (const Vector3& center, const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, float extent0, float extent1, float extent2);
		Box3 (const Box3& input);

		// Asssignment Operator
		inline Box3& operator= (const Box3& input);

		// Comparison Operators
		inline bool operator== (const Box3& input) const;
		inline bool operator!= (const Box3& input) const;

		// Containment Methods
		inline bool Contains(const Vector3& point) const;
	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Box3& Box3::operator= (const Box3& input)
	{
		Axis[0] = input.Axis[0];
		Axis[1] = input.Axis[1];
		Axis[2] = input.Axis[2];
		Extent[0] = input.Extent[0];
		Extent[1] = input.Extent[1];
		Extent[2] = input.Extent[2];
		Center = input.Center;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Box3::operator== (const Box3& input) const
	{
		return Axis[0] == input.Axis[0] 
			&& Axis[1] == input.Axis[1] 
			&& Axis[2] == input.Axis[2]
			&& Extent[0] == input.Extent[0] 
			&& Extent[1] == input.Extent[1] 
			&& Extent[2] == input.Extent[2]
			&& Center == Center;
	}

	// ***********************************************************************
	inline bool Box3::operator!= (const Box3& input) const
	{
		return Axis[0] != input.Axis[0] 
			|| Axis[1] != input.Axis[1] 
			|| Axis[2] != input.Axis[2]
			|| Extent[0] != input.Extent[0] 
			|| Extent[1] != input.Extent[1] 
			|| Extent[2] != input.Extent[2]
			|| Center != Center;
	}

	// ===================================================================================
	// Containment Methods
	// ===================================================================================

	// ***********************************************************************
	inline bool Box3::Contains(const Vector3& point) const
	{
		// Get the vector from the center of the box to the point
		Vector3 diff = point - this->Center;
		
		// Loop through the axis
		for(int i=0; i<3; i++)
		{
			// Get the distance from the point to the center along this axis
			float dist = diff.Dot(this->Axis[i]);

			// Is the point outside the extent of this axis?
			if(Math::Abs(dist) > this->Extent[i])
				return false;
		}

		return true;
	}


} // namespace

