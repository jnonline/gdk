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

// *****************************************************************
/// @brief
///     Constructs a new matrix.
/// @param identity
///     If true (default), the matrix values will be set to the identity matrix.  
///     Otherwise the matrix values will be undefined.
// *****************************************************************
Matrix2D::Matrix2D(bool identity)
{
	if(identity)
		MakeIdentity();
}

// *****************************************************************
/// @brief
///     Constructs a new matrix from a set of values
// *****************************************************************
Matrix2D::Matrix2D(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
{
	M11 = m11;	M12 = m12;	M13 = m13;
	M21 = m21;	M22 = m22;	M23 = m23;
	M31 = m31;	M32 = m32;	M33 = m33;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Matrix2D::Matrix2D(const Matrix2D& input)
{
	M11 = input.M11;	M12 = input.M12;	M13 = input.M13;
	M21 = input.M21;	M22 = input.M22;	M23 = input.M23;
	M31 = input.M31;	M32 = input.M32;	M33 = input.M33;
}


// *****************************************************************
/// @brief
///     Sets all the values of the matrix to 0
// *****************************************************************
Matrix2D& Matrix2D::MakeZero()
{
	M11 = 0.0f;		M12 = 0.0f;		M13 = 0.0f;
	M21 = 0.0f;		M22 = 0.0f;		M23 = 0.0f;
	M31 = 0.0f;		M32 = 0.0f;		M33 = 0.0f;
	return *this;
}

// *****************************************************************
/// @brief
///     Sets the values of the matrix to be the identity matrix
// *****************************************************************
Matrix2D& Matrix2D::MakeIdentity()
{
	M11 = 1.0f;		M12 = 0.0f;		M13 = 0.0f;
	M21 = 0.0f;		M22 = 1.0f;		M23 = 0.0f;
	M31 = 0.0f;		M32 = 0.0f;		M33 = 1.0f;
	return *this;
}

// *****************************************************************
/// @brief
///     Multiplies this matrix with a given matrix
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Multiplies this matrix with a given matrix (as direct values)
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Multiplies a given matrix by this matrix
// *****************************************************************
Matrix2D& Matrix2D::PreMultiply(const Matrix2D& input)
{
    float m11 = input.M11 * M11 + input.M12 * M21 + input.M13 * M31;
	float m12 = input.M11 * M12 + input.M12 * M22 + input.M13 * M32;
	float m13 = input.M11 * M13 + input.M12 * M23 + input.M13 * M33;
	float m21 = input.M21 * M11 + input.M22 * M21 + input.M23 * M31;
	float m22 = input.M21 * M12 + input.M22 * M22 + input.M23 * M32;
	float m23 = input.M21 * M13 + input.M22 * M23 + input.M23 * M33;
	float m31 = input.M31 * M11 + input.M32 * M21 + input.M33 * M31;
	float m32 = input.M31 * M12 + input.M32 * M22 + input.M33 * M32;
	float m33 = input.M31 * M13 + input.M32 * M23 + input.M33 * M33;
	
	M11 = m11; M12 = m12; M13 = m13;
	M21 = m21; M22 = m22; M23 = m23;
	M31 = m31; M32 = m32; M33 = m33;
	return *this;
}

// *****************************************************************
/// @brief
///     Multiplies a given matrix (as direct values) with this matrix
// *****************************************************************
Matrix2D& Matrix2D::PreMultiply(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
{
    float finalM11 = m11 * M11 + m12 * M21 + m13 * M31;
	float finalM12 = m11 * M12 + m12 * M22 + m13 * M32;
	float finalM13 = m11 * M13 + m12 * M23 + m13 * M33;
	float finalM21 = m21 * M11 + m22 * M21 + m23 * M31;
	float finalM22 = m21 * M12 + m22 * M22 + m23 * M32;
	float finalM23 = m21 * M13 + m22 * M23 + m23 * M33;
	float finalM31 = m31 * M11 + m32 * M21 + m33 * M31;
	float finalM32 = m31 * M12 + m32 * M22 + m33 * M32;
	float finalM33 = m31 * M13 + m32 * M23 + m33 * M33;
	
	M11 = finalM11; M12 = finalM12; M13 = finalM13;
	M21 = finalM21; M22 = finalM22; M23 = finalM23;
	M31 = finalM31; M32 = finalM32; M33 = finalM33;
	return *this;
}

// *****************************************************************
/// @brief
///     Applies a translation to this matrix's current transform
/// @param translation
///     A direction vector representing the translation
// *****************************************************************
Matrix2D& Matrix2D::Translate(const Vector2& translation)
{
	Multiply(
             1.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             translation.X, translation.Y, 1.0f
             );
	return *this;
}

// *****************************************************************
/// @brief
///     Applies a translation to this matrix's current transform
/// @param x
///     X-component of the translation vector
/// @param y
///     Y-component of the translation vector
// *****************************************************************
Matrix2D& Matrix2D::Translate(float x, float y)
{
	Multiply(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		x, y, 1.0f
		);
	return *this;
}

// *****************************************************************
/// @brief
///     Applies uniform scaling to this matrix's current transform
/// @param scale
///     The uniform scale factor
// *****************************************************************
Matrix2D& Matrix2D::Scale(float scale)
{
	Multiply(
             scale,   0.0f, 0.0f,
             0.0f, scale,   0.0f,
             0.0f, 0.0f, 1.0f
             );
	return *this;
}

// *****************************************************************
/// @brief
///     Applies non-uniform scaling to this matrix's current transform
/// @param sx
///     The X-component of the non-uniform scale factor
/// @param sy
///     The Y-component of the non-uniform scale factor
// *****************************************************************
Matrix2D& Matrix2D::Scale(float sx, float sy)
{
	Multiply(
		sx,   0.0f, 0.0f,
		0.0f, sy,   0.0f,
		0.0f, 0.0f, 1.0f
		);
	return *this;
}

// *****************************************************************
/// @brief
///     Applies a rotation to this matrix's current transform
/// @param angle
///     The angle of the rotation
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Applies scaling, rotation, and translation to this matrix's current transform
/// @remarks
///     The transformations are applied in the exact order as defined by the method name:
///     Scaling, Rotation, then Translation.
/// @param sx
///     The X-component of the non-uniform scale factor
/// @param sy
///     The Y-component of the non-uniform scale factor
/// @param tx
///     The X-component of the translation
/// @param ty
///     The Y-component of the translation
/// @param angle
///     The angle of the rotation
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Applies rotation, scaling, and translation to this matrix's current transform
/// @remarks
///     The transformations are applied in the exact order as defined by the method name:
///     Rotation, Scaling, then Translation.
/// @param sx
///     The X-component of the non-uniform scale factor
/// @param sy
///     The Y-component of the non-uniform scale factor
/// @param tx
///     The X-component of the translation
/// @param ty
///     The Y-component of the translation
/// @param angle
///     The angle of the rotation
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Creates a transformation matrix that applies a translation
/// @param translation
///     A direction vector representing the translation
// *****************************************************************
Matrix2D Matrix2D::CreateTranslation(const Vector2& translation)
{
	return Matrix2D(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		translation.X, translation.Y, 1.0f
		);
}

// *****************************************************************
/// @brief
///     Creates a transformation matrix that applies a translation
/// @param x
///     X-component of the translation vector
/// @param y
///     Y-component of the translation vector
// *****************************************************************
Matrix2D Matrix2D::CreateTranslation(float x, float y)
{
	return Matrix2D(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        x,    y,    1.0f
        );
}

// *****************************************************************
/// @brief
///     Creates a transformation matrix that applies uniform scaling
/// @param scale
///     The uniform scale factor
// *****************************************************************
Matrix2D Matrix2D::CreateScale(float scale)
{
	return Matrix2D(
        scale, 0.0f,  0.0f,
        0.0f,  scale, 0.0f,
        0.0f,  0.0f,  1.0f
        );
}

// *****************************************************************
/// @brief
///     Creates a transformation matrix that applies non-uniform scaling
/// @param sx
///     The X-component of the non-uniform scale factor
/// @param sy
///     The Y-component of the non-uniform scale factor
// *****************************************************************
Matrix2D Matrix2D::CreateScale(float sx, float sy)
{
	return Matrix2D(
		sx,   0.0f, 0.0f,
		0.0f, sy,   0.0f,
		0.0f, 0.0f, 1.0f
		);
}

// *****************************************************************
/// @brief
///     Creates a transformation matrix that applies a rotation
/// @param angle
///     The angle of the rotation
// *****************************************************************
Matrix2D Matrix2D::CreateRotation(float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	return Matrix2D(
		cosTheta, sinTheta, 0.0f,
		-sinTheta, cosTheta, 0.0f,
		0.0f, 0.0f, 1.0f
		);
}

// *****************************************************************
/// @brief
///     Creates a transformation matrix that applies scaling, rotation, and translation.
/// @remarks
///     The transformations are applied in the exact order as defined by the method name:
///     Scaling, Rotation, then Translation.
/// @param sx
///     The X-component of the non-uniform scale factor
/// @param sy
///     The Y-component of the non-uniform scale factor
/// @param tx
///     The X-component of the translation
/// @param ty
///     The Y-component of the translation
/// @param angle
///     The angle of the rotation
// *****************************************************************
Matrix2D Matrix2D::CreateScaleRotateTranslate(float sx, float sy, float tx, float ty, float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	return Matrix2D(
		sx * cosTheta,	sx * sinTheta,	0.0f,
		sy * -sinTheta, sy * cosTheta,	0.0f,
		tx,				ty,				1.0f
		);
}

// *****************************************************************
/// @brief
///     Creates a transformation matrix that applies rotation, scaling, and translation.
/// @remarks
///     The transformations are applied in the exact order as defined by the method name:
///     Rotation, Scaling, then Translation.
/// @param sx
///     The X-component of the non-uniform scale factor
/// @param sy
///     The Y-component of the non-uniform scale factor
/// @param tx
///     The X-component of the translation
/// @param ty
///     The Y-component of the translation
/// @param angle
///     The angle of the rotation
// *****************************************************************
Matrix2D Matrix2D::CreateRotateScaleTranslate(float sx, float sy, float tx, float ty, float angle)
{
	float sinTheta = Math::Sin(angle);
	float cosTheta = Math::Cos(angle);

	return Matrix2D(
		sx * cosTheta,	sy * sinTheta,	0.0f,
		sx * -sinTheta, sy * cosTheta,	0.0f,
		tx,				ty,				1.0f
		);
}

// *****************************************************************
/// @brief
///     Calculates the transpose of this matrix
// *****************************************************************
Matrix2D Matrix2D::GetTranspose()
{
	return Matrix2D(
		M11, M21, M31,
		M12, M22, M32,
		M13, M23, M33
		);
}

// *****************************************************************
/// @brief
///     Calculates the inverse of this matrix
/// @remarks
///     If the matrix is non-invertable, the Zero matrix will be returned instead.
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Calculates the determinant of this matrix
// *****************************************************************

float Matrix2D::GetDeterminant()
{
	return
		M11 * (M22 * M33 - M23 * M32) +
		M12 * (M23 * M31 - M21 * M33) +
		M13 * (M21 * M32 - M22 * M31);
}

// *****************************************************************
/// @brief
///     Checks if this matrix is the identity matrix
// *****************************************************************
bool Matrix2D::IsIdentity()
{
	if (M11 == 1.0f && M12 == 0.0f && M13 == 0.0f &&
		M21 == 0.0f && M22 == 1.0f && M23 == 0.0f &&
		M31 == 0.0f && M32 == 0.0f && M33 == 1.0f )
		return true;
	return false;
}

// *****************************************************************
/// @brief
///     Reads a Matrix2D from a stream
// *****************************************************************
Matrix2D Matrix2D::ReadFromStream(Stream* stream)
{
	Matrix2D result;
	stream->Read(&result, sizeof(float[9]));
	return result;
}

// *****************************************************************
/// @brief
///     Writes this matrix to a stream
// *****************************************************************
void Matrix2D::WriteToStream(Stream* stream)
{
	stream->Write(this, sizeof(float[9]));
}