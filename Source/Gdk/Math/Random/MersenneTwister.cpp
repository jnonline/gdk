
#include "BasePCH.h"
#include "MersenneTwister.h"

using namespace Gdk;

// Defines for the Mersenne Twister
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

// ***********************************************************************
MersenneTwister::MersenneTwister(UInt32 seed)
{
	mti = MERSENNE_TWISTER_N + 1;

	// Seed this random generator
	Seed(seed);
}

// ***********************************************************************
void MersenneTwister::Seed(UInt32 seed)
{
	mt[0] = seed & 0xffffffffUL;
    for (mti=1; mti<MERSENNE_TWISTER_N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        mt[mti] &= 0xffffffffUL;
    }
}

// ***********************************************************************
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

// ***********************************************************************
Int32 MersenneTwister::GetInt32()
{
	UInt32 a = GetUInt32();
	Int32 result = (Int32)(a & 0x7FFFFFFF);
	if(a & 0x80000000)
		result = -result;
	return result;
}

// ***********************************************************************
Int32 MersenneTwister::FromInterval(Int32 min, Int32 max)
{
	return (GetUInt32() % (max - min + 1)) + min;
}

// ***********************************************************************
float MersenneTwister::GetUnit()
{
	return (float)(GetUInt32() * (1.0 / 4294967295.0)); 
}

// ***********************************************************************
float MersenneTwister::GetSymmetric()
{
	return (float)(GetInt32() * (1.0 / 2147483648.0)); 
}

// ***********************************************************************
float MersenneTwister::FromInterval(float min, float max)
{
	return GetUnit() * (max - min) + min;
}




// ------------------------------------------------------------------------------
/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/
