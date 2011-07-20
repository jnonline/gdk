/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	class Box2
	{
	public:
		// Components
		Vector2 Center;
		Vector2 Axis[2];
		float Extent[2];

		// Constructors
		Box2 ();
		Box2 (const Vector2& center, const Vector2& axis0, const Vector2& axis1, float extent0, float extent1);
		Box2 (const Box2& input);

		// Containment/Intersection Methods
		inline bool Contains(const Vector2& point) const;
		inline bool Intersects(const Box2& box) const;

		// special constants
		static const Box2 ZERO;    // (0,0)-(0,0)
	};

	// ===================================================================================
	// Containment & Intersection Methods
	// ===================================================================================

	// ***********************************************************************
	inline bool Box2::Contains(const Vector2& point) const
	{
		// Get the vector from the center of the box to the point
		Vector2 diff = point - this->Center;
		
		// Loop through the axis
		for(int i=0; i<2; i++)
		{
			// Get the distance from the point to the center along this axis
			float dist = diff.Dot(this->Axis[i]);

			// Is the point outside the extent of this axis?
			if(Math::Abs(dist) > this->Extent[i])
				return false;
		}

		return true;
	}

	// ***********************************************************************
	inline bool Box2::Intersects(const Box2& box) const
	{
		// Get the difference vector between the box centers
		Vector2 diff = box.Center - this->Center;

		// Axis C0+t*A0
		float absA0dotB0 = Math::Abs(this->Axis[0].Dot(box.Axis[0]));
		float absA0dotB1 = Math::Abs(this->Axis[0].Dot(box.Axis[1]));
		float absA0dotDiff = Math::Abs(this->Axis[0].Dot(diff));
		float sum = this->Extent[0] + box.Extent[0]*absA0dotB0 + box.Extent[1]*absA0dotB1;
		if (absA0dotDiff > sum)
		{
			return false;
		}

		// Axis C0+t*A1
		float absA1dotB0 = Math::Abs(this->Axis[1].Dot(box.Axis[0]));
		float absA1dotB1 = Math::Abs(this->Axis[1].Dot(box.Axis[1]));
		float absA1dotDiff = Math::Abs(this->Axis[1].Dot(diff));
		sum = this->Extent[1] + box.Extent[0]*absA1dotB0 + box.Extent[1]*absA1dotB1;
		if (absA1dotDiff > sum)
		{
			return false;
		}

		// Axis C0+t*B0
		float absB0dotDiff = Math::Abs(box.Axis[0].Dot(diff));
		sum = box.Extent[0] + this->Extent[0]*absA0dotB0 + this->Extent[1]*absA1dotB0;
		if (absB0dotDiff > sum)
		{
			return false;
		}

		// Axis C0+t*B1
		float absB1dotDiff = Math::Abs(box.Axis[1].Dot(diff));
		sum = box.Extent[1] + this->Extent[0]*absA0dotB1 + this->Extent[1]*absA1dotB1;
		if (absB1dotDiff > sum)
		{
			return false;
		}

		return true;
	}

} // namespace Gdk
