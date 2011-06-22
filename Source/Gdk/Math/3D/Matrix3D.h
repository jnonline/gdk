/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// NOTE!!
	//   All Camera/LookAt matrices assume 
	//		X=Right
	//		Y=Up
	//		Z=Towards you

	// *******************************************************
	class Matrix3D
	{
	public:
		// Components
		float M11, M12, M13, M14;
		float M21, M22, M23, M24;
		float M31, M32, M33, M34;
		float M41, M42, M43, M44;

		// Constructors
		Matrix3D(bool identity = true);
		Matrix3D(float m11, float m12, float m13, float m14, 
					float m21, float m22, float m23, float m24, 
					float m31, float m32, float m33, float m34, 
					float m41, float m42, float m43, float m44);
		Matrix3D(float *components);
		Matrix3D(const Matrix3D& input);

		// Asssignment Operator
		inline Matrix3D& operator= (const Matrix3D& input);

		// Comparison Operators
		inline bool operator== (const Matrix3D& input) const;
		inline bool operator!= (const Matrix3D& input) const;

		// Arithmetic Operators
		inline Matrix3D operator* (const Matrix3D& input) const;
		inline Matrix3D operator* (float scalar) const;
		inline Matrix3D operator/ (float scalar) const;
		inline Matrix3D operator- () const;

		// Arithmetic Update Operators
		inline Matrix3D& operator*= (const Matrix3D& input);
		inline Matrix3D& operator*= (float scalar);
		inline Matrix3D& operator/= (float scalar);

		// Vector Transformation Methods
		inline Vector3 TransformVertex(const Vector3& vertex);
		inline Vector3 TransformNormal(const Vector3& normal);
		inline void TransformVertexInPlace(float& x, float& y, float& z);
		inline void TransformNormalInPlace(float& x, float& y, float& z);

		// In-Place Transformation Methods
		Matrix3D& MakeZero();
		Matrix3D& MakeIdentity();
		Matrix3D& Multiply(const Matrix3D& input);
		Matrix3D& Multiply(float m11, float m12, float m13, float m14, 
					float m21, float m22, float m23, float m24, 
					float m31, float m32, float m33, float m34, 
					float m41, float m42, float m43, float m44);
		Matrix3D& Translate(const Vector3& translation);
		Matrix3D& Translate(float x, float y, float z);
		Matrix3D& Scale(float scalar);
		Matrix3D& Scale(float sx, float sy, float sz);
		Matrix3D& RotateX(float angle);
		Matrix3D& RotateY(float angle);
		Matrix3D& RotateZ(float angle);
		Matrix3D& RotateOnAxis(const Vector3& axis, float angle);
		Matrix3D& RotateQuaternion(const class Quaternion& quat);
		Matrix3D& Reflect(const class Plane3& plane);

		// In-Place View / Projection Methods
		Matrix3D& LookAt(const Vector3& position, const Vector3& target, const Vector3& up);
		Matrix3D& PerspectiveFOV(float fov, float aspectRatio, float near, float far);
		Matrix3D& Ortho(float width, float height, float nearPlane, float farPlane);
		Matrix3D& OrthoOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);

		// Extra Matrix Methods
		Matrix3D GetTranspose();
		Matrix3D GetInverse();
		float GetDeterminant();

		// Special matrices
		static const Matrix3D ZERO;
		static const Matrix3D IDENTITY;
	};
	
	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline Matrix3D& Matrix3D::operator= (const Matrix3D& input)
	{
		M11 = input.M11; M12 = input.M12; M13 = input.M13; M14 = input.M14;
		M21 = input.M21; M22 = input.M22; M23 = input.M23; M24 = input.M24;
		M31 = input.M31; M32 = input.M32; M33 = input.M33; M34 = input.M34;
		M41 = input.M41; M42 = input.M42; M43 = input.M43; M44 = input.M44;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool Matrix3D::operator== (const Matrix3D& input) const
	{
		return M11 == input.M11 && M12 == input.M12 && M13 == input.M13 && M14 == input.M14
			&& M21 == input.M21 && M22 == input.M22 && M23 == input.M23 && M24 == input.M24
			&& M31 == input.M31 && M32 == input.M32 && M33 == input.M33 && M34 == input.M34
			&& M41 == input.M41 && M42 == input.M42 && M43 == input.M43 && M44 == input.M44;
	}

	// ***********************************************************************
	inline bool Matrix3D::operator!= (const Matrix3D& input) const
	{
		return M11 != input.M11 || M12 != input.M12 || M13 != input.M13 || M14 != input.M14
			|| M21 != input.M21 || M22 != input.M22 || M23 != input.M23 || M24 != input.M24
			|| M31 != input.M31 || M32 != input.M32 || M33 != input.M33 || M34 != input.M34
			|| M41 != input.M41 || M42 != input.M42 || M43 != input.M43 || M44 != input.M44;
	}

	// ===================================================================================
	// Arithmetic Operators
	// ===================================================================================

	// ***********************************************************************
	inline Matrix3D Matrix3D::operator* (const Matrix3D& input) const
	{
		return Matrix3D(
			M11 * input.M11 + M12 * input.M21 + M13 * input.M31 + M14 * input.M41,
			M11 * input.M12 + M12 * input.M22 + M13 * input.M32 + M14 * input.M42,
			M11 * input.M13 + M12 * input.M23 + M13 * input.M33 + M14 * input.M43,
			M11 * input.M14 + M12 * input.M24 + M13 * input.M34 + M14 * input.M44,

			M21 * input.M11 + M22 * input.M21 + M23 * input.M31 + M24 * input.M41,
			M21 * input.M12 + M22 * input.M22 + M23 * input.M32 + M24 * input.M42,
			M21 * input.M13 + M22 * input.M23 + M23 * input.M33 + M24 * input.M43,
			M21 * input.M14 + M22 * input.M24 + M23 * input.M34 + M24 * input.M44,

			M31 * input.M11 + M32 * input.M21 + M33 * input.M31 + M34 * input.M41,
			M31 * input.M12 + M32 * input.M22 + M33 * input.M32 + M34 * input.M42,
			M31 * input.M13 + M32 * input.M23 + M33 * input.M33 + M34 * input.M43,
			M31 * input.M14 + M32 * input.M24 + M33 * input.M34 + M34 * input.M44,

			M41 * input.M11 + M42 * input.M21 + M43 * input.M31 + M44 * input.M41,
			M41 * input.M12 + M42 * input.M22 + M43 * input.M32 + M44 * input.M42,
			M41 * input.M13 + M42 * input.M23 + M43 * input.M33 + M44 * input.M43,
			M41 * input.M14 + M42 * input.M24 + M43 * input.M34 + M44 * input.M44
			);
	}

	// ***********************************************************************
	inline Matrix3D Matrix3D::operator* (float scalar) const
	{
		return Matrix3D(
			M11 * scalar, M12 * scalar, M13 * scalar, M14 * scalar,
			M21 * scalar, M22 * scalar, M23 * scalar, M24 * scalar,
			M31 * scalar, M32 * scalar, M33 * scalar, M34 * scalar,
			M41 * scalar, M42 * scalar, M43 * scalar, M44 * scalar
			);
	}

	// ***********************************************************************
	inline Matrix3D Matrix3D::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Matrix3D(
				M11 * inverse, M12 * inverse, M13 * inverse, M14 * inverse,
				M21 * inverse, M22 * inverse, M23 * inverse, M24 * inverse,
				M31 * inverse, M32 * inverse, M33 * inverse, M34 * inverse,
				M41 * inverse, M42 * inverse, M43 * inverse, M44 * inverse
				);
		}
		else
		{
			return Matrix3D(
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX
				);
		}
	}

	// ***********************************************************************
	inline Matrix3D Matrix3D::operator- () const
	{
		return Matrix3D(
			-M11, -M12, -M13, -M14,
			-M21, -M22, -M23, -M24,
			-M31, -M32, -M33, -M43,
			-M41, -M42, -M43, -M44
			);
	}

	// ***********************************************************************
	inline Vector3 operator* (const Vector3& v, const Matrix3D& m)
	{
		return Vector3(
			v.X * m.M11 + v.Y * m.M21 + v.Z * m.M31 + m.M41,
			v.X * m.M12 + v.Y * m.M22 + v.Z * m.M32 + m.M42,
			v.X * m.M13 + v.Y * m.M23 + v.Z * m.M33 + m.M43
			);
	}

	// ===================================================================================
	// Arithmetic Update Operators
	// ===================================================================================

	// ***********************************************************************
	inline Matrix3D& Matrix3D::operator*= (const Matrix3D& input)
	{
		float m11 = M11 * input.M11 + M12 * input.M21 + M13 * input.M31 + M14 * input.M41;
		float m12 = M11 * input.M12 + M12 * input.M22 + M13 * input.M32 + M14 * input.M42;
		float m13 = M11 * input.M13 + M12 * input.M23 + M13 * input.M33 + M14 * input.M43;
		float m14 = M11 * input.M14 + M12 * input.M24 + M13 * input.M34 + M14 * input.M44;

		float m21 = M21 * input.M11 + M22 * input.M21 + M23 * input.M31 + M24 * input.M41;
		float m22 = M21 * input.M12 + M22 * input.M22 + M23 * input.M32 + M24 * input.M42;
		float m23 = M21 * input.M13 + M22 * input.M23 + M23 * input.M33 + M24 * input.M43;
		float m24 = M21 * input.M14 + M22 * input.M24 + M23 * input.M34 + M24 * input.M44;

		float m31 = M31 * input.M11 + M32 * input.M21 + M33 * input.M31 + M34 * input.M41;
		float m32 = M31 * input.M12 + M32 * input.M22 + M33 * input.M32 + M34 * input.M42;
		float m33 = M31 * input.M13 + M32 * input.M23 + M33 * input.M33 + M34 * input.M43;
		float m34 = M31 * input.M14 + M32 * input.M24 + M33 * input.M34 + M34 * input.M44;

		float m41 = M41 * input.M11 + M42 * input.M21 + M43 * input.M31 + M44 * input.M41;
		float m42 = M41 * input.M12 + M42 * input.M22 + M43 * input.M32 + M44 * input.M42;
		float m43 = M41 * input.M13 + M42 * input.M23 + M43 * input.M33 + M44 * input.M43;
		float m44 = M41 * input.M14 + M42 * input.M24 + M43 * input.M34 + M44 * input.M44;
		
		M11 = m11; M12 = m12; M13 = m13; M13 = m14;
		M21 = m21; M22 = m22; M23 = m23; M24 = m24;
		M31 = m31; M32 = m32; M33 = m33; M34 = m34;
		M41 = m41; M42 = m42; M43 = m43; M44 = m44;
		return *this;
	}


	// ***********************************************************************
	inline Matrix3D& Matrix3D::operator*= (float scalar)
	{
		M11 *= scalar; M12 *= scalar; M13 *= scalar; M14 *= scalar;
		M21 *= scalar; M22 *= scalar; M23 *= scalar; M24 *= scalar;
		M31 *= scalar; M32 *= scalar; M33 *= scalar; M34 *= scalar;
		M41 *= scalar; M42 *= scalar; M43 *= scalar; M44 *= scalar;
		return *this;
	}

	// ***********************************************************************
	inline Matrix3D& Matrix3D::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			M11 *= inverse; M12 *= inverse; M13 *= inverse; M14 *= inverse; 
			M21 *= inverse; M22 *= inverse; M23 *= inverse; M24 *= inverse; 
			M31 *= inverse; M32 *= inverse; M33 *= inverse; M34 *= inverse; 
			M41 *= inverse; M42 *= inverse; M43 *= inverse; M44 *= inverse; 
		}
		else
		{
			M11 = FLT_MAX; M12 = FLT_MAX; M13 = FLT_MAX; M14 = FLT_MAX;
			M21 = FLT_MAX; M22 = FLT_MAX; M23 = FLT_MAX; M24 = FLT_MAX;
			M31 = FLT_MAX; M32 = FLT_MAX; M33 = FLT_MAX; M34 = FLT_MAX;
			M41 = FLT_MAX; M42 = FLT_MAX; M43 = FLT_MAX; M44 = FLT_MAX;
		}
		return *this;
	}

	// ===================================================================================
	// Vector Transformation Methods
	// ===================================================================================


	inline Vector3 Matrix3D::TransformVertex(const Vector3& vertex)
	{
		return Vector3(
			vertex.X * M11 + vertex.Y * M21 + vertex.Z * M31 + M41,
			vertex.X * M12 + vertex.Y * M22 + vertex.Z * M32 + M42,
			vertex.X * M13 + vertex.Y * M23 + vertex.Z * M33 + M43
			);
	}

	inline Vector3 Matrix3D::TransformNormal(const Vector3& normal)
	{
		return Vector3(
			normal.X * M11 + normal.Y * M21 + normal.Z * M31,
			normal.X * M12 + normal.Y * M22 + normal.Z * M32,
			normal.X * M13 + normal.Y * M23 + normal.Z * M33
			);
	}

	inline void Matrix3D::TransformVertexInPlace(float &x, float &y, float &z)
	{
		float tempX = x * M11 + y * M21 + z * M31 + M41;
		float tempY = x * M12 + y * M22 + z * M32 + M42;
		float tempZ = x * M13 + y * M23 + z * M33 + M43;

		x = tempX; y = tempY; z = tempZ;
	}

	inline void Matrix3D::TransformNormalInPlace(float &x, float &y, float &z)
	{
		float tempX = x * M11 + y * M21 + z * M31;
		float tempY = x * M12 + y * M22 + z * M32;
		float tempZ = x * M13 + y * M23 + z * M33;

		x = tempX; y = tempY; z = tempZ;
	}


} // namespace
