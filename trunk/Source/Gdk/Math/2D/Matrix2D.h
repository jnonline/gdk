/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Matrix2D
	{
	public:
		// Components
		float M11, M12, M13;
		float M21, M22, M23;
		float M31, M32, M33;

		// Constructors
		Matrix2D(bool identity = true);
		Matrix2D(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);
		Matrix2D(const Matrix2D& input);

		// Asssignment Operator
		inline Matrix2D& operator= (const Matrix2D& input);
		inline Matrix2D& Set(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);

		// Comparison Operators
		inline bool operator== (const Matrix2D& input) const;
		inline bool operator!= (const Matrix2D& input) const;

		// Arithmetic Operators
		inline Matrix2D operator* (const Matrix2D& input) const;
		inline Matrix2D operator* (float scalar) const;
		inline Matrix2D operator/ (float scalar) const;
		inline Matrix2D operator- () const;

		// Arithmetic Update Operators
		inline Matrix2D& operator*= (const Matrix2D& input);
		inline Matrix2D& operator*= (float scalar);
		inline Matrix2D& operator/= (float scalar);

		// Vector Transformation Methods
		inline Vector2 TransformVertex(const Vector2& vertex);
		inline Vector2 TransformNormal(const Vector2& normal);
		inline void TransformVertexInPlace(float &x, float &y);
		inline void TransformNormalInPlace(float &x, float &y);

		// In-Place Transformation Methods
		Matrix2D& MakeZero();
		Matrix2D& MakeIdentity();
		Matrix2D& Multiply(const Matrix2D& input);
		Matrix2D& Multiply(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);
        Matrix2D& PreMultiply(const Matrix2D& input);
		Matrix2D& PreMultiply(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);

		Matrix2D& Translate(const Vector2& translation);
        Matrix2D& Translate(float tx, float ty);
		Matrix2D& Scale(float scale);
		Matrix2D& Scale(float sx, float sy);
		Matrix2D& Rotate(float angle);
		Matrix2D& ScaleRotateTranslate(float sx, float sy, float tx, float ty, float angle);
		Matrix2D& RotateScaleTranslate(float sx, float sy, float tx, float ty, float angle);

		// Static Creation Methods
		static Matrix2D CreateTranslation(const Vector2& translation);
        static Matrix2D CreateTranslation(float tx, float ty);
		static Matrix2D CreateScale(float scale);
		static Matrix2D CreateScale(float sx, float sy);
		static Matrix2D CreateRotation(float angle);
		static Matrix2D CreateScaleRotateTranslate(float sx, float sy, float tx, float ty, float angle);
		static Matrix2D CreateRotateScaleTranslate(float sx, float sy, float tx, float ty, float angle);

		// Extra Matrix Methods
		Matrix2D GetTranspose();
		Matrix2D GetInverse();
		float GetDeterminant();
		bool IsIdentity();

		// Stream Utilities
		static Matrix2D ReadFromStream(class Stream* stream);
		void WriteToStream(class Stream* stream);

		// Special matrices
		static const Matrix2D ZERO;
		static const Matrix2D IDENTITY;

	};
	
	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Matrix2D& Matrix2D::operator= (const Matrix2D& input)
	{
		M11 = input.M11; M12 = input.M12; M13 = input.M13;
		M21 = input.M21; M22 = input.M22; M23 = input.M23;
		M31 = input.M31; M32 = input.M32; M33 = input.M33;
		return *this;
	}

	// ***********************************************************************
	inline Matrix2D& Matrix2D::Set(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
	{
		M11 = m11;	M12 = m12;	M13 = m13;
		M21 = m21;	M22 = m22;	M23 = m23;
		M31 = m31;	M32 = m32;	M33 = m33;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Matrix2D::operator== (const Matrix2D& input) const
	{
		return M11 == input.M11 && M12 == input.M12 && M13 == input.M13
			&& M21 == input.M21 && M22 == input.M22 && M23 == input.M23
			&& M31 == input.M31 && M32 == input.M32 && M33 == input.M33;
	}

	// ***********************************************************************
	inline bool Matrix2D::operator!= (const Matrix2D& input) const
	{
		return M11 != input.M11 || M12 != input.M12 || M13 != input.M13
			|| M21 != input.M21 || M22 != input.M22 || M23 != input.M23
			|| M31 != input.M31 || M32 != input.M32 || M33 != input.M33;
	}

	// ===================================================================================
	// Arithmetic Operators
	// ===================================================================================

	// ***********************************************************************
	inline Matrix2D Matrix2D::operator* (const Matrix2D& input) const
	{
		return Matrix2D(
			M11 * input.M11 + M12 * input.M21 + M13 * input.M31,
			M11 * input.M12 + M12 * input.M22 + M13 * input.M32,
			M11 * input.M13 + M12 * input.M23 + M13 * input.M33,
			M21 * input.M11 + M22 * input.M21 + M23 * input.M31,
			M21 * input.M12 + M22 * input.M22 + M23 * input.M32,
			M21 * input.M13 + M22 * input.M23 + M23 * input.M33,
			M31 * input.M11 + M32 * input.M21 + M33 * input.M31,
			M31 * input.M12 + M32 * input.M22 + M33 * input.M32,
			M31 * input.M13 + M32 * input.M23 + M33 * input.M33
			);
	}

	// ***********************************************************************
	inline Matrix2D Matrix2D::operator* (float scalar) const
	{
		return Matrix2D(
			M11 * scalar, M12 * scalar, M13 * scalar,
			M21 * scalar, M22 * scalar, M23 * scalar,
			M31 * scalar, M32 * scalar, M33 * scalar
			);
	}

	// ***********************************************************************
	inline Matrix2D Matrix2D::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Matrix2D(
				M11 * inverse, M12 * inverse, M13 * inverse,
				M21 * inverse, M22 * inverse, M23 * inverse,
				M31 * inverse, M32 * inverse, M33 * inverse
				);
		}
		else
		{
			return Matrix2D(
				FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX
				);
		}
	}

	// ***********************************************************************
	inline Matrix2D Matrix2D::operator- () const
	{
		return Matrix2D(
			-M11, -M12, -M13,
			-M21, -M22, -M23,
			-M31, -M32, -M33
			);
	}

	// ***********************************************************************
	inline Vector2 operator* (const Vector2& v, const Matrix2D& m)
	{
		return Vector2(
			v.X * m.M11 + v.Y * m.M21 + m.M31,
			v.X * m.M12 + v.Y * m.M22 + m.M32
			);
	}

	// ===================================================================================
	// Arithmetic Update Operators
	// ===================================================================================

	// ***********************************************************************
	inline Matrix2D& Matrix2D::operator*= (const Matrix2D& input)
	{
		float m11 = M11 * input.M11 + M12 * input.M21 + M13 * input.M31;
		float m12 = M11 * input.M12 + M12 * input.M22 + M13 * input.M32;
		float m13 = M11 * input.M13 + M12 * input.M23 + M13 * input.M33;
		float m21 = M21 * input.M11 + M22 * input.M21 + M23 * input.M31;
		float m22 = M21 * input.M12 + M22 * input.M22 + M23 * input.M32;
		float m23 = M21 * input.M13 + M22 * input.M23 + M23 * input.M33;
		float m31 = M31 * input.M11 + M32 * input.M21 + M33 * input.M31;
		float m32 = M31 * input.M12 + M32 * input.M22 + M33 * input.M32;
		float m33 = M31 * input.M13 + M32 * input.M23 + M33 * input.M33;
		
		M11 = m11; M12 = m12; M13 = m13;
		M21 = m21; M22 = m22; M23 = m23;
		M31 = m31; M32 = m32; M33 = m33;
		return *this;
	}


	// ***********************************************************************
	inline Matrix2D& Matrix2D::operator*= (float scalar)
	{
		M11 *= scalar; M12 *= scalar; M13 *= scalar;
		M21 *= scalar; M22 *= scalar; M23 *= scalar;
		M31 *= scalar; M32 *= scalar; M33 *= scalar;
		return *this;
	}

	// ***********************************************************************
	inline Matrix2D& Matrix2D::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			M11 *= inverse; M12 *= inverse; M13 *= inverse;
			M21 *= inverse; M22 *= inverse; M23 *= inverse;
			M31 *= inverse; M32 *= inverse; M33 *= inverse;
		}
		else
		{
			M11 = FLT_MAX; M12 = FLT_MAX; M13 = FLT_MAX;
			M21 = FLT_MAX; M22 = FLT_MAX; M23 = FLT_MAX;
			M31 = FLT_MAX; M32 = FLT_MAX; M33 = FLT_MAX;
		}
		return *this;
	}

	// ===================================================================================
	// Vector Transformation Methods
	// ===================================================================================


	inline Vector2 Matrix2D::TransformVertex(const Vector2& vertex)
	{
		return Vector2(
			vertex.X * M11 + vertex.Y * M21 + M31,
			vertex.X * M12 + vertex.Y * M22 + M32
			);
	}

	inline Vector2 Matrix2D::TransformNormal(const Vector2& normal)
	{
		return Vector2(
			normal.X * M11 + normal.Y * M21,
			normal.X * M12 + normal.Y * M22
			);
	}

	inline void Matrix2D::TransformVertexInPlace(float &x, float &y)
	{
		float tempX = x * M11 + y * M21 + M31;
		float tempY = x * M12 + y * M22 + M32;
		x = tempX; y = tempY;
	}

	inline void Matrix2D::TransformNormalInPlace(float &x, float &y)
	{
		float tempX = x * M11 + y * M21;
		float tempY = x * M12 + y * M22;
		x = tempX; y = tempY;
	}

} // namespace Gdk
