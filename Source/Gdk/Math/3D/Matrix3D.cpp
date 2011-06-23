/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Matrix3D.h"

using namespace Gdk;

// instantiations of static constants
const Matrix3D Matrix3D::ZERO(false);
const Matrix3D Matrix3D::IDENTITY(true);

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Matrix3D::Matrix3D(bool identity)
{
	if(identity)
		MakeIdentity();
	else
		MakeZero();
}

// ***********************************************************************
Matrix3D::Matrix3D(float m11, float m12, float m13, float m14, 
				   float m21, float m22, float m23, float m24, 
				   float m31, float m32, float m33, float m34,
				   float m41, float m42, float m43, float m44)
{
	M11 = m11;	M12 = m12;	M13 = m13;  M14 = m14;
	M21 = m21;	M22 = m22;	M23 = m23;  M24 = m24;
	M31 = m31;	M32 = m32;	M33 = m33;  M34 = m34;
	M41 = m41;	M42 = m42;	M43 = m43;  M44 = m44;
}

// ***********************************************************************
Matrix3D::Matrix3D(const Matrix3D& input)
{
	M11 = input.M11;	M12 = input.M12;	M13 = input.M13;	M14 = input.M14;
	M21 = input.M21;	M22 = input.M22;	M23 = input.M23;	M24 = input.M24;
	M31 = input.M31;	M32 = input.M32;	M33 = input.M33;	M34 = input.M34;
	M41 = input.M41;	M42 = input.M42;	M43 = input.M43;	M44 = input.M44;
}


// ===================================================================================
// In-Place Transformations
// ===================================================================================

// ***********************************************************************
Matrix3D& Matrix3D::MakeZero()
{
	M11 = 0.0f;		M12 = 0.0f;		M13 = 0.0f;		M14 = 0.0f;
	M21 = 0.0f;		M22 = 0.0f;		M23 = 0.0f;		M24 = 0.0f;
	M31 = 0.0f;		M32 = 0.0f;		M33 = 0.0f;		M34 = 0.0f;
	M41 = 0.0f;		M42 = 0.0f;		M43 = 0.0f;		M44 = 0.0f;
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::MakeIdentity()
{
	M11 = 1.0f;		M12 = 0.0f;		M13 = 0.0f;		M14 = 0.0f;
	M21 = 0.0f;		M22 = 1.0f;		M23 = 0.0f;		M24 = 0.0f;
	M31 = 0.0f;		M32 = 0.0f;		M33 = 1.0f;		M34 = 0.0f;
	M41 = 0.0f;		M42 = 0.0f;		M43 = 0.0f;		M44 = 1.0f;
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::Multiply(const Matrix3D& input)
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
Matrix3D& Matrix3D::Multiply(float m11, float m12, float m13, float m14, 
                             float m21, float m22, float m23, float m24, 
                             float m31, float m32, float m33, float m34,
                             float m41, float m42, float m43, float m44)
{
	float finalM11 = M11 * m11 + M12 * m21 + M13 * m31 + M14 * m41;
	float finalM12 = M11 * m12 + M12 * m22 + M13 * m32 + M14 * m42;
	float finalM13 = M11 * m13 + M12 * m23 + M13 * m33 + M14 * m43;
	float finalM14 = M11 * m14 + M12 * m24 + M13 * m34 + M14 * m44;

	float finalM21 = M21 * m11 + M22 * m21 + M23 * m31 + M24 * m41;
	float finalM22 = M21 * m12 + M22 * m22 + M23 * m32 + M24 * m42;
	float finalM23 = M21 * m13 + M22 * m23 + M23 * m33 + M24 * m43;
	float finalM24 = M21 * m14 + M22 * m24 + M23 * m34 + M24 * m44;

	float finalM31 = M31 * m11 + M32 * m21 + M33 * m31 + M34 * m41;
	float finalM32 = M31 * m12 + M32 * m22 + M33 * m32 + M34 * m42;
	float finalM33 = M31 * m13 + M32 * m23 + M33 * m33 + M34 * m43;
	float finalM34 = M31 * m14 + M32 * m24 + M33 * m34 + M34 * m44;

	float finalM41 = M41 * m11 + M42 * m21 + M43 * m31 + M44 * m41;
	float finalM42 = M41 * m12 + M42 * m22 + M43 * m32 + M44 * m42;
	float finalM43 = M41 * m13 + M42 * m23 + M43 * m33 + M44 * m43;
	float finalM44 = M41 * m14 + M42 * m24 + M43 * m34 + M44 * m44;
	
	M11 = finalM11; M12 = finalM12; M13 = finalM13; M14 = finalM14;
	M21 = finalM21; M22 = finalM22; M23 = finalM23; M24 = finalM24;
	M31 = finalM31; M32 = finalM32; M33 = finalM33; M34 = finalM34;
	M41 = finalM41; M42 = finalM42; M43 = finalM43; M44 = finalM44;
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::PreMultiply(const Matrix3D& input)
{
    float m11 = input.M11 * M11 + input.M12 * M21 + input.M13 * M31 + input.M14 * M41;
	float m12 = input.M11 * M12 + input.M12 * M22 + input.M13 * M32 + input.M14 * M42;
	float m13 = input.M11 * M13 + input.M12 * M23 + input.M13 * M33 + input.M14 * M43;
	float m14 = input.M11 * M14 + input.M12 * M24 + input.M13 * M34 + input.M14 * M44;
    
	float m21 = input.M21 * M11 + input.M22 * M21 + input.M23 * M31 + input.M24 * M41;
	float m22 = input.M21 * M12 + input.M22 * M22 + input.M23 * M32 + input.M24 * M42;
	float m23 = input.M21 * M13 + input.M22 * M23 + input.M23 * M33 + input.M24 * M43;
	float m24 = input.M21 * M14 + input.M22 * M24 + input.M23 * M34 + input.M24 * M44;
    
	float m31 = input.M31 * M11 + input.M32 * M21 + input.M33 * M31 + input.M34 * M41;
	float m32 = input.M31 * M12 + input.M32 * M22 + input.M33 * M32 + input.M34 * M42;
	float m33 = input.M31 * M13 + input.M32 * M23 + input.M33 * M33 + input.M34 * M43;
	float m34 = input.M31 * M14 + input.M32 * M24 + input.M33 * M34 + input.M34 * M44;
    
	float m41 = input.M41 * M11 + input.M42 * M21 + input.M43 * M31 + input.M44 * M41;
	float m42 = input.M41 * M12 + input.M42 * M22 + input.M43 * M32 + input.M44 * M42;
	float m43 = input.M41 * M13 + input.M42 * M23 + input.M43 * M33 + input.M44 * M43;
	float m44 = input.M41 * M14 + input.M42 * M24 + input.M43 * M34 + input.M44 * M44;
    
	M11 = m11; M12 = m12; M13 = m13; M13 = m14;
	M21 = m21; M22 = m22; M23 = m23; M24 = m24;
	M31 = m31; M32 = m32; M33 = m33; M34 = m34;
	M41 = m41; M42 = m42; M43 = m43; M44 = m44;
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::PreMultiply(float m11, float m12, float m13, float m14, 
                                float m21, float m22, float m23, float m24, 
                                float m31, float m32, float m33, float m34,
                                float m41, float m42, float m43, float m44)
{
    float finalM11 = m11 * M11 + m12 * M21 + m13 * M31 + m14 * M41;
	float finalM12 = m11 * M12 + m12 * M22 + m13 * M32 + m14 * M42;
	float finalM13 = m11 * M13 + m12 * M23 + m13 * M33 + m14 * M43;
	float finalM14 = m11 * M14 + m12 * M24 + m13 * M34 + m14 * M44;
    
	float finalM21 = m21 * M11 + m22 * M21 + m23 * M31 + m24 * M41;
	float finalM22 = m21 * M12 + m22 * M22 + m23 * M32 + m24 * M42;
	float finalM23 = m21 * M13 + m22 * M23 + m23 * M33 + m24 * M43;
	float finalM24 = m21 * M14 + m22 * M24 + m23 * M34 + m24 * M44;
    
	float finalM31 = m31 * M11 + m32 * M21 + m33 * M31 + m34 * M41;
	float finalM32 = m31 * M12 + m32 * M22 + m33 * M32 + m34 * M42;
	float finalM33 = m31 * M13 + m32 * M23 + m33 * M33 + m34 * M43;
	float finalM34 = m31 * M14 + m32 * M24 + m33 * M34 + m34 * M44;
    
	float finalM41 = m41 * M11 + m42 * M21 + m43 * M31 + m44 * M41;
	float finalM42 = m41 * M12 + m42 * M22 + m43 * M32 + m44 * M42;
	float finalM43 = m41 * M13 + m42 * M23 + m43 * M33 + m44 * M43;
	float finalM44 = m41 * M14 + m42 * M24 + m43 * M34 + m44 * M44;
    
	M11 = finalM11; M12 = finalM12; M13 = finalM13; M14 = finalM14;
	M21 = finalM21; M22 = finalM22; M23 = finalM23; M24 = finalM24;
	M31 = finalM31; M32 = finalM32; M33 = finalM33; M34 = finalM34;
	M41 = finalM41; M42 = finalM42; M43 = finalM43; M44 = finalM44;
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::Translate(const Vector3& translation)
{
	Multiply(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translation.X, translation.Y, translation.Z, 1.0f
		);
	return *this;
}


// ***********************************************************************
Matrix3D& Matrix3D::Translate(float x, float y, float z)
{
	Multiply(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x,    y,    z,    1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::Scale(float scalar)
{
	Multiply(
		scalar, 0.0f, 0.0f, 0.0f,
		0.0f, scalar, 0.0f, 0.0f, 
		0.0f, 0.0f, scalar, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::Scale(float sx, float sy, float sz)
{
	Multiply(
		sx,   0.0f, 0.0f, 0.0f,
		0.0f, sy,   0.0f, 0.0f, 
		0.0f, 0.0f, sz,   0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::RotateX(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	Multiply(
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, cosTheta, sinTheta, 0.0f,
		0.0f, -sinTheta, cosTheta, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::RotateY(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	Multiply(
		cosTheta, 0.0f, -sinTheta, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
		sinTheta, 0.0f, cosTheta, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::RotateZ(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	Multiply(
		cosTheta, sinTheta, 0.0f, 0.0f, 
		-sinTheta, cosTheta, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::RotateOnAxis(const Vector3& axis, float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	// Pre-calculate some values
	float x = axis.X;
	float y = axis.Y;
	float z = axis.Z;
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;

	Multiply(
		xx + (cosTheta * (1.0f - xx)),
		(xy - (cosTheta * xy)) + (sinTheta * z),
		(xz - (cosTheta * xz)) - (sinTheta * y),
		0.0f,
		(xy - (cosTheta * xy)) - (sinTheta * z),
		yy + (cosTheta * (1.0f - yy)),
		(yz - (cosTheta * yz)) + (sinTheta * x),
		0.0f,
		(xz - (cosTheta * xz)) + (sinTheta * y),
		(yz - (cosTheta * yz)) - (sinTheta * x),
		zz + (cosTheta * (1.0f - zz)),
		0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::RotateQuaternion(const class Quaternion& quat)
{
	// Compute a bunch of intermediate float vales
	float x2 = 2.0f * quat.X;
	float y2 = 2.0f * quat.Y;
	float z2 = 2.0f * quat.Z;
	float wx2 = x2 * quat.W;
	float wy2 = y2 * quat.W;
	float wz2 = z2 * quat.W;
	float xx2 = x2 * quat.X;
	float xy2 = y2 * quat.X;
	float xz2 = z2 * quat.X;
	float yy2 = y2 * quat.Y;
	float yz2 = z2 * quat.Y;
	float zz2 = z2 * quat.Z;

	// Multiply in the given matrix
	Multiply(
		1.0f - (yy2 + zz2),		xy2 - wz2,				xz2 + wy2,				0.0f,
		xy2 + wz2,				1.0f - (xx2 + zz2),		yz2 - wx2,				0.0f,
		xz2 - wy2,				yz2 + wx2,				1.0f - (xx2 + yy2),		0.0f,
		0.0f,					0.0f,					0.0f,					1.0f
		);

	return *this;
}

// ***********************************************************************
Matrix3D& Matrix3D::Reflect(const class Plane3& plane)
{
	// Assumes the plane normal, is normalized...
	float x = plane.Normal.X;
	float y = plane.Normal.Y;
	float z = plane.Normal.Z;
	float n2x = -2.0f * plane.Normal.X;
	float n2y = -2.0f * plane.Normal.Y;
	float n2z = -2.0f * plane.Normal.Z;

	Multiply(
		n2x * x + 1.0f, n2y * x, n2z * x, 0.0f,
		n2x * y, n2y * y + 1.0f, n2z * y, 0.0f,
		n2x * z, n2y * z, n2z * z + 1.0f, 0.0f,
		n2x * plane.D, n2y * plane.D, n2z * plane.D, 1.0f
		);

	return *this;
}

// ===================================================================================
// Static Creation Methods
// ===================================================================================

// ***********************************************************************
Matrix3D Matrix3D::CreateTranslation(const Vector3& translation)
{
	return Matrix3D(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translation.X, translation.Y, translation.Z, 1.0f
        );
}


// ***********************************************************************
Matrix3D Matrix3D::CreateTranslation(float x, float y, float z)
{
	return Matrix3D(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x,    y,    z,    1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateScale(float scalar)
{
	return Matrix3D(
        scalar, 0.0f, 0.0f, 0.0f,
        0.0f, scalar, 0.0f, 0.0f, 
        0.0f, 0.0f, scalar, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateScale(float sx, float sy, float sz)
{
	return Matrix3D(
        sx,   0.0f, 0.0f, 0.0f,
        0.0f, sy,   0.0f, 0.0f, 
        0.0f, 0.0f, sz,   0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateRotationX(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);
    
	return Matrix3D(
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, cosTheta, sinTheta, 0.0f,
        0.0f, -sinTheta, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateRotationY(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);
    
	return Matrix3D(
        cosTheta, 0.0f, -sinTheta, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        sinTheta, 0.0f, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateRotationZ(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);
    
	return Matrix3D(
        cosTheta, sinTheta, 0.0f, 0.0f, 
        -sinTheta, cosTheta, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateRotationOnAxis(const Vector3& axis, float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);
    
	// Pre-calculate some values
	float x = axis.X;
	float y = axis.Y;
	float z = axis.Z;
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
    
	return Matrix3D(
        xx + (cosTheta * (1.0f - xx)),
        (xy - (cosTheta * xy)) + (sinTheta * z),
        (xz - (cosTheta * xz)) - (sinTheta * y),
        0.0f,
        (xy - (cosTheta * xy)) - (sinTheta * z),
        yy + (cosTheta * (1.0f - yy)),
        (yz - (cosTheta * yz)) + (sinTheta * x),
        0.0f,
        (xz - (cosTheta * xz)) + (sinTheta * y),
        (yz - (cosTheta * yz)) - (sinTheta * x),
        zz + (cosTheta * (1.0f - zz)),
        0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateRotationFromQuaternion(const class Quaternion& quat)
{
	// Compute a bunch of intermediate float vales
	float x2 = 2.0f * quat.X;
	float y2 = 2.0f * quat.Y;
	float z2 = 2.0f * quat.Z;
	float wx2 = x2 * quat.W;
	float wy2 = y2 * quat.W;
	float wz2 = z2 * quat.W;
	float xx2 = x2 * quat.X;
	float xy2 = y2 * quat.X;
	float xz2 = z2 * quat.X;
	float yy2 = y2 * quat.Y;
	float yz2 = z2 * quat.Y;
	float zz2 = z2 * quat.Z;
    
	// Multiply in the given matrix
	return Matrix3D(
        1.0f - (yy2 + zz2),		xy2 - wz2,				xz2 + wy2,				0.0f,
        xy2 + wz2,				1.0f - (xx2 + zz2),		yz2 - wx2,				0.0f,
        xz2 - wy2,				yz2 + wx2,				1.0f - (xx2 + yy2),		0.0f,
        0.0f,					0.0f,					0.0f,					1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateReflection(const class Plane3& plane)
{
	// Assumes the plane normal, is normalized...
	float x = plane.Normal.X;
	float y = plane.Normal.Y;
	float z = plane.Normal.Z;
	float n2x = -2.0f * plane.Normal.X;
	float n2y = -2.0f * plane.Normal.Y;
	float n2z = -2.0f * plane.Normal.Z;
    
	return Matrix3D(
        n2x * x + 1.0f, n2y * x, n2z * x, 0.0f,
        n2x * y, n2y * y + 1.0f, n2z * y, 0.0f,
        n2x * z, n2y * z, n2z * z + 1.0f, 0.0f,
        n2x * plane.D, n2y * plane.D, n2z * plane.D, 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
	// Get the direction vector
	Vector3 direction = position - target;
	direction.Normalize();
    
	// Get the true up & right vectors
	Vector3 right = up.Cross(direction);
	right.Normalize();
	Vector3 trueUp = direction.Cross(right);
	trueUp.Normalize();
    
	return Matrix3D(
        right.X, trueUp.X, direction.X, 0.0f,
        right.Y, trueUp.Y, direction.Y, 0.0f,
        right.Z, trueUp.Z, direction.Z, 0.0f,
        -right.Dot(position), -trueUp.Dot(position), -direction.Dot(position), 1.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreatePerspectiveFOV(float fov, float aspectRatio, float nearDistance, float farDistance)
{
	// Assume:  0 < fov < PI
	// Assume:  near > 0
	// Assume:  far > 0
	// Assume:  far > near
    
	float yAdjust = 1.0f / Math::Tan(fov * 0.5f);
	float xAdjust = yAdjust / aspectRatio;
    
	return Matrix3D(
        xAdjust, 0.0f, 0.0f, 0.0f,
        0.0f, yAdjust, 0.0f, 0.0f,
        0.0f, 0.0f, 
        (farDistance / (nearDistance - farDistance)), 
        -1.0f,
        0.0f, 0.0f, (nearDistance * farDistance) / (nearDistance - farDistance), 0.0f
        );
}

// ***********************************************************************
Matrix3D Matrix3D::CreateOrtho(float width, float height, float nearPlane, float farPlane)
{
	return Matrix3D(
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f / (nearPlane - farPlane), 0.0f,
        0.0f, 0.0f, nearPlane / (nearPlane - farPlane), 1.0f
        );
}


// ***********************************************************************
Matrix3D Matrix3D::CreateOrthoOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	float invHalfX = 2.0f / (right - left);
	float invHalfY = 2.0f / (top - bottom);
	float invZ = 1.0f / (nearPlane - farPlane);
    
	return Matrix3D(
        invHalfX, 0.0f, 0.0f, 0.0f,
        0.0f, invHalfY, 0.0f, 0.0f,
        0.0f, 0.0f, invZ, 0.0f,
        (left + right) / (left - right), (top + bottom) / (bottom - top), nearPlane / (nearPlane - farPlane), 1.0f
        );
}



// ===================================================================================
// Extra Matrix Methods
// ===================================================================================

// ***********************************************************************
Matrix3D Matrix3D::GetTranspose()
{
	return Matrix3D(
		M11, M21, M31, M41, 
		M12, M22, M32, M42, 
		M13, M23, M33, M43, 
		M14, M24, M34, M44
		);
}

// ***********************************************************************
Matrix3D Matrix3D::GetInverse()
{
	float a0 = M11*M22 - M12*M21;
    float a1 = M11*M23 - M13*M21;
    float a2 = M11*M24 - M14*M21;
    float a3 = M12*M23 - M13*M22;
    float a4 = M12*M24 - M14*M22;
    float a5 = M13*M24 - M14*M23;
    float b0 = M31*M42 - M32*M41;
    float b1 = M31*M43 - M33*M41;
    float b2 = M31*M44 - M34*M41;
    float b3 = M32*M43 - M33*M42;
    float b4 = M32*M44 - M34*M42;
    float b5 = M33*M44 - M34*M43;

    float det = a0*b5-a1*b4+a2*b3+a3*b2-a4*b1+a5*b0;
	if (Math::Abs(det) <= Math::ZERO_TOLERANCE)
    {
        return Matrix3D::ZERO;
    }

    Matrix3D result;
    result.M11 = + M22*b5 - M23*b4 + M24*b3;
    result.M21 = - M21*b5 + M23*b2 - M24*b1;
    result.M31 = + M21*b4 - M22*b2 + M24*b0;
    result.M41 = - M21*b3 + M22*b1 - M23*b0;
    result.M12 = - M12*b5 + M13*b4 - M14*b3;
    result.M22 = + M11*b5 - M13*b2 + M14*b1;
    result.M32 = - M11*b4 + M12*b2 - M14*b0;
    result.M42 = + M11*b3 - M12*b1 + M13*b0;
    result.M13 = + M42*a5 - M43*a4 + M44*a3;
    result.M23 = - M41*a5 + M43*a2 - M44*a1;
    result.M33 = + M41*a4 - M42*a2 + M44*a0;
    result.M43 = - M41*a3 + M42*a1 - M43*a0;
    result.M14 = - M32*a5 + M33*a4 - M34*a3;
    result.M24 = + M31*a5 - M33*a2 + M34*a1;
    result.M34 = - M31*a4 + M32*a2 - M34*a0;
    result.M44 = + M31*a3 - M32*a1 + M33*a0;

    float inverseDet = 1.0f / det;
    result *= inverseDet;
	return result;
}

// ***********************************************************************
float Matrix3D::GetDeterminant()
{
	float a0 = M11*M22 - M12*M21;
    float a1 = M11*M23 - M13*M21;
    float a2 = M11*M24 - M14*M21;
    float a3 = M12*M23 - M13*M22;
    float a4 = M12*M24 - M14*M22;
    float a5 = M13*M24 - M14*M23;
    float b0 = M31*M42 - M32*M41;
    float b1 = M31*M43 - M33*M41;
    float b2 = M31*M44 - M34*M41;
    float b3 = M32*M43 - M33*M42;
    float b4 = M32*M44 - M34*M42;
    float b5 = M33*M44 - M34*M43;

    return ( a0*b5-a1*b4+a2*b3+a3*b2-a4*b1+a5*b0 );
}
