

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

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Color::Color()
{
	R = G = B = 0;
	A = 255;
}

// ***********************************************************************
Color::Color(Byte r, Byte g, Byte b)
{
	R = r;
	G = g;
	B = b;
	A = 255;
}

// ***********************************************************************
Color::Color(Byte r, Byte g, Byte b, Byte a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

// ***********************************************************************
Color::Color(const Color& input)
{
	R = input.R;
	G = input.G;
	B = input.B;
	A = input.A;
}

// ===================================================================================
// String conversion
// ===================================================================================

// ***********************************************************************
Color Color::Parse(const char *str)
{
	int a=255,r=0,g=0,b=0;

	// Get the length of the string
	int len = strlen(str);
	if(len == 6)
	{
		// String Format:  "RRGGBB" in hex  (alpha = 255)
		sscanf(str, "%2X%2X%2X", &r, &g, &b);
	}
	else if(len == 8)
	{
		// String Format:  "RRGGBBAA" in hex
		sscanf(str, "%2X%2X%2X%2X", &r, &g, &b, &a);
	}

	return Color(r,g,b,a);
}

// ***********************************************************************
string Color::ToString()
{
	// Build a string of the format "RRGGBBAA" in hex
	char temp[9];
	sprintf(temp, "%2X%2X%2X%2X", (int)R, (int)G, (int)B,  (int)A);
	return string(temp);
}

// ===================================================================================
// Stream Utilities
// ===================================================================================

// ***********************************************************************
Color Color::ReadFromStream(Stream* stream)
{
	Color result;
	stream->Read(&result, 4);
	return result;
}

// ***********************************************************************
void Color::WriteToStream(Stream* stream)
{
	stream->Write(this, 4);
}


// ===================================================================================
// Normalization
// ===================================================================================


// ***********************************************************************
void Color::Normalize(float* output, bool includeAlpha)
{
	output[0] = R * Color::NORMALIZATION_SCALAR;
	output[1] = G * Color::NORMALIZATION_SCALAR;
	output[2] = B * Color::NORMALIZATION_SCALAR;
	if(includeAlpha)
		output[3] = A * Color::NORMALIZATION_SCALAR;
}