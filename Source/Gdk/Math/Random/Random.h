/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

// -------------------------------------------
// Usage:
//
// Get values
//		Random::FromInterval(0, 100);
//		Random::FromInterval(1.0f, 2.5f);
//		Random::GetSymmetric()
//		Random::GetUnit()
//


namespace Gdk
{
	// =========================================================
	class Random
	{
	public:
		
		// Static Methods
		// ---------------------------------------

		// Seeding
		static void Seed(UInt32 seed);

		// Integer Methods
		static UInt32 GetUInt32();
		static Int32 GetInt32();
		static Int32 FromInterval(Int32 min, Int32 max);

		// Floating Point Methods
		static float GetUnit();								// [ 0, 1]
		static float GetSymmetric();						// [-1, 1]
		static float FromInterval(float min, float max);	// [min, max]

		// The Generator
		static class MersenneTwister Generator;

	private:
		// Internals
		Random();
	};

} // namespace Gdk
