
#include "BasePCH.h"
#include "Enums.h"

using namespace Gdk;

// ***********************************************************************
short PixelFormat::GetBytesPerPixel(PixelFormat::Enum pixelFormat)
{
	switch(pixelFormat)
	{
		case PixelFormat::LUMINANCE:
		case PixelFormat::ALPHA:
			return 1;

		case PixelFormat::RGB_565:	
		case PixelFormat::RGBA_5551:
		case PixelFormat::RGBA_4444:
		case PixelFormat::LUMINANCE_ALPHA:
			return 2;

		case PixelFormat::RGB_888:
			return 3;

		case PixelFormat::RGBA_8888:
			return 4;
	}
	return 0; // Should never get here....
}

// ***********************************************************************
void BlendMode::GetBlendSettings(
	BlendMode::Enum blendMode,
	BlendFactor::Enum& sourceRGBFactor, BlendFactor::Enum& destRGBFactor, BlendEquation::Enum& rgbBlendEquation, 
	BlendFactor::Enum& sourceAlphaFactor, BlendFactor::Enum& destAlphaFactor, BlendEquation::Enum& alphaBlendEquation
	)
{
	// Return the proper blend settings for the given blend mode
	switch(blendMode)
	{
		case BlendMode::Normal:				
			sourceRGBFactor = sourceAlphaFactor		= BlendFactor::SrcAlpha;
			destRGBFactor = destAlphaFactor			= BlendFactor::OneMinusSrcAlpha;
			rgbBlendEquation = alphaBlendEquation	= BlendEquation::Add;
			break;

		case BlendMode::Add:				
			sourceRGBFactor = sourceAlphaFactor		= BlendFactor::One;
			destRGBFactor = destAlphaFactor			= BlendFactor::One;
			rgbBlendEquation = alphaBlendEquation	= BlendEquation::Add;
			break;

		case BlendMode::Multiply:				
			sourceRGBFactor = sourceAlphaFactor		= BlendFactor::Zero;
			destRGBFactor = destAlphaFactor			= BlendFactor::SrcColor;
			rgbBlendEquation = alphaBlendEquation	= BlendEquation::Add;
			break;

		case BlendMode::MultiplyAdd:				
			sourceRGBFactor = sourceAlphaFactor		= BlendFactor::DestColor;
			destRGBFactor = destAlphaFactor			= BlendFactor::One;
			rgbBlendEquation = alphaBlendEquation	= BlendEquation::Add;
			break;

		case BlendMode::Subtract:				
			sourceRGBFactor = sourceAlphaFactor		= BlendFactor::One;
			destRGBFactor = destAlphaFactor			= BlendFactor::One;
			rgbBlendEquation = alphaBlendEquation	= BlendEquation::ReverseSubtract;
			break;

		case BlendMode::MultiplySubtract:				
			sourceRGBFactor = sourceAlphaFactor		= BlendFactor::DestColor;
			destRGBFactor = destAlphaFactor			= BlendFactor::One;
			rgbBlendEquation = alphaBlendEquation	= BlendEquation::ReverseSubtract;
			break;
	}
}

// ***********************************************************************
const char* BlendMode::ToString(BlendMode::Enum value)
{
	static const char* normal = "Normal";
	static const char* add = "Add";
	static const char* multiply = "Multiply";
	static const char* multiplyAdd = "MultiplyAdd";
	static const char* subtract = "Subtract";
	static const char* multiplySubtract = "MultiplySubtract";

	switch(value)
	{
		case BlendMode::Normal:				return normal;
		case BlendMode::Add:				return add;
		case BlendMode::Multiply:			return multiply;
		case BlendMode::MultiplyAdd:		return multiplyAdd;
		case BlendMode::Subtract:			return subtract;
		case BlendMode::MultiplySubtract:	return multiplySubtract;
	};

	return StringUtilities::Unknown;
}