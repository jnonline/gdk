/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Random.h"
#include "MersenneTwister.h"

using namespace Gdk;

// instantiations of static constants
MersenneTwister Random::Generator(0x19791979);

// *****************************************************************
/// @brief
///     Re-seeds the random number sequence
// *****************************************************************
void Random::Seed(UInt32 seed)
{
	Generator.Seed(seed);
}

// *****************************************************************
/// @brief
///     Generates a random UInt32 value
// *****************************************************************

UInt32 Random::GetUInt32()
{
    return Generator.GetUInt32();
}

// *****************************************************************
/// @brief
///     Generates a random Int32 value
// *****************************************************************

Int32 Random::GetInt32()
{
	return Generator.GetInt32();
}

// *****************************************************************
/// @brief
///     Generates a random number between two Int32 values, inclusively
/// @param min
///     The minimum possible value to be generated
/// @param max
///     The maximum possible value to be generated
// *****************************************************************
Int32 Random::FromInterval(Int32 min, Int32 max)
{
	return Generator.FromInterval(min, max);
}

// *****************************************************************
/// @brief
///     Generates a random float value between 0.0 and 1.0, inclusively
// *****************************************************************
float Random::GetUnit()
{
	return Generator.GetUnit();
}

// *****************************************************************
/// @brief
///     Generates a random float value between -1.0 and 1.0, inclusively
// *****************************************************************
float Random::GetSymmetric()
{
	return Generator.GetSymmetric();
}

// *****************************************************************
/// @brief
///     Generates a random number between two float values, inclusively
/// @param min
///     The minimum possible value to be generated
/// @param max
///     The maximum possible value to be generated
// *****************************************************************
float Random::FromInterval(float min, float max)
{
	return Generator.FromInterval(min, max);
}