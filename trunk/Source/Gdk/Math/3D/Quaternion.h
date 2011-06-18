#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Quaternion
	{
	public:
		// Components
		float W, X, Y, Z;

		// Constructors
		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(const Quaternion& input);

		// Asssignment Operator
		inline Quaternion& operator= (const Quaternion& input);

		// Comparison Operators
		inline bool operator== (const Quaternion& input) const;
		inline bool operator!= (const Quaternion& input) const;

		// Arithmetic Operators
		inline Quaternion operator+ (const Quaternion& input) const;
		inline Quaternion operator- (const Quaternion& input) const;
		inline Quaternion operator* (const Quaternion& input) const;
		inline Quaternion operator* (float scalar) const;
		inline Quaternion operator/ (float scalar) const;
		inline Quaternion operator- () const;

		// Arithmetic Update Operators
		inline Quaternion& operator+= (const Quaternion& input);
		inline Quaternion& operator-= (const Quaternion& input);
		inline Quaternion& operator*= (float scalar);
		inline Quaternion& operator/= (float scalar);

		// Instance Methods
		inline float Length() const;
		inline float LengthSquared() const;
		inline float Normalize();
		inline float Dot(const Quaternion& input) const;
		Quaternion Inverse() const;
		Quaternion Conjugate() const;

		// Creation / Transformation
		float ToAxisAngle(Vector3& axis) const;
		static Quaternion FromAxisAngle(const Vector3& axis, float angle);
		class Matrix3D ToRotationMatrix() const;
		static Quaternion FromRotationMatrix(const class Matrix3D& matrix);
		static Quaternion FromPitchYawRoll(float pitch, float yaw, float roll);
		static Quaternion FromAlignment(const Vector3& start, const Vector3& end);	// Creates a quaternion that rotates from V1 to V2

		// Interpolation
		static Quaternion Slerp(float t, const Quaternion& start, const Quaternion& end);
		static Quaternion SlerpExtraSpins (float t, const Quaternion& start, const Quaternion& end, int extraSpins);
		
		// Special quaternions
		static const Quaternion ZERO;		// (0)(0,0,0)
		static const Quaternion IDENTITY;	// (1)(0,0,0)
	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Quaternion& Quaternion::operator= (const Quaternion& input)
	{
		W = input.W;
		X = input.X;
		Y = input.Y;
		Z = input.Z;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Quaternion::operator== (const Quaternion& input) const
	{
		return
			W == input.W &&
			X == input.X &&
			Y == input.Y &&
			Z == input.Z;
	}

	// ***********************************************************************
	inline bool Quaternion::operator!= (const Quaternion& input) const
	{
		return 
			W != input.W ||
			X != input.X ||
			Y != input.Y ||
			Z != input.Z;
	}

	// ===================================================================================
	// Arithmetic Operators
	// ===================================================================================

	// ***********************************************************************
	inline Quaternion Quaternion::operator+ (const Quaternion& input) const
	{
		return Quaternion(
			W + input.W,
			X + input.X,
			Y + input.Y,
			Z + input.Z
			);
	}

	// ***********************************************************************
	inline Quaternion Quaternion::operator- (const Quaternion& input) const
	{
		return Quaternion(
			W - input.W,
			X - input.X,
			Y - input.Y,
			Z - input.Z
			);
	}

	// ***********************************************************************
	inline Quaternion Quaternion::operator* (const Quaternion& input) const
	{
		return Quaternion(
			W * input.W - X * input.X - Y * input.Y - Z * input.Z,
			W * input.X + X * input.W + Y * input.Z - Z * input.Y,
			W * input.Y + Y * input.W + Z * input.X - X * input.Z,
			W * input.Z + Z * input.W + X * input.Y - Y * input.X
			);
	}

	// ***********************************************************************
	inline Quaternion Quaternion::operator* (float scalar) const
	{
		return Quaternion(
			W * scalar,
			X * scalar,
			Y * scalar,
			Z * scalar
			);
	}

	// ***********************************************************************
	inline Quaternion Quaternion::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Quaternion(
				W * inverse,
				X * inverse,
				Y * inverse,
				Z * inverse
				);
		}
		else
		{
			return Quaternion(
				FLT_MAX,
				FLT_MAX,
				FLT_MAX,
				FLT_MAX
				);
		}
	}

	// ***********************************************************************
	inline Quaternion Quaternion::operator- () const
	{
		return Quaternion(
			-W,
			-X,
			-Y,
			-Z
			);
	}

	// ***********************************************************************
	inline Quaternion operator* (float scalar, const Quaternion& input)
	{
		return Quaternion(
			scalar * input.W,
			scalar * input.X,
			scalar * input.Y,
			scalar * input.Z
			);
	}

	// ===================================================================================
	// Arithmetic Update Operators
	// ===================================================================================

	// ***********************************************************************
	inline Quaternion& Quaternion::operator+= (const Quaternion& input)
	{
		W += input.W;
		X += input.X;
		Y += input.Y;
		Z += input.Z;
		return *this;
	}

	// ***********************************************************************
	inline Quaternion& Quaternion::operator-= (const Quaternion& input)
	{
		W -= input.W;
		X -= input.X;
		Y -= input.Y;
		Z -= input.Z;
		return *this;
	}

	// ***********************************************************************
	inline Quaternion& Quaternion::operator*= (float scalar)
	{
		W *= scalar;
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

	// ***********************************************************************
	inline Quaternion& Quaternion::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			W *= inverse;
			X *= inverse;
			Y *= inverse;
			Z *= inverse;
		}
		else
		{
			W = FLT_MAX;
			X = FLT_MAX;
			Y = FLT_MAX;
			Z = FLT_MAX;
		}
		return *this;
	}

	// ===================================================================================
	// Instance Methods
	// ===================================================================================

	// ***********************************************************************
	inline float Quaternion::Length() const
	{
		return Math::Sqrt(
			W * W +
			X * X +
			Y * Y +
			Z * Z
			);
	}

	// ***********************************************************************
	inline float Quaternion::LengthSquared() const
	{
		return
			W * W +
			X * X +
			Y * Y +
			Z * Z
			;
	}

	// ***********************************************************************
	inline float Quaternion::Dot(const Quaternion& input) const
	{
		return 
			W * input.W +
			X * input.X +
			Y * input.Y +
			Z * input.Z
			;
	}

	// ***********************************************************************
	inline float Quaternion::Normalize()
	{
		float length = Length();

		if (length > Math::ZERO_TOLERANCE)
		{
			// Is the length > Zero tolerance?
			float inverseLength = 1.0f / length;
			W *= inverseLength;
			X *= inverseLength;
			Y *= inverseLength;
			Z *= inverseLength;
		}
		else
		{
			// The length is approximately 0
			length = 0.0f;
			W = 0.0f;
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}

		return length;
	}

} // namespace
