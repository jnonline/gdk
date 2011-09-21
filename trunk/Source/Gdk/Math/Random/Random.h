/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



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
	/// @addtogroup Math
    /// @{
    /// @addtogroup Random
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Exposes simple static methods for generating random numbers
    // =================================================================================
	class Random
	{
	public:
		
        // Public Methods
		// =====================================================
        
        /// The generator mechanism used to generate random numbers
        static class MersenneTwister Generator;
        
		// Public Methods
		// =====================================================

        // ---------------------------------
        /// @name Methods
        /// @{
        
		static void Seed(UInt32 seed);

		static UInt32 GetUInt32();
		static Int32 GetInt32();
		static Int32 FromInterval(Int32 min, Int32 max);

		static float GetUnit();
		static float GetSymmetric();
		static float FromInterval(float min, float max);

        /// @}
        
	private:
		// Internals
		Random();
	};

    /// @}  Math
    /// @}  Random
    
} // namespace Gdk
