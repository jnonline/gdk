/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Vector2.h"

using namespace Gdk;

// instantiations of static constants
const Vector2 Vector2::ZERO(0.0f, 0.0f);
const Vector2 Vector2::UNIT_X(1.0f, 0.0f);
const Vector2 Vector2::UNIT_Y(0.0f, 1.0f);
const Vector2 Vector2::ONE(1.0f, 1.0f);

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Vector2::Vector2()
{
}

// *****************************************************************
/// @brief
///     Constructs a new vector from the given x & y
// *****************************************************************
Vector2::Vector2(float x, float y)
{
	X = x;
	Y = y;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Vector2::Vector2(const Vector2& input)
{
	X = input.X;
	Y = input.Y;
}

// *****************************************************************
/// @brief
///     Parses a Vector2 from a string
/// @param str
///     A string of the format "%f,%f"
// *****************************************************************
Vector2 Vector2::Parse(const char *str)
{
	// String format: "2.31,1.23"
	float x,y;

	// Read the 2 floats from the string
	GDK_SSCANF(str, "%f,%f", &x, &y);

	return Vector2(x,y);
}

// *****************************************************************
/// @brief
///     Creates a string representation of this vector.
/// @remarks
///     The resultant string has the format: "%f,%f"
// *****************************************************************
string Vector2::ToString()
{
	// Build a string of the format "1.23,3.21"
	char temp[32];
	GDK_SPRINTF(temp, 32, "%f,%f", X, Y);
	return string(temp);
}

// *****************************************************************
/// @brief
///     Gets the polar coordiantes equivalent of this vector
/// @param[out] r
///     The radius polar coordinate
/// @param[out] theta
///     The angle polar coordinate
// *****************************************************************
void Vector2::ToPolarCoordinate(float& r, float& theta)
{
    // r = Pythagorean
    r = this->Length();
    
    // theta = (-PI to PI)
    if(X > 0)
    {
        theta = Math::ATan(Y/X);
    }
    else if(X < 0)
    {
        if(Y >= 0)
            theta = Math::ATan(Y/X) + Math::PI;
        else
            theta = Math::ATan(Y/X) - Math::PI;
    }
    else
    {
        if(Y > 0)
            theta = Math::HALF_PI;
        else if(Y < 0)
            theta = -Math::HALF_PI;
        else
            theta = 0;
    }
}

// *****************************************************************
/// @brief
///     Creates a vector from a set of polar coordinates
/// @param r
///     The radius polar coordinate
/// @param theta
///     The angle polar coordinate
// *****************************************************************
Vector2 Vector2::FromPolarCoordinate(float r, float theta)
{
    return Vector2(
        r * Math::Cos(theta), 
        r * Math::Sin(theta)
        );
}

// *****************************************************************
/// @brief
///     Calculates the linear interpolation between two vectors
/// @param start
///     The 1st vector, represented at t=0
/// @param end
///     The 2nd vector, represented at t=1
/// @param t
///     The t-value of the interpolation. (usually between 0 and 1, but can be outside that range to extrapolate instead of interpolate)
// *****************************************************************
Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float t)
{
	// Calculate the new point
	return Vector2(
		start.X + t * (end.X - start.X),  
		start.Y + t * (end.Y - start.Y)
		); 
}

// *****************************************************************
/// @brief
///     Calculates an interpolated point along a Hermite Spline.
/// @remarks
///     Hermite splines are cubic splines defined by two endpoints and two tangent vectors pointing away from the endpoints
/// @param point1
///     The 1st endpoint, represented at t=0
/// @param tangent1
///     The tangent vector from the 1st endpoint.
/// @param point2
///     The 2nd endpoint, represented at t=1
/// @param tangent2
///     The tangent vector from the 2nd endpoint.
/// @param t
///     The t-value of the interpolation. (usually between 0 and 1, but can be outside that range to extrapolate instead of interpolate)
// *****************************************************************
Vector2 Vector2::Hermite(const Vector2& point1, const Vector2& tangent1, const Vector2& point2, const Vector2& tangent2, float t)
{
	// Calculate the Hermite Cooefficients
	float t2 = t * t;
	float t3 = t2 * t;
	float a = (2.0f * t3) - (3.0f * t2) + 1.0f;
	float b = (-2.0f * t3) + (3.0f * t2);
	float c = t3 - (2.0f * t2) + t;
	float d = t3 - t2;

	// Calculate the new point
	return Vector2(
		point1.X * a + point2.X * b + tangent1.X * c + tangent2.X * d,
		point1.Y * a + point2.Y * b + tangent1.Y * c + tangent2.Y * d
		); 
}

// *****************************************************************
/// @brief
///     Calculates an interpolated point along a Bezier Spline.
/// @remarks
///     Bezier splines are cubic splines defined by 4 points:  2 end-points that the spline will 
///     go through and 2 middle control points that the spline will approximate.
/// @param point1
///     The 1st endpoint, represented at t=0
/// @param point2
///     The control point near the 1st endpoint
/// @param point3
///     The control point near the 2nd endpoint
/// @param point4
///     The 2nd endpoint, represented at t=1
/// @param t
///     The t-value of the interpolation. (usually between 0 and 1, but can be outside that range to extrapolate instead of interpolate)
// *****************************************************************
Vector2 Vector2::Bezier(const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector2& point4, float t)
{
	// Bezier splines will go through the endpoints and approximate the 2 middle control points

	// Calculate the Bezier Cooefficients
	float t2 = t * t;
	float t3 = t2 * t;
	float a = -t3 + (3.0f * t2) - (3.0f * t) + 1.0f;
	float b = (3.0f * t3) - (6.0f * t2) + (3.0f * t);
	float c = -(3.0f * t3) + (3.0f * t2);
	float d = t3;

	// Calculate the new point
	return Vector2(
		point1.X * a + point2.X * b + point3.X * c + point4.X * d,
		point1.Y * a + point2.Y * b + point3.Y * c + point4.Y * d
		); 
}

// *****************************************************************
/// @brief
///     Calculates an interpolated point along a Catmull-Rom Spline.
/// @remarks
///     Catmull Rom splines are defined by 4 points: 2 outer control points and 2 inner endpoints
/// @param point1
///     The outer control point near the 1st endpoint
/// @param point2
///     The 1st endpoint, represented at t=0
/// @param point3
///     The 2nd endpoint, represented at t=1
/// @param point4
///     The outer control point near the 2nd endpoint
/// @param t
///     The t-value of the interpolation. (usually between 0 and 1, but can be outside that range to extrapolate instead of interpolate)
// *****************************************************************
Vector2 Vector2::CatmullRom(const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector2& point4, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

	return Vector2(
		0.5f * ((((2.0f * point2.X) + ((-point1.X + point3.X) * t)) + (((((2.0f * point1.X) - (5.0f * point2.X)) + (4.0f * point3.X)) - point4.X) * t2)) + ((((-point1.X + (3.0f * point2.X)) - (3.0f * point3.X)) + point4.X) * t3)),
		0.5f * ((((2.0f * point2.Y) + ((-point1.Y + point3.Y) * t)) + (((((2.0f * point1.Y) - (5.0f * point2.Y)) + (4.0f * point3.Y)) - point4.Y) * t2)) + ((((-point1.Y + (3.0f * point2.Y)) - (3.0f * point3.Y)) + point4.Y) * t3))
		);
}

// *****************************************************************
/// @brief
///     Calculates an interpolated point from a set of Barycentric Coordinates
/// @remarks
///     Barycentric coordinates are defined by two parametric coordinates that interpolate
///     From one vertex of the triangle to the other 2 vertices.
/// @param point1
///     The anchor vertex of the triangle.  Represented by t1=0, t2=0
/// @param point2
///     A 2nd vertex of the triangle.  Represented by t1=1, t2=0
/// @param point3
///     A 2rd vertex of the triangle.  Represented by t1=0, t2=1
/// @param t1
///     A t-value that linear interpolates between point1 and point2
/// @param t2
///     A t-value that linear interpolates between point1 and point3
// *****************************************************************
Vector2 Vector2::Barycentric(const Vector2& point1, const Vector2& point2, const Vector2& point3, float t1, float t2)
{
    return Vector2(
		(point1.X + (t1 * (point2.X - point1.X))) + (t2 * (point3.X - point1.X)),
		(point1.Y + (t1 * (point2.Y - point1.Y))) + (t2 * (point3.Y - point1.Y))
		);
}

// *****************************************************************
/// @brief
///     Reads a Vector2 from a stream.
// *****************************************************************
Vector2 Vector2::ReadFromStream(Stream* stream)
{
	Vector2 result;
	stream->Read(&result, sizeof(float[2]));
	return result;
}

// *****************************************************************
/// @brief
///     Writes this vector to a stream.
// *****************************************************************
void Vector2::WriteToStream(Stream* stream)
{
	stream->Write(this, sizeof(float[2]));
}


 
