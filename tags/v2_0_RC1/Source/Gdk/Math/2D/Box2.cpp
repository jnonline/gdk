/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Box2.h"

using namespace Gdk;

// instantiations of static constants
const Box2 Box2::ZERO(Vector2::ZERO, Vector2::ZERO, Vector2::ZERO, 0.0f, 0.0f);
const Box2 Box2::UNIT(Vector2::ZERO, Vector2::UNIT_X, Vector2::UNIT_Y, 0.5f, 0.5f);

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Box2::Box2()
{
}

// *****************************************************************
/// @brief
///     Constructs a new oriented box from a center and two extent axes
/// @param center
///     Center of the new box
/// @param axis1
///     1st orthoganol axis of the box
/// @param axis2
///     2nd orthoganol axis of the box
/// @param extent1
///     The size of the box along the 1st axis
/// @param extent2
///     The size of the box along the 2nd axis
// *****************************************************************
Box2::Box2(const Vector2& center, const Vector2& axis1, const Vector2& axis2, float extent1, float extent2)
{
	this->Center = center;
	this->Axis[0] = axis1;
	this->Axis[1] = axis2;
	this->Extent[0] = extent1;
	this->Extent[1] = extent2;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Box2::Box2(const Box2& input)
{
	this->Center = input.Center;
	this->Axis[0] = input.Axis[0];
	this->Axis[1] = input.Axis[1];
	this->Extent[0] = input.Extent[0];
	this->Extent[1] = input.Extent[1];
}
