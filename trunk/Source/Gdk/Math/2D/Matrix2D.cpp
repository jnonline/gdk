/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Matrix2D.h"

using namespace Gdk;

// instantiations of static constants
const Matrix2D Matrix2D::ZERO(false);
const Matrix2D Matrix2D::IDENTITY(true);

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Matrix2D::Matrix2D(bool identity)
{
	if(identity)
		MakeIdentity();
}

// ***********************************************************************
Matrix2D::Matrix2D(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
{
	M11 = m11;	M12 = m12;	M13 = m13;
	M21 = m21;	M22 = m22;	M23 = m23;
	M31 = m31;	M32 = m32;	M33 = m33;
}

// ***********************************************************************
Matrix2D::Matrix2D(const Matrix2D& input)
{
	M11 = input.M11;	M12 = input.M12;	M13 = input.M13;
	M21 = input.M21;	M22 = input.M22;	M23 = input.M23;
	M31 = input.M31;	M32 = input.M32;	M33 = input.M33;
}


// ===================================================================================
// In-Place Transformations
// ===================================================================================

// ***********************************************************************
Matrix2D& Matrix2D::MakeZero()
{
	M11 = 0.0f;		M12 = 0.0f;		M13 = 0.0f;
	M21 = 0.0f;		M22 = 0.0f;		M23 = 0.0f;
	M31 = 0.0f;		M32 = 0.0f;		M33 = 0.0f;
	return *this;
}

// ***********************************************************************
Matrix2D& Matrix2D::MakeIdentity()
{
	M11 = 1.0f;		M12 = 0.0f;		M13 = 0.0f;
	M21 = 0.0f;		M22 = 1.0f;		M23 = 0.0f;
	M31 = 0.0f;		M32 = 0.0f;		M33 = 1.0f;
	return *this;
}

// ***********************************************************************
Matrix2D& Matrix2D::Multiply(const Matrix2D& input)
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
Matrix2D& Matrix2D::Multiply(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
{
	float finalM11 = M11 * m11 + M12 * m21 + M13 * m31;
	float finalM12 = M11 * m12 + M12 * m22 + M13 * m32;
	float finalM13 = M11 * m13 + M12 * m23 + M13 * m33;
	float finalM21 = M21 * m11 + M22 * m21 + M23 * m31;
	float finalM22 = M21 * m12 + M22 * m22 + M23 * m32;
	float finalM23 = M21 * m13 + M22 * m23 + M23 * m33;
	float finalM31 = M31 * m11 + M32 * m21 + M33 * m31;
	float finalM32 = M31 * m12 + M32 * m22 + M33 * m32;
	float finalM33 = M31 * m13 + M32 * m23 + M33 * m33;
	
	M11 = finalM11; M12 = finalM12; M13 = finalM13;
	M21 = finalM21; M22 = finalM22; M23 = finalM23;
	M31 = finalM31; M32 = finalM32; M33 = finalM33;
	return *this;
}

// ***********************************************************************
Matrix2D& Matrix2D::Translate(float x, float y)
{
	Multiply(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		x, y, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix2D& Matrix2D::Scale(float sx, float sy)
{
	Multiply(
		sx,   0.0f, 0.0f,
		0.0f, sy,   0.0f,
		0.0f, 0.0f, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix2D& Matrix2D::Rotate(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	Multiply(
		cosTheta, sinTheta, 0.0f,
		-sinTheta, cosTheta, 0.0f,
		0.0f, 0.0f, 1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix2D& Matrix2D::ScaleRotateTranslate(float sx, float sy, float tx, float ty, float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	Multiply(
		sx * cosTheta,	sx * sinTheta,	0.0f,
		sy * -sinTheta, sy * cosTheta,	0.0f,
		tx,				ty,				1.0f
		);
	return *this;
}

// ***********************************************************************
Matrix2D& Matrix2D::RotateScaleTranslate(float sx, float sy, float tx, float ty, float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	Multiply(
		sx * cosTheta,	sy * sinTheta,	0.0f,
		sx * -sinTheta, sy * cosTheta,	0.0f,
		tx,				ty,				1.0f
		);
	return *this;
}

// ===================================================================================
// Static Creation Methods
// ===================================================================================

// ***********************************************************************
Matrix2D Matrix2D::CreateTranslation(float tx, float ty)
{
	Matrix2D result(false);
	CreateTranslation(tx, ty, result);
	return result;
}

// ***********************************************************************
Matrix2D Matrix2D::CreateScale(float sx, float sy)
{
	Matrix2D result(false);
	CreateScale(sx, sy, result);
	return result;
}

// ***********************************************************************
Matrix2D Matrix2D::CreateRotation(float angle)
{
	Matrix2D result(false);
	CreateRotation(angle, result);
	return result;
}

// ***********************************************************************
Matrix2D Matrix2D::CreateScaleRotateTranslate(float sx, float sy, float tx, float ty, float angle)
{
	Matrix2D result(false);
	CreateRotateScaleTranslate(sx, sy, tx, ty, angle, result);
	return result;
}

// ***********************************************************************
Matrix2D Matrix2D::CreateRotateScaleTranslate(float sx, float sy, float tx, float ty, float angle)
{
	Matrix2D result(false);
	CreateRotateScaleTranslate(sx, sy, tx, ty, angle, result);
	return result;
}

// ===================================================================================
// Static In-Place Creation Methods
// ===================================================================================

// ***********************************************************************
void Matrix2D::CreateTranslation(float x, float y, Matrix2D& result)
{
	result.Set(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		x, y, 1.0f
		);
}

// ***********************************************************************
void Matrix2D::CreateScale(float sx, float sy, Matrix2D& result)
{
	result.Set(
		sx,   0.0f, 0.0f,
		0.0f, sy,   0.0f,
		0.0f, 0.0f, 1.0f
		);
}

// ***********************************************************************
void Matrix2D::CreateRotation(float angle, Matrix2D& result)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	result.Set(
		cosTheta, sinTheta, 0.0f,
		-sinTheta, cosTheta, 0.0f,
		0.0f, 0.0f, 1.0f
		);
}

// ***********************************************************************
void Matrix2D::CreateScaleRotateTranslate(float sx, float sy, float tx, float ty, float angle, Matrix2D& result)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	result.Set(
		sx * cosTheta,	sx * sinTheta,	0.0f,
		sy * -sinTheta, sy * cosTheta,	0.0f,
		tx,				ty,				1.0f
		);
}

// ***********************************************************************
void Matrix2D::CreateRotateScaleTranslate(float sx, float sy, float tx, float ty, float angle, Matrix2D& result)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	result.Set(
		sx * cosTheta,	sy * sinTheta,	0.0f,
		sx * -sinTheta, sy * cosTheta,	0.0f,
		tx,				ty,				1.0f
		);
}

// ===================================================================================
// Extra Matrix Methods
// ===================================================================================

// ***********************************************************************
Matrix2D Matrix2D::GetTranspose()
{
	return Matrix2D(
		M11, M21, M31,
		M12, M22, M32,
		M13, M23, M33
		);
}

// ***********************************************************************
Matrix2D Matrix2D::GetInverse()
{
	Matrix2D inverse;

	inverse.M11 = M22 * M33 - M23 * M32;
	inverse.M12 = M13 * M32 - M12 * M33;
	inverse.M13 = M12 * M23 - M13 * M22;
	inverse.M21 = M23 * M31 - M21 * M33;
	inverse.M22 = M11 * M33 - M13 * M31;
	inverse.M23 = M13 * M21 - M11 * M23;
	inverse.M31 = M21 * M32 - M22 * M31;
	inverse.M32 = M12 * M31 - M11 * M32;
	inverse.M33 = M11 * M22 - M12 * M21;

	float det = 
		M11 * inverse.M11 + 
		M12 * inverse.M21 + 
		M13 * inverse.M31;

	if(Math::Abs(det) <= Math::ZERO_TOLERANCE)
		return Matrix2D::ZERO;

	float invDet = 1.0f / det;
	inverse *= invDet;

	return inverse;
}

// ***********************************************************************
float Matrix2D::GetDeterminant()
{
	return
		M11 * (M22 * M33 - M23 * M32) +
		M12 * (M23 * M31 - M21 * M33) +
		M13 * (M21 * M32 - M22 * M31);
}

// ***********************************************************************
bool Matrix2D::IsIdentity()
{
	if (M11 == 1.0f && M12 == 0.0f && M13 == 0.0f &&
		M21 == 0.0f && M22 == 1.0f && M23 == 0.0f &&
		M31 == 0.0f && M32 == 0.0f && M33 == 1.0f )
		return true;
	return false;
}