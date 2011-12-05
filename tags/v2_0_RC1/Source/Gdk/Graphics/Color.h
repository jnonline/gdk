/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Math/Maths.h"

namespace Gdk
{
	/// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Represents a 32-bit RGBA color value.
    // =================================================================================
	class Color
	{
	public:
        
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Properties
        /// @{

		/// 8-Bit Red Channel
		UInt8 R;
        
        /// 8-Bit Green Channel
        UInt8 G;
        
        /// 8-Bit Blue Channel
        UInt8 B;
        
        /// 8-Bit Alpha Channel
        UInt8 A;
        
        /// @}
        // ---------------------------------
        /// @name Constructors
        /// @{

		Color();
		Color(UInt8 r, UInt8 g, UInt8 b);
		Color(UInt8 r, UInt8 g, UInt8 b, UInt8 a);
		Color(const Color& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Color& operator= (const Color& input);

		inline bool operator== (const Color& input) const;
		inline bool operator!= (const Color& input) const;

		inline Color operator* (const Color& input) const;
		inline Color operator* (float scalar) const;

		inline Color& operator*= (const Color& input);
		inline Color& operator*= (float scalar);

        /// @}
        // ---------------------------------
        /// @name Utility Methods
        /// @{
        
		static inline Color FromNormalized(float r, float g, float b, float a);
		static inline Color Lerp(const Color& a, const Color& b, float t);
		static inline Color LerpTable(Color colors[], float intervals[], float t);

		static Color Parse(const char *str);
		string ToString();

		static Color ReadFromStream(class Stream* stream);
		void WriteToStream(class Stream* stream);
        
        void Normalize(float* output, bool includeAlpha = true);   // must be float[3] or float[4] with alpha

        /// @}
        // ---------------------------------
        /// @name Static Constants
        /// @{
        
        /// Multiplying a UInt8 value by this scalar will result in a normalized value in the (0.0f - 1.0f) range.
        static const float NORMALIZATION_SCALAR;	
        
        /// @}
        // ---------------------------------
        /// @name Common Colors
        /// @{
        
		static const Color TRANSPARENT_BLACK;
		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color YELLOW;
		static const Color PURPLE;
		static const Color CYAN;
		static const Color GRAY;
        
        /// @}
	};
    
    /// @}

	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies the values of the given box to this box
    // *****************************************************************
	inline Color& Color::operator= (const Color& input)
	{
		R = input.R;
		G = input.G;
		B = input.B;
		A = input.A;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: Returns true if two colors are the same
    // *****************************************************************
    inline bool Color::operator== (const Color& input) const
	{
		return R == input.R && G == input.G && B == input.B && A == input.A;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: Returns true if two colors are different
    // *****************************************************************
	inline bool Color::operator!= (const Color& input) const
	{
		return R != input.R || G != input.G || B != input.B || A != input.A;
	}
    
    // *****************************************************************
    /// @brief
    ///     Multiplies two byte values on a normalized scale.
    /// @remarks
    ///     This is the equivalent of converting both bytes from 0-1 float
    ///     scale and multiplying them, then converting back to byte.
    // *****************************************************************
	inline Byte MultiplyAndClampBytes(Byte a, Byte b)
	{
		// Use some good old math bits hackery!
		unsigned short c = (unsigned short)a * (unsigned short)(b + 1);
		c >>= 8;
		return (Byte)c;
	}
    
	// *****************************************************************
    /// @brief
    ///     Multiplies a byte by a float scalar.
    /// @remarks
    ///     The result is clamped between 0 & 255
    // *****************************************************************
	inline Byte MultiplyAndClampByteAndScalar(Byte a, float scalar)
	{
		int c = (int)(a * scalar);
		if(c > 255)
			return 255;
		if(c < 0)
			return 0;
		return (Byte)c;
	}

    // *****************************************************************
    /// @brief
    ///     Multiplies two color values together.
    /// @remarks
    ///     The multiplication is done with normalized values.  Thus multiplying 
    ///     a color with white (255,255,255,255) will just be the color.
    // *****************************************************************
	inline Color Color::operator* (const Color& input) const
	{
		return Color(
                     MultiplyAndClampBytes(R, input.R),
                     MultiplyAndClampBytes(G, input.G),
                     MultiplyAndClampBytes(B, input.B),
                     MultiplyAndClampBytes(A, input.A)
                     );
	}
    
	// *****************************************************************
    /// @brief
    ///     Multiplies a color value by a scalar
    /// @remarks
    ///     The multiplication is done with normalized values.  Thus multiplying 
    ///     a color by 1.0 will just be the color.
    // *****************************************************************
    inline Color Color::operator* (float scalar) const
	{
		return Color(
                     MultiplyAndClampByteAndScalar(R, scalar),
                     MultiplyAndClampByteAndScalar(G, scalar),
                     MultiplyAndClampByteAndScalar(B, scalar),
                     MultiplyAndClampByteAndScalar(A, scalar)
                     );
	}
    
	// *****************************************************************
    /// @brief
    ///     Multiplies this color by a specific color.
    /// @remarks
    ///     The multiplication is done with normalized values.  Thus multiplying 
    ///     with white (255,255,255,255) will leave the color unaffected.
    // *****************************************************************
    inline Color& Color::operator*= (const Color& input)
	{
		R = MultiplyAndClampBytes(R, input.R);
		G = MultiplyAndClampBytes(G, input.G);
		B = MultiplyAndClampBytes(B, input.B);
		A = MultiplyAndClampBytes(A, input.A);
		return *this;
	}
    
	// *****************************************************************
    /// @brief
    ///     Multiplies this color by a scalar
    /// @remarks
    ///     The multiplication is done with normalized values.  
    ///     Thus multiplying a by 1.0 will just be the color.
    // *****************************************************************
    inline Color& Color::operator*= (float scalar)
	{
		R = MultiplyAndClampByteAndScalar(R, scalar);
		G = MultiplyAndClampByteAndScalar(G, scalar);
		B = MultiplyAndClampByteAndScalar(B, scalar);
		A = MultiplyAndClampByteAndScalar(A, scalar);
		return *this;
	}
	
	// *****************************************************************
    /// @brief
    ///     Creates a Color from a set of normalized color values
    /// @param r
    ///     The red component value (0.0 - 1.0)
    /// @param g
    ///     The green component value (0.0 - 1.0)
    /// @param b
    ///     The blue component value (0.0 - 1.0)
    /// @param a
    ///     The alpha component value (0.0 - 1.0)
    // *****************************************************************
    inline Color Color::FromNormalized(float r, float g, float b, float a)
	{
		return Color(
			(UInt8)(r * 255.0f),
			(UInt8)(g * 255.0f),
			(UInt8)(b * 255.0f),
			(UInt8)(a * 255.0f)
			);
	}

	// *****************************************************************
    /// @brief
    ///     Linear interpolates between two colors
    /// @param start
    ///     The 1st color, represented at t=0
    /// @param end
    ///     The 2nd color, represented at t=1
    /// @param t
    ///     The t-value of the interpolation. (usually between 0 and 1)
    // *****************************************************************
    inline Color Color::Lerp(const Color& a, const Color& b, float t)
	{
		// t(0) = a
		// t(1) = b
		return Color(
			Math::Lerp(a.R, b.R, t),
			Math::Lerp(a.G, b.G, t),
			Math::Lerp(a.B, b.B, t),
			Math::Lerp(a.A, b.A, t)
			);
	}

	// *****************************************************************
    /// @brief
    ///     Interpolates a color within a table of Color/T-Value pairs.
    /// @remarks
    ///     This method takes a table of Color/T-value pairs, and a T-value within the table range,
    ///     then finds the local min & max color pairs and linear interpolates between them.
    /// @param colors
    ///     Array of colors to be interpolated
    /// @param end
    ///     Array of T-value intervals that coorespond to the [colors] array
    /// @param t
    ///     T-value used to control the interpolation.
    // *****************************************************************
	inline Color Color::LerpTable(Color colors[], float intervals[], float t)
	{
		// Find the intervals at the bounds of the given t
		int i = 0;
		for(; t > intervals[i+1]; i++);

		float actualT = (t - intervals[i]) / (intervals[i+1] - intervals[i]);
		return Color::Lerp(colors[i], colors[i+1], actualT);
	}

} // namespace Gdk
