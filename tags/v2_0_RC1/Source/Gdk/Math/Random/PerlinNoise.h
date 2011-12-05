/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{    
	/// @addtogroup Math
    /// @{
    /// @addtogroup Random
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Provides static methods for generating perlin noise in 1, 2, and 3 dimensions     
    /// @remarks
    ///     Noise values are generated in the symmetric -1.0 to 1.0 range.
    /// @par
    ///     The noise values are generated on whole number coordinate boundarys and any 
    ///     fractional component is smoothly interpolated.
    /// @par
    ///     The noise generator methods will begin to repeat after 256 on the input coordinate(s)
    // =================================================================================

	class PerlinNoise
	{
	public:
		
		// Public Methods
		// =====================================================

        // ---------------------------------
        /// @name Methods
        /// @{
        
        static float Noise1(float x);
        static float CompoundNoise1(float x, int octaves = 4, float persistence = 0.5f);
        
        static float Noise2(float x, float y);
        static float CompoundNoise2(float x, float y, int octaves = 4, float persistence = 0.5f);

        static float Noise3(float x, float y, float z);
        static float CompoundNoise3(float x, float y, float z, int octaves = 4, float persistence = 0.5f);
        
        /// @}
        
    public:
        
        // INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
        
        static void Init();
        
        /// @}
        
        /// @endcond
        
	private:
        
		// Private Properties
		// =====================================================        
        
        static UInt8 noiseTable[];
        
        // Internal Methods
		// =====================================================
        
        static inline float Lerp(float t, float a, float b) 
        {
            return a + t * (b - a);
        }
        
        static inline float Fade(float t)
        {
            return t * t * t * (t * (t * 6 - 15) + 10); 
        }
        
        static float Grad1(UInt8 hash, float x);
        static float Grad2(UInt8 hash, float x, float y);
        static float Grad3(UInt8 hash, float x, float y, float z);
        
	};

    /// @}  Random
    /// @}  Math
    
} // namespace Gdk
