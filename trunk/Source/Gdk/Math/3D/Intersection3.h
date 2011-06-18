#pragma once

#include "GdkLIB.h"

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
