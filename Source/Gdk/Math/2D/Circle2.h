/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk 
{

	class Circle2
	{
	public:
		// Components
		Vector2 Center;
		float Radius;

		// Constructors
		Circle2();
		Circle2(float x, float y, float radius);
		Circle2(const Vector2& center, float radius);
		Circle2(const Circle2& input);

		// Asssignment Operator
		inline Circle2& operator= (const Circle2& input);

		// Comparison Operators
		inline bool operator== (const Circle2& input) const;
		inline bool operator!= (const Circle2& input) const;

		// Containment/Intersection Methods
		inline bool Contains(float x, float y) const;
		inline bool Contains(const Vector2& point) const;
		inline bool Intersects(const Circle2& input) const;

		// Utility Methods
		Vector2 GetParametricPointInArea(float tAngle, float tRadius);
		Vector2 GetParametricPointOnPerimeter(float tAngle);

		// special constants
		static const Circle2 ZERO;    // Center(0,0) Radius(0)
	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Circle2& Circle2::operator= (const Circle2& input)
	{
		Center = input.Center;
		Radius = input.Radius;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Circle2::operator== (const Circle2& input) const
	{
		return 
			Center == input.Center && Radius == input.Radius;
	}

	// ***********************************************************************
	inline bool Circle2::operator!= (const Circle2& input) const
	{
		return 
			Center != input.Center || Radius != input.Radius;
	}

	// ===================================================================================
	// Containment & Intersection Methods
	// ===================================================================================

	// ***********************************************************************
	inline bool Circle2::Contains(float x, float y) const
	{
		// Get the distance squared between the circle center & the point
		float xDiff = x - this->Center.X;
		float yDiff = y - this->Center.Y;
		float distanceSquared = xDiff * xDiff + yDiff * yDiff;
		return distanceSquared <= this->Radius * this->Radius;
	}

	// ***********************************************************************
	inline bool Circle2::Contains(const Vector2& point) const
	{
		// Get the distance squared between the circle center & the point
		float distanceSquared = (this->Center - point).LengthSquared();
		return distanceSquared <= this->Radius * this->Radius;
	}

	// ***********************************************************************
	inline bool Circle2::Intersects(const Circle2& input) const
	{
		// Get the distance (squared) between the two circle centers 
		float distanceSquared = (this->Center - input.Center).LengthSquared();

		// Get the total radii of the circles
		float totalRadii = this->Radius + input.Radius;
		
		return distanceSquared <= totalRadii * totalRadii;
	}

} // namespace
