/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;

namespace Gdk.Framework
{
    static public class MathUtility
    {
        /// <summary>
        /// Static constructor
        /// </summary>
        static MathUtility()
        {
            // Init the perlin noise lookup table
            for (int i=0; i < 256; i++) 
            {
                noiseTable[256 + i] = noiseTable[i] = permutationTable[i]; 
            }

        }

        // -----------------------------------------------------------
        // Math Constants
        
        public const float Epsilon = 0.0001f;

        public const float TwoPi = (float)Math.PI * 2f;
        public const float PiOver2 = (float)Math.PI / 2f;
        public const float PiOver3 = (float)Math.PI / 3f;
        public const float PiOver4 = (float)Math.PI / 4f;

        /// <summary>
        /// Converts degrees to radians
        /// </summary>
        static public float DegreesToRadians(float degrees)
        {
            return (degrees / 180f) * (float)Math.PI;
        }

        /// <summary>
        /// Converts radians to degrees
        /// </summary>
        static public float RadiansToDegrees(float radians)
        {
            return (radians / (float)Math.PI) * 180f;
        }

        
        /// <summary>
        /// Linear interpolates between two values:  result = a + t(b-a)
        /// </summary>
        static public float Lerp(float t, float a, float b) 
        { 
            return a + t * (b - a); 
        }

        #region Perlin Noise

        /// <summary>
        /// Fade utility method for noise generation
        /// </summary>
        static private float Fade(float t) 
        { 
            return t * t * t * (t * (t * 6 - 15) + 10); 
        }

        /// <summary>
        /// Grad utility method for noise generation
        /// </summary>
        static private float Grad(int hash, float x, float y, float z) 
        {
            int h = hash & 15;                     // CONVERT LO 4 BITS OF HASH CODE
            float u = h<8 ? x : y;                 // INTO 12 GRADIENT DIRECTIONS.
            float v = h<4 ? y : h==12||h==14 ? x : z;
            return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
        }

        // Permutation tables
        static private int[] noiseTable = new int[512];
        static private int[] permutationTable = { 151,160,137,91,90,15,
           131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
           190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
           88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
           77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
           102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
           135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
           5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
           223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
           129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
           251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
           49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
           138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
           };

        /// <summary>
        /// Gets a perlin noise value at the given x,y,z coordinate.
        /// Adapted from: http://mrl.nyu.edu/~perlin/noise/
        /// 
        /// Original Source: COPYRIGHT 2002 KEN PERLIN
        /// </summary>
        static public float Noise(float x, float y, float z) 
        {
            int X = (int)Math.Floor(x) & 255;   // FIND UNIT CUBE THAT
            int Y = (int)Math.Floor(y) & 255;   // CONTAINS POINT.
            int Z = (int)Math.Floor(z) & 255;

            x -= (float)Math.Floor(x);                                // FIND RELATIVE X,Y,Z
            y -= (float)Math.Floor(y);                                // OF POINT IN CUBE.
            z -= (float)Math.Floor(z);

            float u = Fade(x);                                // COMPUTE FADE CURVES
            float v = Fade(y);                                // FOR EACH OF X,Y,Z.
            float w = Fade(z);

            int A = noiseTable[X  ]+Y, AA = noiseTable[A]+Z, AB = noiseTable[A+1]+Z;      // HASH COORDINATES OF
            int B = noiseTable[X+1]+Y, BA = noiseTable[B]+Z, BB = noiseTable[B+1]+Z;      // THE 8 CUBE CORNERS,

            return
                Lerp(w, Lerp(v, Lerp(u, Grad(noiseTable[AA], x, y, z),  // AND ADD
                                        Grad(noiseTable[BA  ], x-1, y  , z   )), // BLENDED
                                Lerp(u, Grad(noiseTable[AB], x, y - 1, z),  // RESULTS
                                        Grad(noiseTable[BB  ], x-1, y-1, z   ))),// FROM  8
                        Lerp(v, Lerp(u, Grad(noiseTable[AA + 1], x, y, z - 1),  // CORNERS
                                        Grad(noiseTable[BA+1], x-1, y  , z-1 )), // OF CUBE
                                Lerp(u, Grad(noiseTable[AB + 1], x, y - 1, z - 1),
                                        Grad(noiseTable[BB+1], x-1, y-1, z-1 ))));
        }

        /// <summary>
        /// Generates noise over several combined octaves
        /// </summary>
        static public float Noise(float x, float y, float z, int octaves, float persistence)
        {
            float total = 0f;
            float amplitude = 1f;
            float frequency = 1f;

            // Loop through the octaves
            for (int i = 0; i < octaves; i++)
            {
                total += Noise(x * frequency, y * frequency, z) * amplitude;
                amplitude *= persistence;
                frequency *= 2f;
            }

            return total;
        }

        #endregion
    }
}