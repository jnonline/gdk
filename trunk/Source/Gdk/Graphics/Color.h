/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "../Math/Maths.h"

namespace Gdk
{
	// ===========================================================
	class Color
	{
	public:
		// Components
		Byte R, G, B, A;

		// Constructors
		Color();
		Color(Byte r, Byte g, Byte b);
		Color(Byte r, Byte g, Byte b, Byte a);
		Color(const Color& input);

		// Asssignment Operator
		inline Color& operator= (const Color& input);

		// Comparison Operators
		inline bool operator== (const Color& input) const;
		inline bool operator!= (const Color& input) const;

		// Arithmetic Operators
		inline Color operator* (const Color& input) const;
		inline Color operator* (float scalar) const;

		// Arithmetic Update Operators
		inline Color& operator*= (const Color& input);
		inline Color& operator*= (float scalar);

		// Static methods
		static inline Color FromNormalized(float r, float g, float b, float a);
		static inline Color Lerp(const Color& a, const Color& b, float t);
		static inline Color LerpTable(Color colors[], float intervals[], float t);

		// String conversion methods
		static Color Parse(const char *str);
		string ToString();

		// Stream Utilities
		static Color ReadFromStream(class Stream* stream);
		void WriteToStream(class Stream* stream);

		// Named colors
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

		// Multiply a color value by this scalar to normalize the color to the (0.0f - 1.0f) scale
		static const float NORMALIZATION_SCALAR;	
		void Normalize(float* output, bool includeAlpha = true);   // must be float[3] or float[4] with alpha
	};

	// ===================================================================================
	// Assignment Operators
	// ===================================================================================

	// ***********************************************************************
	inline Color& Color::operator= (const Color& input)
	{
		R = input.R;
		G = input.G;
		B = input.B;
		A = input.A;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Color::operator== (const Color& input) const
	{
		return R == input.R && G == input.G && B == input.B && A == input.A;
	}

	// ***********************************************************************
	inline bool Color::operator!= (const Color& input) const
	{
		return R != input.R || G != input.G || B != input.B || A != input.A;
	}

	// ===================================================================================
	// Arithmetic Operators
	// ===================================================================================

	inline Byte MultiplyAndClampBytes(Byte a, Byte b)
	{
		// Use some good old math bits hackery!
		unsigned short c = (unsigned short)a * (unsigned short)(b + 1);
		c >>= 8;
		return (Byte)c;
	}

	// ***********************************************************************
	inline Byte MultiplyAndClampByteAndScalar(Byte a, float scalar)
	{
		int c = (int)(a * scalar);
		if(c > 255)
			return 255;
		if(c < 0)
			return 0;
		return (Byte)c;
	}

	// ***********************************************************************
	inline Color Color::operator* (const Color& input) const
	{
		return Color(
			MultiplyAndClampBytes(R, input.R),
			MultiplyAndClampBytes(G, input.G),
			MultiplyAndClampBytes(B, input.B),
			MultiplyAndClampBytes(A, input.A)
			);
	}

	// ***********************************************************************
	inline Color Color::operator* (float scalar) const
	{
		return Color(
			MultiplyAndClampByteAndScalar(R, scalar),
			MultiplyAndClampByteAndScalar(G, scalar),
			MultiplyAndClampByteAndScalar(B, scalar),
			MultiplyAndClampByteAndScalar(A, scalar)
			);
	}

	// ===================================================================================
	// Arithmetic Update Operators
	// ===================================================================================

	// ***********************************************************************
	inline Color& Color::operator*= (const Color& input)
	{
		R = MultiplyAndClampBytes(R, input.R);
		G = MultiplyAndClampBytes(G, input.G);
		B = MultiplyAndClampBytes(B, input.B);
		A = MultiplyAndClampBytes(A, input.A);
		return *this;
	}

	// ***********************************************************************
	inline Color& Color::operator*= (float scalar)
	{
		R = MultiplyAndClampByteAndScalar(R, scalar);
		G = MultiplyAndClampByteAndScalar(G, scalar);
		B = MultiplyAndClampByteAndScalar(B, scalar);
		A = MultiplyAndClampByteAndScalar(A, scalar);
		return *this;
	}


	// ===================================================================================
	// Inline Methods
	// ===================================================================================

	// ***********************************************************************
	inline Color Color::FromNormalized(float r, float g, float b, float a)
	{
		return Color(
			(UInt8)(r * 255.0f),
			(UInt8)(g * 255.0f),
			(UInt8)(b * 255.0f),
			(UInt8)(a * 255.0f)
			);
	}

	// ***********************************************************************
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

	// ***********************************************************************
	inline Color Color::LerpTable(Color colors[], float intervals[], float t)
	{
		// Find the intervals at the bounds of the given t
		int i = 0;
		for(; t > intervals[i+1]; i++);

		float actualT = (t - intervals[i]) / (intervals[i+1] - intervals[i]);
		return Color::Lerp(colors[i], colors[i+1], actualT);
	}

} // namespace Gdk
