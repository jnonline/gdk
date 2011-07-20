/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    // Notes:
    //  Return Value:   Noise* functions return a unit value from [0.0 to 1.0]
    //                  CompoundNoise* functions return a value from [0.0 to (1.0 + persistance * 2)]
    //
    //  Compound Noise: Multiple octaves of noise are combined doubleing the frequency, 
    //                  and scaling the amplitude by the persistance for each successive octave
    //
    //  Octaves:        The number of octaves to combine (Good values are 4-6)
    //  Persistance:    The amount of amplitude to apply for each successive octave (Good values are 0.3 (Smooth) to 0.7 (Rough))
    
	// =========================================================
	class PerlinNoise
	{
	public:
		
		// Public Methods
		// ---------------------------------------

        // 1D
        static float Noise1(float x);
        static float CompoundNoise1(float x, int octaves = 4, float persistence = 0.5f);
        
		// 2D
        static float Noise2(float x, float y);
        static float CompoundNoise2(float x, float y, int octaves = 4, float persistence = 0.5f);

        // 3D
        static float Noise3(float x, float y, float z);
        static float CompoundNoise3(float x, float y, float z, int octaves = 4, float persistence = 0.5f);
        
        // Init
        static void Init();
        
	private:
        
		// Internals
        // -----------------
        
        // Properties
        static UInt8 noiseTable[];
            
        // CTor
		PerlinNoise();
        
        // Inlines
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

} // namespace Gdk
