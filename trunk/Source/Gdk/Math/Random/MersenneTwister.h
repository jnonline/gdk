/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



// -------------------------------------------
// Usage:
//
// Create an instance
//		MersenneTwister twister(12345);
//		
// Get Values
//	twister.FromInterval(0, 100);
//	twister.FromInterval(1.0f, 2.5f);
//


// Mersenne Twister constants
#define MERSENNE_TWISTER_N 624

namespace Gdk
{
	// =========================================================
	class MersenneTwister
	{
	public:
	    
		// Instance Methods
		// ---------------------------------------

		// CTor
		MersenneTwister(UInt32 seed);

		// Seed the random
		void Seed(UInt32 seed);

		// Integer Methods
		UInt32 GetUInt32();
		Int32 GetInt32();
		Int32 FromInterval(Int32 min, Int32 max);

		// Floating Point Methods
		float GetUnit();							// [ 0, 1]
		float GetSymmetric();						// [-1, 1]
		float FromInterval(float min, float max);	// [min, max]

	private:
		UInt32 mt[MERSENNE_TWISTER_N];
		Int32 mti;
	};
} // namespace Gdk
