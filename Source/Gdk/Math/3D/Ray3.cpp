
#include "BasePCH.h"
#include "Ray3.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Ray3::Ray3()
{
}

// ***********************************************************************
Ray3::Ray3(const Vector3& origin, const Vector3& direction)
{
	Origin = origin;
	Direction = direction;
}

// ***********************************************************************
Ray3::Ray3(const Ray3& input)
{
	Origin = input.Origin;
	Direction = input.Direction;
}



