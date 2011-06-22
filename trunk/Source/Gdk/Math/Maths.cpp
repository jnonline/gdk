/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Maths.h"

using namespace Gdk;

// instantiations of static constants
const float Math::EPSILON = FLT_EPSILON;
const float Math::ZERO_TOLERANCE = 1e-06f;
const float Math::PI = (float)(4.0*atan(1.0));
const float Math::TWO_PI = 2.0f*Math::PI;
const float Math::HALF_PI = 0.5f*Math::PI;
const float Math::INV_PI = 1.0f/Math::PI;
const float Math::INV_TWO_PI = 1.0f/Math::TWO_PI;
const float Math::DEG_TO_RAD = Math::PI/180.0f;
const float Math::RAD_TO_DEG = 180.0f/Math::PI;
const float Math::LN_2 = Math::Log(2.0f);
const float Math::LN_10 = Math::Log(10.0f);
const float Math::INV_LN_2 = 1.0f/Math::LN_2;
const float Math::INV_LN_10 = 1.0f/Math::LN_10;

// ***********************************************************************
float Math::Sign (float fValue)
{
	if (fValue > (float)0)
		return (float)1;

	if (fValue < (float)0)
		return -(float)1;

	return (float)0;
}

// ***********************************************************************
float Math::Abs (float input)
{
	return (float)fabs((double)input);
}

// ***********************************************************************
float Math::Ceil (float input)
{
	return (float)ceil((double)input);
}

// ***********************************************************************
float Math::Floor (float input)
{
	return (float)floor((double)input);
}

// ***********************************************************************
float Math::Mod (float numerator, float denominator)
{
	return (float)fmod((double)numerator, (double)denominator);
}

// ***********************************************************************
float Math::Sqr (float input)
{
	return input * input;
}

// ***********************************************************************
float Math::Sqrt (float input)
{
	return (float)sqrt((double)input);
}

// ***********************************************************************
float Math::Pow (float base, float exponent)
{
	return (float)pow((double)base,(double)exponent);
}

// ***********************************************************************
float Math::Exp (float input)
{
	return (float)exp((double)input);
}

// ***********************************************************************
float Math::Log (float input)
{
	return (float)log((double)input);
}

// ***********************************************************************
float Math::Log2 (float input)
{
	return INV_LN_2 * (float)log((double)input);
}

// ***********************************************************************
float Math::Log10 (float input)
{
	return INV_LN_10 * (float)log((double)input);
}

// ***********************************************************************
float Math::Sin(float input)
{
	return (float)sin((double)input);
}

// ***********************************************************************
float Math::Cos (float input)
{
	return (float)cos((double)input);
}

// ***********************************************************************
float Math::Tan (float input)
{
	return (float)tan((double)input);
}

// ***********************************************************************
float Math::ACos (float input)
{
	if (input < -(float)1.0)
		return PI;

	if (input > (float)1.0)
		return (float)0.0;

	return (float)acos((double)input);
}

// ***********************************************************************
float Math::ASin (float input)
{
	if (input < -(float)1.0)
		return HALF_PI;

	if (input > (float)1.0)
		return -HALF_PI;

	return (float)asin((double)input);
}

// ***********************************************************************
float Math::ATan (float input)
{
	return (float)atan((double)input);
}

// ***********************************************************************
float Math::ATan2 (float y, float x)
{
	return (float)atan2((double)y,(double)x);
}

// ***********************************************************************
bool Math::IsPowerOfTwo(UInt32 value)
{
    return (value & (value - 1)) == 0;
}

