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

// ***********************************************************************
Box3::Box3()
{
}

// ***********************************************************************
Box3::Box3(const Vector3& center, const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, float extent0, float extent1, float extent2)
{
	this->Center = center;
	this->Axis[0] = axis0;
	this->Axis[1] = axis1;
	this->Axis[2] = axis2;
	this->Extent[0] = extent0;
	this->Extent[1] = extent1;
	this->Extent[2] = extent2;
}

// ***********************************************************************
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
