/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	class Ray3
	{
	public:
		// Components
		Vector3 Origin;
		Vector3 Direction;

		// Constructors
		Ray3();
		Ray3(const Vector3& origin, const Vector3& direction);
		Ray3(const Ray3& input);

		// Asssignment Operator
		inline Ray3& operator= (const Ray3& input);

	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Ray3& Ray3::operator= (const Ray3& input)
	{
		Origin = input.Origin;
		Direction = input.Direction;
		return *this;
	}


} // namespace
