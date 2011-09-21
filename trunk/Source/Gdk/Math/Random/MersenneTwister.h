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
	/// @addtogroup Math
    /// @{
    /// @addtogroup Random
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Mersenne Twister based random number generator.
    // =================================================================================
    class MersenneTwister
	{
	public:
	    
		// Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
		MersenneTwister(UInt32 seed);
		void Seed(UInt32 seed);

		UInt32 GetUInt32();
		Int32 GetInt32();
		Int32 FromInterval(Int32 min, Int32 max);
		
        float GetUnit();
		float GetSymmetric();
		float FromInterval(float min, float max);
        
        /// @}

	private:
        
        // Public Propertiers
		// =====================================================
        
		UInt32 mt[MERSENNE_TWISTER_N];
		Int32 mti;
	};
    
    /// @}  Random
    /// @}  Math
    
} // namespace Gdk
