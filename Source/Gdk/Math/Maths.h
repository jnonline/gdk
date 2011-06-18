
#ifndef _GDK_MATH_H
#define _GDK_MATH_H

#include "GdkLIB.h"

// NOTE: this file is named Maths.h instead of Math.h, so the GCC / XCode compilers wont confuse it with the ansi-C math.h

namespace Gdk
{

	class Math
	{
	public:
	    
		// Simple Math functions
		static float Sign (float input);
		static float Abs (float input);
		static float Ceil (float input);
		static float Floor (float input);
		static float Mod (float numerator, float denominator);
		
		static inline float Max(float a, float b);
		static inline int Max(int a, int b);
		static inline unsigned char Max(unsigned char a, unsigned char b);
		static inline float Min(float a, float b);
		static inline int Min(int a, int b);
		static inline unsigned char Min(unsigned char a, unsigned char b);
		static inline float Clamp(float in, float min, float max);
        
        static bool IsPowerOfTwo(UInt32 value);

		// Exponential functions
		static float Sqr (float input);
		static float Sqrt (float input);
		static float Pow (float base, float exponent);
		static float Exp (float input);    
		static float Log (float input);
		static float Log2 (float input);
		static float Log10 (float input);

		// Trigonometry functions
		static float Cos (float input);
		static float Sin (float input);
		static float Tan (float input);
		static float ACos (float input);
		static float ASin (float input);
		static float ATan (float input);
		static float ATan2 (float y, float x);

		// Linear interpolation
		static inline float Lerp (float start, float end, float t);
		static inline int Lerp (int start, int end, float t);
		static inline unsigned char Lerp (unsigned char start, unsigned char end, float t);
		static inline float LerpTable(float values[], float intervals[], float t);
		static inline float ReverseLerp(float start, float end, float value);

		// common constants
		static const float EPSILON;
		static const float ZERO_TOLERANCE;
		static const float MAX_float;
		static const float PI;
		static const float TWO_PI;
		static const float HALF_PI;
		static const float INV_PI;
		static const float INV_TWO_PI;
		static const float DEG_TO_RAD;
		static const float RAD_TO_DEG;
		static const float LN_2;
		static const float LN_10;
		static const float INV_LN_2;
		static const float INV_LN_10;
	};

	// Inline Implementations
	// ====================================

	// ***********************************************************************
	inline float Math::Max (float a, float b)
	{
		return a > b ? a : b;
	}

	// ***********************************************************************
	inline int Math::Max (int a, int b)
	{
		return a > b ? a : b;
	}

	// ***********************************************************************
	inline unsigned char Math::Max (unsigned char a, unsigned char b)
	{
		return a > b ? a : b;
	}

	// ***********************************************************************
	inline float Math::Min (float a, float b)
	{
		return a < b ? a : b;
	}

	// ***********************************************************************
	inline int Math::Min (int a, int b)
	{
		return a < b ? a : b;
	}

	// ***********************************************************************
	inline unsigned char Math::Min (unsigned char a, unsigned char b)
	{
		return a < b ? a : b;
	}
	// ***********************************************************************
	inline float Math::Lerp (float start, float end, float t)
	{
		return start + t * (end - start);
	}

	// ***********************************************************************
	inline int Math::Lerp (int start, int end, float t)
	{
		return (int)(start + t * (end - start));
	}

	// ***********************************************************************
	inline unsigned char Math::Lerp(unsigned char start, unsigned char end, float t)
	{
		return (unsigned char)(start + t * (end - start));
	}

	// ***********************************************************************
	inline float Math::ReverseLerp(float start, float end, float value)
	{
		return (value - start) / (end - start);
	}

	// ***********************************************************************
	inline float Math::Clamp(float in, float min, float max)
	{
		if(in < min)
			return min;
		if(in > max)
			return max;
		return in;
	}

	// ***********************************************************************
	inline float Math::LerpTable(float values[], float intervals[], float t)
	{
		// Find the intervals at the bounds of the given t
		int i = 0;
		for(; t > intervals[i+1]; i++);

		float actualT = (t - intervals[i]) / (intervals[i+1] - intervals[i]);
		return Lerp(values[i], values[i+1], actualT);
	}

} // namespace Gdk

#endif