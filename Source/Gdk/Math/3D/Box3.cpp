/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Box3.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Box3::Box3()
{
}

// *****************************************************************
/// @brief
///     Constructs a new oriented box from a center and three extent axes
/// @param center
///     Center of the new box
/// @param axis1
///     1st orthoganol axis of the box
/// @param axis2
///     2nd orthoganol axis of the box
/// @param axis3
///     3rd orthoganol axis of the box
/// @param extent1
///     The size of the box along the 1st axis
/// @param extent2
///     The size of the box along the 2nd axis
/// @param extent3
///     The size of the box along the 3rd axis
// *****************************************************************
Box3::Box3(const Vector3& center, const Vector3& axis1, const Vector3& axis2, const Vector3& axis3, float extent1, float extent2, float extent3)
{
	this->Center = center;
	this->Axis[0] = axis1;
	this->Axis[1] = axis2;
	this->Axis[2] = axis3;
	this->Extent[0] = extent1;
	this->Extent[1] = extent2;
	this->Extent[2] = extent3;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Box3::Box3(const Box3& input)
{
	this->Center = input.Center;
	this->Axis[0] = input.Axis[0];
	this->Axis[1] = input.Axis[1];
	this->Axis[2] = input.Axis[2];
	this->Extent[0] = input.Extent[0];
	this->Extent[1] = input.Extent[1];
	this->Extent[2] = input.Extent[2];
}
