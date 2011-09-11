/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../2D/Vector2.h"

namespace Gdk
{
	class Vector3
	{
	public:
		// Components
		float X, Y, Z;

		// Constructors
		Vector3 ();
		Vector3 (float x, float y, float z);
        Vector3 (const Vector2& xy, float z);
		Vector3 (const Vector3& input);

		// Asssignment Operator
		inline Vector3& operator= (const Vector3& input);

		// Comparison Operators
		inline bool operator== (const Vector3& input) const;
		inline bool operator!= (const Vector3& input) const;

		// Arithmetic Operators
		inline Vector3 operator+ (const Vector3& input) const;
		inline Vector3 operator- (const Vector3& input) const;
		inline Vector3 operator* (float scalar) const;
		inline Vector3 operator/ (float scalar) const;
		inline Vector3 operator- () const;

		// Arithmetic Update Operators
		inline Vector3& operator+= (const Vector3& input);
		inline Vector3& operator-= (const Vector3& input);
		inline Vector3& operator*= (float scalar);
		inline Vector3& operator/= (float scalar);

        // Vector2 accessor functions
        inline Vector2 GetXY()    { return Vector2(X,Y); }
        inline Vector2 GetYZ()    { return Vector2(Y,Z); }
        inline Vector2 GetXZ()    { return Vector2(X,Z); }
        
		// Vector functions
		inline float Length() const;
		inline float LengthSquared() const;
		inline float Normalize();               // normalizes in place
        inline Vector3 GetNormalized() const;   // returns a normalized copy
		inline float Dot (const Vector3& input) const;
		inline Vector3 Cross (const Vector3& input) const;
		inline Vector3 UnitCross (const Vector3& input) const;
        inline static float Distance(const Vector3& v1, const Vector3& v2);
        
		// Interpolation Methods
		static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
		static Vector3 Hermite(const Vector3& point1, const Vector3& tangent1, const Vector3& point2, const Vector3& tangent2, float t);
		static Vector3 Bezier(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& point4, float t);
		static Vector3 CatmullRom(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& point4, float t);	// 4 Control Points, t=0=CP2, t=1=CP3
		static Vector3 Barycentric(const Vector3& point1, const Vector3& point2, const Vector3& point3, float t1, float t2);

		// String conversion methods
		static Vector3 Parse(const char *str);
		string ToString();

		// Stream Utilities
		static Vector3 ReadFromStream(class Stream* stream);
		void WriteToStream(class Stream* stream);

		// special vectors
		static const Vector3 ZERO;    // (0,0,0)
		static const Vector3 UNIT_X;  // (1,0,0)
		static const Vector3 UNIT_Y;  // (0,1,0)
		static const Vector3 UNIT_Z;  // (0,0,1)
		static const Vector3 ONE;     // (1,1,1)

	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Vector3& Vector3::operator= (const Vector3& input)
	{
		X = input.X;
		Y = input.Y;
		Z = input.Z;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Vector3::operator== (const Vector3& input) const
	{
		return 
			X == input.X &&
			Y == input.Y &&
			Z == input.Z;
	}

	// ***********************************************************************
	inline bool Vector3::operator!= (const Vector3& input) const
	{
		return 
			X != input.X ||
			Y != input.Y ||
			Z != input.Z;
	}

	// ===================================================================================
	// Arithmetic Operators
	// ===================================================================================

	// ***********************************************************************
	inline Vector3 Vector3::operator+ (const Vector3& input) const
	{
		return Vector3(
			X + input.X,
			Y + input.Y,
			Z + input.Z
			);
	}

	// ***********************************************************************
	inline Vector3 Vector3::operator- (const Vector3& input) const
	{
		return Vector3(
			X - input.X,
			Y - input.Y,
			Z - input.Z
			);
	}

	// ***********************************************************************
	inline Vector3 Vector3::operator* (float scalar) const
	{
		return Vector3(
			X * scalar,
			Y * scalar,
			Z * scalar
			);
	}

	// ***********************************************************************
	inline Vector3 Vector3::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Vector3(
				X * inverse,
				Y * inverse,
				Z * inverse
				);
		}
		else
		{
			return Vector3(
				FLT_MAX,
				FLT_MAX,
				FLT_MAX
				);
		}
	}

	// ***********************************************************************
	inline Vector3 Vector3::operator- () const
	{
		return Vector3(
			-X,
			-Y,
			-Z
			);
	}

	// ***********************************************************************
	inline Vector3 operator* (float scalar, const Vector3& input)
	{
		return Vector3(
			scalar * input.X,
			scalar * input.Y,
			scalar * input.Z
			);
	}

	// ===================================================================================
	// Arithmetic Update Operators
	// ===================================================================================

	// ***********************************************************************
	inline Vector3& Vector3::operator+= (const Vector3& input)
	{
		X += input.X;
		Y += input.Y;
		Z += input.Z;
		return *this;
	}

	// ***********************************************************************
	inline Vector3& Vector3::operator-= (const Vector3& input)
	{
		X -= input.X;
		Y -= input.Y;
		Z -= input.Z;
		return *this;
	}

	// ***********************************************************************
	inline Vector3& Vector3::operator*= (float scalar)
	{
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

	// ***********************************************************************
	inline Vector3& Vector3::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			X *= inverse;
			Y *= inverse;
			Z *= inverse;
		}
		else
		{
			X = FLT_MAX;
			Y = FLT_MAX;
			Z = FLT_MAX;
		}
		return *this;
	}

	// ===================================================================================
	// Vector Functions
	// ===================================================================================

	// ***********************************************************************
	inline float Vector3::Length() const
	{
		return Math::Sqrt(
			X * X +
			Y * Y +
			Z * Z
			);
	}

	// ***********************************************************************
	inline float Vector3::LengthSquared() const
	{
		return
			X * X +
			Y * Y +
			Z * Z
			;
	}

	// ***********************************************************************
	inline float Vector3::Dot(const Vector3& input) const
	{
		return 
			X * input.X +
			Y * input.Y +
			Z * input.Z
			;
	}

	// ***********************************************************************
	inline float Vector3::Normalize()
	{
		float length = Length();

		if (length > Math::ZERO_TOLERANCE)
		{
			// Is the length > Zero tolerance?
			float inverseLength = 1.0f / length;
			X *= inverseLength;
			Y *= inverseLength;
			Z *= inverseLength;
		}
		else
		{
			// The length is approximately 0
			length = 0.0f;
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}

		return length;
	}

    // ***********************************************************************
	inline Vector3 Vector3::GetNormalized() const
	{
        // Clone this vector
		Vector3 normalized(this->X, this->Y, this->Z);
        
        // Normalize the clone
        normalized.Normalize();
        
        return normalized;
    }

	// ***********************************************************************
	inline Vector3 Vector3::Cross(const Vector3& input) const
	{
		// Right handed-rule cross product
		return Vector3(
			Y * input.Z - Z * input.Y,
			Z * input.X - X * input.Z,
			X * input.Y - Y * input.X
			);
	}
    
    // ***********************************************************************
    inline float Vector3::Distance(const Vector3& v1, const Vector3& v2)
    {
        float x = v1.X - v2.X;
        float y = v1.Y - v2.Y;
        float z = v1.Z - v2.Z;
        return Math::Sqrt(x*x + y*y + z*z);
    }

} // namespace
