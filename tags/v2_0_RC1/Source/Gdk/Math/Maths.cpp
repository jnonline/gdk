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

// *****************************************************************
/// @brief
///     Returns the sign of the given value.
/// @param input
///     The input value to determine the sign of.
/// @return
///     @li If (input > 0) the method returns (1.0)
///     @li If (input < 0) the method returns (-1.0)
///     @li If (input = 0) the method returns (0.0)
// *****************************************************************
float Math::Sign (float input)
{
	if (input > (float)0)
		return (float)1;

	if (input < (float)0)
		return -(float)1;

	return (float)0;
}

// *****************************************************************
/// @brief
///     Gets the absolute value of a float
// *****************************************************************
float Math::Abs (float input)
{
	return (float)fabs((double)input);
}

// *****************************************************************
/// @brief
///     Rounds the input up to the nearest whole number
// *****************************************************************
float Math::Ceil (float input)
{
	return (float)ceil((double)input);
}

// *****************************************************************
/// @brief
///     Rounds the input down to the nearest whole number
// *****************************************************************
float Math::Floor (float input)
{
	return (float)floor((double)input);
}

// *****************************************************************
/// @brief
///     Computes the remainder of a float division.
// *****************************************************************
float Math::Mod (float numerator, float denominator)
{
	return (float)fmod((double)numerator, (double)denominator);
}

// *****************************************************************
/// @brief
///     Computes the square of an input value
// *****************************************************************
float Math::Sqr (float input)
{
	return input * input;
}

// *****************************************************************
/// @brief
///     Computes the square root of an input value
// *****************************************************************
float Math::Sqrt (float input)
{
	return (float)sqrt((double)input);
}

// *****************************************************************
/// @brief
///     Computes a float value raised to a given exponent
// *****************************************************************
float Math::Pow (float base, float exponent)
{
	return (float)pow((double)base,(double)exponent);
}

// *****************************************************************
/// @brief
///     Computes e raised to the given input exponential
// *****************************************************************
float Math::Exp (float input)
{
	return (float)exp((double)input);
}

// *****************************************************************
/// @brief
///     Computes the natural logarithm of an input value
// *****************************************************************
float Math::Log (float input)
{
	return (float)log((double)input);
}

// *****************************************************************
/// @brief
///     Computes the base 2 logarithm of an input value
// *****************************************************************
float Math::Log2 (float input)
{
	return INV_LN_2 * (float)log((double)input);
}

// *****************************************************************
/// @brief
///     Computes the base 10 logarithm of an input value
// *****************************************************************
float Math::Log10 (float input)
{
	return INV_LN_10 * (float)log((double)input);
}

// *****************************************************************
/// @brief
///     Computes the sine of a value
// *****************************************************************
float Math::Sin(float input)
{
	return (float)sin((double)input);
}

// *****************************************************************
/// @brief
///     Computes the cosine of a value
// *****************************************************************
float Math::Cos (float input)
{
	return (float)cos((double)input);
}

// *****************************************************************
/// @brief
///     Computes the tangent of a value
// *****************************************************************
float Math::Tan (float input)
{
	return (float)tan((double)input);
}

// *****************************************************************
/// @brief
///     Computes the arc cosine of a value
// *****************************************************************
float Math::ACos (float input)
{
	if (input < -(float)1.0)
		return PI;

	if (input > (float)1.0)
		return (float)0.0;

	return (float)acos((double)input);
}

// *****************************************************************
/// @brief
///     Computes the arc sine of a value
// *****************************************************************
float Math::ASin (float input)
{
	if (input < -(float)1.0)
		return HALF_PI;

	if (input > (float)1.0)
		return -HALF_PI;

	return (float)asin((double)input);
}

// *****************************************************************
/// @brief
///     Computes the arc tangent of a value
/// @return
///     A radian value in the interval [-Pi/2,  Pi/2]
// *****************************************************************
float Math::ATan (float input)
{
	return (float)atan((double)input);
}

// *****************************************************************
/// @brief
///     Computes the arc tangent of y/x
/// @remarks
///     With both the x & y parameters, the method can calculate the quadrant
///     of the tangent.
/// @return
///     A radian value in the interval [-Pi,  Pi]
// *****************************************************************
float Math::ATan2 (float y, float x)
{
	return (float)atan2((double)y,(double)x);
}

// *****************************************************************
/// @brief
///     Determines if an integer value is a direct Power of Two
/// @remarks
///     Returns true if the value is in the set: [1, 2, 4, 8, 16, 32, 64, ..., 2^31]
// *****************************************************************
bool Math::IsPowerOfTwo(UInt32 value)
{
    return (value & (value - 1)) == 0;
}

