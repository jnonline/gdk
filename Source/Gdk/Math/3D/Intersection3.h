/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	class Intersection3
	{
	public:
		// Propeties
		// -------------------------------------------------

	protected:
		// Protected Methods
		// -------------------------------------------------

		// Constructors
		Intersection3();

	public:
		// Intersection Test/Find methods
		// ------------------------------------------------

		// Ray -> Plane
		static bool Test(const Ray3& ray, const Plane3& plane);
		static bool Find(const Ray3& ray, const Plane3& box, Vector3& intersectionPoint);

		// Ray -> Sphere
		static bool Test(const Ray3& ray, const Sphere3& sphere);
		static bool Find(const Ray3& ray, const Sphere3& sphere, Vector3& intersectionPoint);

	};

} // namespace
