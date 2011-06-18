
#include "BasePCH.h"
#include "Box2.h"

using namespace Gdk;

// instantiations of static constants
const Box2 Box2::ZERO(Vector2::ZERO, Vector2::ZERO, Vector2::ZERO, 0.0f, 0.0f);

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Box2::Box2()
{
}

// ***********************************************************************
Box2::Box2(const Vector2& center, const Vector2& axis0, const Vector2& axis1, float extent0, float extent1)
{
	this->Center = center;
	this->Axis[0] = axis0;
	this->Axis[1] = axis1;
	this->Extent[0] = extent0;
	this->Extent[1] = extent1;
}

// ***********************************************************************
Box2::Box2(const Box2& input)
{
	this->Center = input.Center;
	this->Axis[0] = input.Axis[0];
	this->Axis[1] = input.Axis[1];
	this->Extent[0] = input.Extent[0];
	this->Extent[1] = input.Extent[1];
}
