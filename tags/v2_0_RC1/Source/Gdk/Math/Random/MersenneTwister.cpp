/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "MersenneTwister.h"

using namespace Gdk;

// Defines for the Mersenne Twister
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

// *****************************************************************
/// @brief
///     Creates a new mersenne twister random number generator with a given seed
/// @param seed
///     The seed for the random number sequence.  
///     Using the same seed will generate the same sequence
// *****************************************************************
MersenneTwister::MersenneTwister(UInt32 seed)
{
	mti = MERSENNE_TWISTER_N + 1;

	// Seed this random generator
	Seed(seed);
}

// *****************************************************************
/// @brief
///     Re-seeds the random number sequence
// *****************************************************************
void MersenneTwister::Seed(UInt32 seed)
{
	mt[0] = seed & 0xffffffffUL;
    for (mti=1; mti<MERSENNE_TWISTER_N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        mt[mti] &= 0xffffffffUL;
    }
}

// *****************************************************************
/// @brief
///     Generates a random UInt32 value
// *****************************************************************
UInt32 MersenneTwister::GetUInt32()
{
    UInt32 y;
    static UInt32 mag01[2] = {0x0UL, MATRIX_A};
    
    if (mti >= MERSENNE_TWISTER_N) { /* generate N words at one time */
        int kk;

        for (kk=0;kk<MERSENNE_TWISTER_N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<MERSENNE_TWISTER_N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-MERSENNE_TWISTER_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[MERSENNE_TWISTER_N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[MERSENNE_TWISTER_N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

// *****************************************************************
/// @brief
///     Generates a random Int32 value
// *****************************************************************
Int32 MersenneTwister::GetInt32()
{
	UInt32 a = GetUInt32();
	Int32 result = (Int32)(a & 0x7FFFFFFF);
	if(a & 0x80000000)
		result = -result;
	return result;
}

// *****************************************************************
/// @brief
///     Generates a random number between two Int32 values, inclusively
/// @param min
///     The minimum possible value to be generated
/// @param max
///     The maximum possible value to be generated
// *****************************************************************
Int32 MersenneTwister::FromInterval(Int32 min, Int32 max)
{
	return (GetUInt32() % (max - min + 1)) + min;
}

// *****************************************************************
/// @brief
///     Generates a random float value between 0.0 and 1.0, inclusively
// *****************************************************************
float MersenneTwister::GetUnit()
{
	return (float)(GetUInt32() * (1.0 / 4294967295.0)); 
}

// *****************************************************************
/// @brief
///     Generates a random float value between -1.0 and 1.0, inclusively
// *****************************************************************
float MersenneTwister::GetSymmetric()
{
	return (float)(GetInt32() * (1.0 / 2147483648.0)); 
}

// *****************************************************************
/// @brief
///     Generates a random number between two float values, inclusively
/// @param min
///     The minimum possible value to be generated
/// @param max
///     The maximum possible value to be generated
// *****************************************************************
float MersenneTwister::FromInterval(float min, float max)
{
	return GetUnit() * (max - min) + min;
}



// License for the Mersenne Twister code
// ------------------------------------------------------------------------------
/* Commercial Use of Mersenne Twister
 
 2001/4/6
 
 Until 2001/4/6, MT had been distributed under GNU Public License, but after 2001/4/6, 
 we decided to let MT be used for any purpose, including commercial use. 2002-versions 
 mt19937ar.c, mt19937ar-cok.c are considered to be usable freely.

 Original source:
 http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
*/
