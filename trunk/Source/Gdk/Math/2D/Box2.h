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

		// Containment Methods
        inline bool Contains(float x, float y) const;
		inline bool Contains(const Vector2& point) const;

		// special constants
		static const Box2 ZERO;    // (0,0)-(0,0)
	};

	// ===================================================================================
	// Containment & Intersection Methods
	// ===================================================================================

	// ***********************************************************************
	inline bool Box2::Contains(float x, float y) const
	{
		// Get the vector from the center of the box to the point
		Vector2 diff(x - this->Center.X, y - this->Center.Y);
		
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
	inline bool Box2::Contains(const Vector2& point) const
	{
        return Contains(point.X, point.Y);
    }

} // namespace Gdk
