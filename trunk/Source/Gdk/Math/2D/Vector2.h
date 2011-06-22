/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Vector2
	{
	public:
		// Components
		float X, Y;

		// Constructors
		Vector2 ();
		Vector2 (float x, float y);
		Vector2 (const Vector2& input);

		// Asssignment Operator
		inline Vector2& operator= (const Vector2& input);

		// Comparison Operators
		inline bool operator== (const Vector2& input) const;
		inline bool operator!= (const Vector2& input) const;

		// Arithmetic Operators
		inline Vector2 operator+ (const Vector2& input) const;
		inline Vector2 operator- (const Vector2& input) const;
		inline Vector2 operator* (float scalar) const;
		inline Vector2 operator/ (float scalar) const;
		inline Vector2 operator- () const;

		// Arithmetic Update Operators
		inline Vector2& operator+= (const Vector2& input);
		inline Vector2& operator-= (const Vector2& input);
		inline Vector2& operator*= (float scalar);
		inline Vector2& operator/= (float scalar);

		// Vector functions
		inline float Length () const;
		inline float LengthSquared () const;
		inline float Normalize ();
		inline float Dot (const Vector2& input) const;
		inline float GetRotationAngle() const;
		static Vector2 FromAngle(float angle); 
        
        // Coordinate System Conversion Methods
        void ToPolarCoordinate(float& r, float& theta);
        static Vector2 FromPolarCoordinate(float r, float theta);
        
		// Interpolation Methods
		static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
		static Vector2 Hermite(const Vector2& point1, const Vector2& tangent1, const Vector2& point2, const Vector2& tangent2, float t);
		static Vector2 Bezier(const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector2& point4, float t);
		static Vector2 CatmullRom(const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector2& point4, float t);	// 4 Control Points, t=0=CP2, t=1=CP3
		static Vector2 Barycentric(const Vector2& point1, const Vector2& point2, const Vector2& point3, float t1, float t2);

		// String conversion methods
		static Vector2 Parse(const char *str);
		string ToString();

		// special vectors
		static const Vector2 ZERO;    // (0,0)
		static const Vector2 UNIT_X;  // (1,0)
		static const Vector2 UNIT_Y;  // (0,1)
		static const Vector2 ONE;     // (1,1)
	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Vector2& Vector2::operator= (const Vector2& input)
	{
		X = input.X;
		Y = input.Y;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Vector2::operator== (const Vector2& input) const
	{
		return 
			X == input.X &&
			Y == input.Y;
	}

	// ***********************************************************************
	inline bool Vector2::operator!= (const Vector2& input) const
	{
		return 
			X != input.X ||
			Y != input.Y;
	}

	// ===================================================================================
	// Arithmetic Operators
	// ===================================================================================

	// ***********************************************************************
	inline Vector2 Vector2::operator+ (const Vector2& input) const
	{
		return Vector2(
			X + input.X,
			Y + input.Y
			);
	}

	// ***********************************************************************
	inline Vector2 Vector2::operator- (const Vector2& input) const
	{
		return Vector2(
			X - input.X,
			Y - input.Y
			);
	}

	// ***********************************************************************
	inline Vector2 Vector2::operator* (float scalar) const
	{
		return Vector2(
			X * scalar,
			Y * scalar
			);
	}

	// ***********************************************************************
	inline Vector2 Vector2::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Vector2(
				X * inverse,
				Y * inverse
				);
		}
		else
		{
			return Vector2(
				FLT_MAX,
				FLT_MAX
				);
		}
	}

	// ***********************************************************************
	inline Vector2 Vector2::operator- () const
	{
		return Vector2(
			-X,
			-Y
			);
	}

	// ***********************************************************************
	inline Vector2 operator* (float scalar, const Vector2& input)
	{
		return Vector2(
			scalar * input.X,
			scalar * input.Y
			);
	}

	// ===================================================================================
	// Arithmetic Update Operators
	// ===================================================================================

	// ***********************************************************************
	inline Vector2& Vector2::operator+= (const Vector2& input)
	{
		X += input.X;
		Y += input.Y;
		return *this;
	}

	// ***********************************************************************
	inline Vector2& Vector2::operator-= (const Vector2& input)
	{
		X -= input.X;
		Y -= input.Y;
		return *this;
	}

	// ***********************************************************************
	inline Vector2& Vector2::operator*= (float scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}

	// ***********************************************************************
	inline Vector2& Vector2::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			X *= inverse;
			Y *= inverse;
		}
		else
		{
			X = FLT_MAX;
			Y = FLT_MAX;
		}
		return *this;
	}

	// ===================================================================================
	// Vector Functions
	// ===================================================================================

	// ***********************************************************************
	inline float Vector2::Length() const
	{
		return Math::Sqrt(
			X * X +
			Y * Y
			);
	}

	// ***********************************************************************
	inline float Vector2::LengthSquared() const
	{
		return
			X * X +
			Y * Y
			;
	}

	// ***********************************************************************
	inline float Vector2::Dot(const Vector2& input) const
	{
		return 
			X * input.X +
			Y * input.Y
			;
	}

	// ***********************************************************************
	inline float Vector2::Normalize()
	{
		float length = Length();

		if (length > Math::ZERO_TOLERANCE)
		{
			// Is the length > Zero tolerance?
			float inverseLength = 1.0f / length;
			X *= inverseLength;
			Y *= inverseLength;
		}
		else
		{
			// The length is approximately 0
			length = 0.0f;
			X = 0.0f;
			Y = 0.0f;
		}

		return length;
	}

	// ***********************************************************************
	inline float Vector2::GetRotationAngle() const
	{
		return Math::ATan2(Y, X);	
	}

} // namespace
