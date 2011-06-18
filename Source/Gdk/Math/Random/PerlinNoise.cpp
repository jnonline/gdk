
#include "BasePCH.h"
#include "PerlinNoise.h"
#include "Random.h"
#include "../Maths.h"

using namespace Gdk;


// The perlin noise generation code is adapted from the original source:
//  http://mrl.nyu.edu/~perlin/noise/
// 
// Original Source: COPYRIGHT 2002 KEN PERLIN


// ***********************************************************************
// instantiations of static constants

UInt8 PerlinNoise::noiseTable[512];

// ***********************************************************************
void PerlinNoise::Init()
{
    for(int i=0; i<256; i++)
    {
        noiseTable[i] = noiseTable[i+256] = Random::GetInt32() & 255;
    }
}

// ***********************************************************************
float PerlinNoise::Grad1(UInt8 hash, float x) 
{
    // Convert the low bit into 1 gradient directions
    return ((hash & 1) == 0) ? x : -x;	
}

// ***********************************************************************
float PerlinNoise::Noise1(float x) 
{
    int X = (int)Math::Floor(x) & 255;
    x -= (float)Math::Floor(x);
    float u = Fade(x);

    return
    Lerp(u, Grad1(noiseTable[X],   x), 
            Grad1(noiseTable[X+1], x-1));
}


// ***********************************************************************
float PerlinNoise::CompoundNoise1(float x, int octaves, float persistence)
{
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    
    // Loop through the octaves
    for (int i = 0; i < octaves; i++)
    {
        total += Noise1(x * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }
    
    return total;
}

// ***********************************************************************
float PerlinNoise::Grad2(UInt8 hash, float x, float y) 
{
    // Convert the low 2 bits into 4 gradient directions	
    return ( ((hash & 2) == 0) ? x : -x) + ( ((hash & 1) == 0) ? y : -y);	
}
        
// ***********************************************************************
float PerlinNoise::Noise2(float x, float y) 
{
    int X = (int)Math::Floor(x) & 255;  // FIND UNIT SQUARE THAT
    int Y = (int)Math::Floor(y) & 255;  // CONTAINS POINT.
    
    x -= (float)Math::Floor(x);         // FIND RELATIVE X,Y
    y -= (float)Math::Floor(y);         // OF POINT IN SQUARE.
    
    float u = Fade(x);                  // COMPUTE FADE CURVES
    float v = Fade(y);                  // FOR EACH OF X,Y.
    
    int A = noiseTable[X] + Y;
    int B = noiseTable[X+1] + Y;		
   
    return
    Lerp(v, Lerp(u, Grad2(noiseTable[A],   x,   y),
                    Grad2(noiseTable[B],   x-1, y)),
            Lerp(u, Grad2(noiseTable[A+1], x,   y-1),
                    Grad2(noiseTable[B+1], x-1, y-1)));
}


// ***********************************************************************
float PerlinNoise::CompoundNoise2(float x, float y, int octaves, float persistence)
{
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    
    // Loop through the octaves
    for (int i = 0; i < octaves; i++)
    {
        total += Noise2(x * frequency, y * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }
    
    return total;
}

// ***********************************************************************
float PerlinNoise::Grad3(UInt8 hash, float x, float y, float z) 
{
    int h = hash & 15;                     // CONVERT LO 4 BITS OF HASH CODE
    float u = h<8 ? x : y;                 // INTO 12 GRADIENT DIRECTIONS.
    float v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

// ***********************************************************************
float PerlinNoise::Noise3(float x, float y, float z) 
{
    int X = (int)Math::Floor(x) & 255;  // FIND UNIT CUBE THAT
    int Y = (int)Math::Floor(y) & 255;  // CONTAINS POINT.
    int Z = (int)Math::Floor(z) & 255;

    x -= (float)Math::Floor(x);         // FIND RELATIVE X,Y,Z
    y -= (float)Math::Floor(y);         // OF POINT IN CUBE.
    z -= (float)Math::Floor(z);
    
    float u = Fade(x);                  // COMPUTE FADE CURVES
    float v = Fade(y);                  // FOR EACH OF X,Y,Z.
    float w = Fade(z);
    
    int A = noiseTable[X  ]+Y, AA = noiseTable[A]+Z, AB = noiseTable[A+1]+Z;      // HASH COORDINATES OF
    int B = noiseTable[X+1]+Y, BA = noiseTable[B]+Z, BB = noiseTable[B+1]+Z;      // THE 8 CUBE CORNERS,
    
    return
    Lerp(w, Lerp(v, Lerp(u, Grad3(noiseTable[AA],   x,   y,   z),       // AND ADD
                            Grad3(noiseTable[BA],   x-1, y,   z)),      // BLENDED
                    Lerp(u, Grad3(noiseTable[AB],   x,   y-1, z),       // RESULTS
                            Grad3(noiseTable[BB],   x-1, y-1, z))),     // FROM  8
            Lerp(v, Lerp(u, Grad3(noiseTable[AA+1], x,   y,   z-1),     // CORNERS
                            Grad3(noiseTable[BA+1], x-1, y,   z-1)),    // OF CUBE
                    Lerp(u, Grad3(noiseTable[AB+1], x,   y-1, z-1),
                            Grad3(noiseTable[BB+1], x-1, y-1, z-1))));
}


// ***********************************************************************
float PerlinNoise::CompoundNoise3(float x, float y, float z, int octaves, float persistence)
{
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    
    // Loop through the octaves
    for (int i = 0; i < octaves; i++)
    {
        total += Noise3(x * frequency, y * frequency, z * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }
    
    return total;
}