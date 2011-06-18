
#include "BasePCH.h"
#include "Vector3.h"

using namespace Gdk;

// instantiations of static constants
const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_X(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Y(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Z(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::ONE(1.0f, 1.0f, 1.0f);


// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Vector3::Vector3()
{
}

// ***********************************************************************
Vector3::Vector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

// ***********************************************************************
Vector3::Vector3(const Vector3& input)
{
	X = input.X;
	Y = input.Y;
	Z = input.Z;
}


// ===================================================================================
// String conversion
// ===================================================================================

// ***********************************************************************
Vector3 Vector3::Parse(const char *str)
{
	// String format: "1.1,2.2,3.3"
	float x,y,z;

	// Read the 2 floats from the string
	sscanf(str, "%f,%f,%f", &x, &y, &z);

	return Vector3(x,y,z);
}

// ***********************************************************************
string Vector3::ToString()
{
	// Build a string of the format "1.1,2.2,3.3"
	char temp[48];
	sprintf(temp, "%f,%f,%f", X, Y, Z);
	return string(temp);
}

// ===================================================================================
// Interpolation Methods
// ===================================================================================

// ***********************************************************************
Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t)
{
	// Calculate the new point
	return Vector3(
		start.X + t * (end.X - start.X),  
		start.Y + t * (end.Y - start.Y),
		start.Z + t * (end.Z - start.Z)
		); 
}

// ***********************************************************************
Vector3 Vector3::Hermite(const Vector3& point1, const Vector3& tangent1, const Vector3& point2, const Vector3& tangent2, float t)
{
	// Calculate the Hermite Cooefficients
	float t2 = t * t;
	float t3 = t2 * t;
	float a = (2.0f * t3) - (3.0f * t2) + 1.0f;
	float b = (-2.0f * t3) + (3.0f * t2);
	float c = t3 - (2.0f * t2) + t;
	float d = t3 - t2;

	// Calculate the new point
	return Vector3(
		point1.X * a + point2.X * b + tangent1.X * c + tangent2.X * d,
		point1.Y * a + point2.Y * b + tangent1.Y * c + tangent2.Y * d,
		point1.Z * a + point2.Z * b + tangent1.Z * c + tangent2.Z * d
		); 
}

// ***********************************************************************
Vector3 Vector3::Bezier(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& point4, float t)
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
	return Vector3(
		point1.X * a + point2.X * b + point3.X * c + point4.X * d,
		point1.Y * a + point2.Y * b + point3.Y * c + point4.Y * d,
		point1.Z * a + point2.Z * b + point3.Z * c + point4.Z * d
		); 
}

// ***********************************************************************
Vector3 Vector3::CatmullRom(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& point4, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

	return Vector3(
		0.5f * ((((2.0f * point2.X) + ((-point1.X + point3.X) * t)) + (((((2.0f * point1.X) - (5.0f * point2.X)) + (4.0f * point3.X)) - point4.X) * t2)) + ((((-point1.X + (3.0f * point2.X)) - (3.0f * point3.X)) + point4.X) * t3)),
		0.5f * ((((2.0f * point2.Y) + ((-point1.Y + point3.Y) * t)) + (((((2.0f * point1.Y) - (5.0f * point2.Y)) + (4.0f * point3.Y)) - point4.Y) * t2)) + ((((-point1.Y + (3.0f * point2.Y)) - (3.0f * point3.Y)) + point4.Y) * t3)),
		0.5f * ((((2.0f * point2.Z) + ((-point1.Z + point3.Z) * t)) + (((((2.0f * point1.Z) - (5.0f * point2.Z)) + (4.0f * point3.Z)) - point4.Z) * t2)) + ((((-point1.Z + (3.0f * point2.Z)) - (3.0f * point3.Z)) + point4.Z) * t3))
		);
}

// ***********************************************************************
Vector3 Vector3::Barycentric(const Vector3& point1, const Vector3& point2, const Vector3& point3, float t1, float t2)
{
    return Vector3(
		(point1.X + (t1 * (point2.X - point1.X))) + (t2 * (point3.X - point1.X)),
		(point1.Y + (t1 * (point2.Y - point1.Y))) + (t2 * (point3.Y - point1.Y)),
		(point1.Z + (t1 * (point2.Y - point1.Z))) + (t2 * (point3.Y - point1.Z))
		);
}



 