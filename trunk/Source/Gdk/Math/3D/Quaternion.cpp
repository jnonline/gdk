/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Quaternion.h"

using namespace Gdk;

// instantiations of static constants
const Quaternion Quaternion::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Quaternion::Quaternion()
{
}

// *****************************************************************
/// @brief
///     Constructs a quaternion from the given component values
// *****************************************************************
Quaternion::Quaternion(float w, float x, float y, float z)
{
	W = w;
	X = x;
	Y = y;
	Z = z;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Quaternion::Quaternion(const Quaternion& input)
{
	W = input.W;
	X = input.X;
	Y = input.Y;
	Z = input.Z;
}

// *****************************************************************
/// @brief
///     Calculates the inverse of this quaternion
// *****************************************************************
Quaternion Quaternion::Inverse() const
{
	float lengthSquared = this->LengthSquared();
	if(lengthSquared > 0.0f)
	{
		float inverseLengthSquared = 1.0f / lengthSquared;
		return Quaternion(
			W * inverseLengthSquared,
			-X * inverseLengthSquared, 
			-Y * inverseLengthSquared,
			-Z * inverseLengthSquared
			);
	}

	// Return the zero quaternion
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

// *****************************************************************
/// @brief
///     Calculates the Conjugate of this quaternion
// *****************************************************************
Quaternion Quaternion::Conjugate() const
{
	return Quaternion(W, -X, -Y, -Z);
}

// *****************************************************************
/// @brief
///     Calculates the axis and angle of rotation that represents this quaternion
/// @param[out] axis
///     This axis of rotation
/// @return
///     The angle of rotation, in radians.
// *****************************************************************
float Quaternion::ToAxisAngle(Vector3& axis) const
{
	// The quaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	float angle = 0.0f;
	float lengthSquared = X*X + Y*Y + Z*Z;
	if(lengthSquared > Math::ZERO_TOLERANCE)
	{
		angle = 2.0f * Math::ACos(W);
		float inverseLength = 1.0f / Math::Sqrt(lengthSquared);
		axis.X = X * inverseLength;
		axis.Y = Y * inverseLength;
		axis.Z = Z * inverseLength;
	}
    else
    {
		// Angle is 0, so axis doesnt matter
		angle = 0.0f;
		axis.X = 1.0f;
		axis.Y = 0.0f;
		axis.Z = 0.0f;
    }
	return angle;
}


// *****************************************************************
/// @brief
///     Creates a quaternion that represents a rotation about an arbitrary axis
/// @param axis
///     This axis of rotation
/// @return
///     The angle of rotation, in radians.
// *****************************************************************
Quaternion Quaternion::FromAxisAngle(const Vector3& axis, float angle)
{
	float halfAngle = angle * 0.5f;
	float sinAngle = Math::Sin(halfAngle);

	// W = cos(angle),  X,Y,Z = n * sin(angle)
	return Quaternion(
		Math::Cos(halfAngle),
		axis.X * sinAngle,
		axis.Y * sinAngle,
		axis.Z * sinAngle
		);
}

// *****************************************************************
/// @brief
///     Calculates the transformation matrix that matches the rotation in this quaternion
// *****************************************************************
Matrix3D Quaternion::ToRotationMatrix() const
{
	// Compute a bunch of intermediate float vales
	float x2 = 2.0f * X;
	float y2 = 2.0f * Y;
	float z2 = 2.0f * Z;
	float wx2 = x2 * W;
	float wy2 = y2 * W;
	float wz2 = z2 * W;
	float xx2 = x2 * X;
	float xy2 = y2 * X;
	float xz2 = z2 * X;
	float yy2 = y2 * Y;
	float yz2 = z2 * Y;
	float zz2 = z2 * Z;

	// Setup the the rotation matrix
	return Matrix3D(
		1.0f - (yy2 + zz2),		xy2 - wz2,				xz2 + wy2,				0.0f,
		xy2 + wz2,				1.0f - (xx2 + zz2),		yz2 - wx2,				0.0f,
		xz2 - wy2,				yz2 + wx2,				1.0f - (xx2 + yy2),		0.0f,
		0.0f,					0.0f,					0.0f,					1.0f
		);
}

// *****************************************************************
/// @brief
///     Creates a quaternion that represents the rotation in a transformation matrix
/// @param matrix
///     A transformation matrix.
// *****************************************************************
Quaternion Quaternion::FromRotationMatrix(const Matrix3D& matrix)
{
	float trace = matrix.M11 + matrix.M22 + matrix.M33;
	Quaternion quaternion;

    if (trace > 0.0f)
    {
		float root = Math::Sqrt(trace + 1.0f);
        quaternion.W = root * 0.5f;
        root = 0.5f / root;
        quaternion.X = (matrix.M23 - matrix.M32) * root;
        quaternion.Y = (matrix.M31 - matrix.M13) * root;
        quaternion.Z = (matrix.M12 - matrix.M21) * root;
        return quaternion;
    }
    if ((matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33))
    {
		float root = Math::Sqrt(1.0f + matrix.M11 - matrix.M22 - matrix.M33);
        float root2 = 0.5f / root;
        quaternion.X = 0.5f * root;
        quaternion.Y = (matrix.M12 + matrix.M21) * root2;
        quaternion.Z = (matrix.M13 + matrix.M31) * root2;
        quaternion.W = (matrix.M23 - matrix.M32) * root2;
        return quaternion;
    }
    if (matrix.M22 > matrix.M33)
    {
        float root = Math::Sqrt(1.0f + matrix.M22 - matrix.M11 - matrix.M33);
        float root2 = 0.5f / root;
        quaternion.X = (matrix.M21 + matrix.M12) * root2;
        quaternion.Y = 0.5f * root;
        quaternion.Z = (matrix.M32 + matrix.M23) * root2;
        quaternion.W = (matrix.M31 - matrix.M13) * root2;
        return quaternion;
    }
    float root = Math::Sqrt(1.0f + matrix.M33 - matrix.M11 - matrix.M22);
    float root2 = 0.5f / root;
    quaternion.X = (matrix.M31 + matrix.M13) * root2;
    quaternion.Y = (matrix.M32 + matrix.M23) * root2;
    quaternion.Z = 0.5f * root;
    quaternion.W = (matrix.M12 - matrix.M21) * root2;
    return quaternion;

}

// *****************************************************************
/// @brief
///     Creates a quaternion that represents rotations about the 3 main axes
/// @param pitch
///     Angle of rotation about the X axis
/// @param yaw
///     Angle of rotation about the Y axis
/// @param roll
///     Angle of rotation about the Z axis
// *****************************************************************
Quaternion Quaternion::FromPitchYawRoll(float pitch, float yaw, float roll)
{
	float halfPitch = pitch * 0.5f;
	float halfYaw = yaw * 0.5f;
	float halfRoll = roll * 0.5f;
	float sinPitch = Math::Sin(halfPitch);
	float cosPitch = Math::Cos(halfPitch);
	float sinYaw = Math::Sin(halfYaw);
	float cosYaw = Math::Cos(halfYaw);
	float sinRoll = Math::Sin(halfRoll);
	float cosRoll = Math::Cos(halfRoll);

	return Quaternion(
		cosYaw * cosPitch * cosRoll - sinYaw * sinPitch * sinRoll,
		cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll,
		sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll,
		cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll
		);
}

// *****************************************************************
/// @brief
///     Creates a quaternion that rotates from one vector to another
/// @param start
///     The initial vector direction
/// @param end
///     The end vector direction that the quaternion will rotate to
// *****************************************************************
Quaternion Quaternion::FromAlignment(const Vector3& start, const Vector3& end)
{
	Vector3 bisector = start + end;
	bisector.Normalize();

	float cosHalfAngle = start.Dot(bisector);
	Vector3 cross;

	Quaternion quaternion;
	quaternion.W = cosHalfAngle;

	if(cosHalfAngle != 0.0f)
	{
		cross = start.Cross(bisector);
		quaternion.X = cross.X;
		quaternion.Y = cross.Y;
		quaternion.Z = cross.Z;
	}
	else
	{
		float inverseLength;
		if(Math::Abs(start.X) >= Math::Abs(start.Y))
		{
			inverseLength = 1.0f / Math::Sqrt(start.Y * start.Y + start.X * start.X);
			quaternion.X = -start.Z * inverseLength;
			quaternion.Y = 0.0f;
			quaternion.Z = start.X * inverseLength;
		}
        else
        {
			inverseLength = 1.0f / Math::Sqrt(start.Y * start.Y + start.Z * start.Z);
			quaternion.X = 0.0f;
			quaternion.Y = start.Z * inverseLength;
			quaternion.Y = -start.Y * inverseLength;
        }
    }

	return quaternion;
}

// *****************************************************************
/// @brief
///     Interpolates between two quaternions using spherical linear interpolation
/// @param t
///     t-Value of the interpolation
/// @param start
///     The starting quaternion, represented at t=0
/// @param end
///     The end quaternion, represented at t=1
// *****************************************************************
Quaternion Quaternion::Slerp(float t, const Quaternion& start, const Quaternion& end)
{
	float cos = start.Dot(end);
	float angle = Math::ACos(cos);

	if(Math::Abs(angle) >= Math::ZERO_TOLERANCE)
	{
		float sin = Math::Sin(angle);
		float invSin = 1.0f / sin;
		float tAngle = t * angle;
		float a = Math::Sin(angle - tAngle) * invSin;
		float b = Math::Sin(tAngle) * invSin;

		return Quaternion(
			a * start.W + b * end.W,
			a * start.X + b * end.X,
			a * start.Y + b * end.Y,
			a * start.Z + b * end.Z
			);
	}
    
    return start;
}

// *****************************************************************
/// @brief
///     Interpolates between two quaternions using spherical linear interpolation, and adds extra spins.
/// @param t
///     t-Value of the interpolation
/// @param start
///     The starting quaternion, represented at t=0
/// @param end
///     The end quaternion, represented at t=1
/// @param extraSpins
///     Whole number of extra spins to use in the interpolation.
// *****************************************************************
Quaternion Quaternion::SlerpExtraSpins(float t, const Quaternion& start, const Quaternion& end, int extraSpins)
{
	float cos = start.Dot(end);
	float angle = Math::ACos(cos);

	if(Math::Abs(angle) >= Math::ZERO_TOLERANCE)
	{
		float sin = Math::Sin(angle);
		float phase = Math::PI * extraSpins * t;
		float invSin = 1.0f / sin;
		float a = Math::Sin((1.0f - t) * angle - phase) * invSin;
		float b = Math::Sin(t * angle + phase) * invSin;

		return Quaternion(
			a * start.W + b * end.W,
			a * start.X + b * end.X,
			a * start.Y + b * end.Y,
			a * start.Z + b * end.Z
			);
	}
    
    return start;
}