/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



// NOTE: this file is named Maths.h instead of Math.h, so the GCC / XCode compilers wont confuse it with the ansi-C math.h
//
// Most of the complex math for Intersection & Distance methods was adapted from Gamedev.net articles
// or David Eberly's Books.  (Which are excellent books if you havent read them)
//

namespace Gdk
{
    /// @addtogroup Math
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Enumeration of common types of intersections that can occur between 2D and/or 3D primitives
    /// @remarks
    ///     This enumeration is often the result of an IntersectionN::Find() method
    // =================================================================================
    namespace IntersectionType
    {
        enum Enum
        {
            None = 0,
            Point,
            Ray,
            LineSegment,
            Line,
            Plane,
            Unknown,
        };
    }
    
    // =================================================================================
    ///	@brief
    ///		Provides common mathematical constants and functions as statics
    // =================================================================================
	class Math
	{
	public:
	    
        /// Public Constants
        // =====================================================
        
        // ---------------------------------
        /// @name Common Mathematical Constants
        /// @{
        
        /// Smallest possible delta between two float values
		static const float EPSILON;
        
        /// Effective variance from 0.0 to be considered != 0
		static const float ZERO_TOLERANCE;
        
        /// Maximum value for a float
		static const float MAX_float;
        
        /// Pi
		static const float PI;
        
        /// 2 * Pi
		static const float TWO_PI;
        
        /// 0.5 * Pi
		static const float HALF_PI;
        
        /// 1.0 / Pi
		static const float INV_PI;
        
        /// 1.0 / (2 * Pi)
		static const float INV_TWO_PI;
        
        /// This scalar times a degrees value will result in the equivalent radians value
		static const float DEG_TO_RAD;
        
        /// This scalar times a radians value will result in the equivalent degrees value
		static const float RAD_TO_DEG;
        
        /// Log(2.0)
		static const float LN_2;
        
        /// Log(10.0)
		static const float LN_10;
        
        /// 1.0 / Log(2.0)
		static const float INV_LN_2;
        
        /// 1.0 / Log(10.0)
		static const float INV_LN_10;
        
        /// @}
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Basic Math Methods
        /// @{
        
		static float Sign (float input);
		static float Abs (float input);
		static float Ceil (float input);
		static float Floor (float input);
		static float Mod (float numerator, float denominator);
		
		static inline float Max(float a, float b);
		static inline int Max(int a, int b);
		static inline unsigned char Max(UInt8 a, UInt8 b);
		static inline float Min(float a, float b);
		static inline int Min(int a, int b);
		static inline unsigned char Min(UInt8 a, UInt8 b);
		static inline float Clamp(float in, float min, float max);
        
        /// @}
        
        // ---------------------------------
        /// @name Exponential Methods
        /// @{

		static float Sqr (float input);
		static float Sqrt (float input);
		static float Pow (float base, float exponent);
		static float Exp (float input);    
		static float Log (float input);
		static float Log2 (float input);
		static float Log10 (float input);
        static bool IsPowerOfTwo(UInt32 value);
        
        /// @}
        
        // ---------------------------------
        /// @name Trigonometry Methods
        /// @{
        
		static float Cos (float input);
		static float Sin (float input);
		static float Tan (float input);
		static float ACos (float input);
		static float ASin (float input);
		static float ATan (float input);
		static float ATan2 (float y, float x);

        /// @}
        
        // ---------------------------------
        /// @name Linear Interpolation Methods
        /// @{
        
		static inline float Lerp (float start, float end, float t);
		static inline int Lerp (int start, int end, float t);
		static inline unsigned char Lerp (unsigned char start, unsigned char end, float t);
		static inline float LerpTable(float values[], float intervals[], float t);
		static inline float ReverseLerp(float start, float end, float value);     
        
        /// @}
	};
    
    /// @} // Math

	// Inline Implementations
	// ====================================

    // *****************************************************************
    /// @brief
    ///     Returns the greater of two float values
    // *****************************************************************
	inline float Math::Max (float a, float b)
	{
		return a > b ? a : b;
	}

	// *****************************************************************
    /// @brief
    ///     Returns the greater of two int values
    // *****************************************************************
	inline int Math::Max (int a, int b)
	{
		return a > b ? a : b;
	}

    // *****************************************************************
    /// @brief
    ///     Returns the greater of two UInt8 values
    // *****************************************************************
	inline unsigned char Math::Max (UInt8 a, UInt8 b)
	{
		return a > b ? a : b;
	}

	// *****************************************************************
    /// @brief
    ///     Returns the lesser of two float values
    // *****************************************************************
	inline float Math::Min (float a, float b)
	{
		return a < b ? a : b;
	}

	// *****************************************************************
    /// @brief
    ///     Returns the lesser of two int values
    // *****************************************************************
	inline int Math::Min (int a, int b)
	{
		return a < b ? a : b;
	}

	// *****************************************************************
    /// @brief
    ///     Returns the lesser of two UInt8 values
    // *****************************************************************
	inline unsigned char Math::Min (UInt8 a, UInt8 b)
	{
		return a < b ? a : b;
	}
	
    // *****************************************************************
    /// @brief
    ///     Linear interpolates between two float values
    /// @param start
    ///     The value at the start of the interpolation.  This value is returned when T=0
    /// @param end
    ///     The value at the end of the interpolation.  This value is returned when T=1
    /// @param t
    ///     T-value used to control the interpolation.
    // *****************************************************************
	inline float Math::Lerp (float start, float end, float t)
	{
		return start + t * (end - start);
	}

    // *****************************************************************
    /// @brief
    ///     Linear interpolates between two int values
    /// @param start
    ///     The value at the start of the interpolation.  This value is returned when T=0
    /// @param end
    ///     The value at the end of the interpolation.  This value is returned when T=1
    /// @param t
    ///     T-value used to control the interpolation.
    // *****************************************************************
	inline int Math::Lerp (int start, int end, float t)
	{
		return (int)(start + t * (end - start));
	}

    // *****************************************************************
    /// @brief
    ///     Linear interpolates between two UInt8 values
    /// @param start
    ///     The value at the start of the interpolation.  This value is returned when T=0
    /// @param end
    ///     The value at the end of the interpolation.  This value is returned when T=1
    /// @param t
    ///     T-value used to control the interpolation.
    // *****************************************************************
	inline unsigned char Math::Lerp(unsigned char start, unsigned char end, float t)
	{
		return (unsigned char)(start + t * (end - start));
	}

    // *****************************************************************
    /// @brief
    ///     Calculates a T value by that would be generated by a given linear interpolation
    /// @param start
    ///     The value at the start of the interpolation.  This value will return T=0
    /// @param end
    ///     The value at the end of the interpolation.  This value will return T=1
    /// @param value
    ///     The resultant value to reverse interpolate back to a T-value
    // *****************************************************************
	inline float Math::ReverseLerp(float start, float end, float value)
	{
		return (value - start) / (end - start);
	}

    // *****************************************************************
    /// @brief
    ///     Clamps a float value between two min and max allowed values
    /// @param in
    ///     The value to be clamped
    /// @param min
    ///     The minimum that [value] can be
    /// @param max
    ///     The maximum that [value] can be
    // *****************************************************************
	inline float Math::Clamp(float in, float min, float max)
	{
		if(in < min)
			return min;
		if(in > max)
			return max;
		return in;
	}

	// *****************************************************************
    /// @brief
    ///     Interpolates a value within a set of interval/value pairs.
    /// @remarks
    ///     This method takes a table of Value/T-value pairs, and a T-value within the table,
    ///     then finds the local min & max value pairs and linear interpolates between them.
    /// @param values
    ///     Array of values to be interpolated
    /// @param end
    ///     Array of T-value intervals that coorespond to the [values] array
    /// @param t
    ///     T-value used to control the interpolation.
    // *****************************************************************
	inline float Math::LerpTable(float values[], float intervals[], float t)
	{
		// Find the intervals at the bounds of the given t
		int i = 0;
		for(; t > intervals[i+1]; i++);

		float actualT = (t - intervals[i]) / (intervals[i+1] - intervals[i]);
		return Lerp(values[i], values[i+1], actualT);
	}
    
    
} // namespace Gdk
