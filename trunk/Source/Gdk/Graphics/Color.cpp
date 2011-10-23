/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Color.h"

using namespace Gdk;

// instantiations of static constants
const Color Color::TRANSPARENT_BLACK(0,0,0,0);
const Color Color::BLACK(0,0,0,255);
const Color Color::WHITE(255,255,255,255);
const Color Color::RED(255,0,0,255);
const Color Color::GREEN(0,255,0,255);
const Color Color::BLUE(0,0,255,255);
const Color Color::YELLOW(255,255,0,255);
const Color Color::PURPLE(255,0,255,255);
const Color Color::CYAN(0,255,255,255);
const Color Color::GRAY(90,90,90,255);

const float Color::NORMALIZATION_SCALAR = 1.0f / 255.0f;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Color::Color()
{
	R = G = B = 0;
	A = 255;
}

// *****************************************************************
/// @brief
///     Constructs a Color from the given RGB values
// *****************************************************************
Color::Color(UInt8 r, UInt8 g, UInt8 b)
{
	R = r;
	G = g;
	B = b;
	A = 255;
}

// *****************************************************************
/// @brief
///     Constructs a Color from the given RGBA values
// *****************************************************************
Color::Color(UInt8 r, UInt8 g, UInt8 b, UInt8 a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Color::Color(const Color& input)
{
	R = input.R;
	G = input.G;
	B = input.B;
	A = input.A;
}

// *****************************************************************
/// @brief
///     Parses a Color from a string
/// @param str
///     A string of the format "RRGGBB" or "RRGGBBAA" where each
///     component value is in hex.  (Example: "FF7F00FF"
// *****************************************************************
Color Color::Parse(const char *str)
{
	int a=255,r=0,g=0,b=0;

	// Get the length of the string
	int len = strlen(str);
	if(len == 6)
	{
		// String Format:  "RRGGBB" in hex  (alpha = 255)
		GDK_SSCANF(str, "%2X%2X%2X", &r, &g, &b);
	}
	else if(len == 8)
	{
		// String Format:  "RRGGBBAA" in hex
		GDK_SSCANF(str, "%2X%2X%2X%2X", &r, &g, &b, &a);
	}

	return Color(r,g,b,a);
}

// *****************************************************************
/// @brief
///     Creates a string representation of this color.
/// @remarks
///     The resultant string has the format: "RRGGBBAA"
// *****************************************************************
string Color::ToString()
{
	// Build a string of the format "RRGGBBAA" in hex
	char temp[9];
	GDK_SPRINTF(temp, 9, "%2X%2X%2X%2X", (int)R, (int)G, (int)B,  (int)A);
	return string(temp);
}

// *****************************************************************
/// @brief
///     Reads a Color from a stream.
// *****************************************************************
Color Color::ReadFromStream(Stream* stream)
{
	Color result;
	stream->Read(&result, 4);
	return result;
}

// *****************************************************************
/// @brief
///     Writes this color to a stream.
// *****************************************************************
void Color::WriteToStream(Stream* stream)
{
	stream->Write(this, 4);
}


// *****************************************************************
/// @brief
///     Calculates the normalized float values of this color
/// @param output
///     A float[3] or float[4] array that will receive the color values as normalized floats
/// @param includeAlpha
///     If true, the output argument must be a float[4], and that alpha channel will be written last.
///     If false, the output argument only needs to be a float[3]
// *****************************************************************
void Color::Normalize(float* output, bool includeAlpha)
{
	output[0] = R * Color::NORMALIZATION_SCALAR;
	output[1] = G * Color::NORMALIZATION_SCALAR;
	output[2] = B * Color::NORMALIZATION_SCALAR;
	if(includeAlpha)
		output[3] = A * Color::NORMALIZATION_SCALAR;
}