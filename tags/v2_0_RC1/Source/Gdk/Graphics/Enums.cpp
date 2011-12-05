/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

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
GLuint PixelFormat::GetGLTextureFormat(PixelFormat::Enum pixelFormat)
{
    switch(pixelFormat)
	{
		case PixelFormat::RGB_565:			return GL_RGB;
		case PixelFormat::RGB_888:			return GL_RGB;
		case PixelFormat::RGBA_5551:		return GL_RGBA;
		case PixelFormat::RGBA_4444:		return GL_RGBA;
		case PixelFormat::RGBA_8888:		return GL_RGBA;
		case PixelFormat::LUMINANCE_ALPHA:	return GL_LUMINANCE_ALPHA;
		case PixelFormat::LUMINANCE:		return GL_LUMINANCE;
		case PixelFormat::ALPHA:			return GL_ALPHA;
	}
    return GL_RGB;
}

// ***********************************************************************
GLuint PixelFormat::GetGLPixelType(PixelFormat::Enum pixelFormat)
{
    switch(pixelFormat)
	{
		case PixelFormat::RGB_565:			return GL_UNSIGNED_SHORT_5_6_5;
		case PixelFormat::RGB_888:			return GL_UNSIGNED_BYTE;
		case PixelFormat::RGBA_5551:		return GL_UNSIGNED_SHORT_5_5_5_1;
		case PixelFormat::RGBA_4444:		return GL_UNSIGNED_SHORT_4_4_4_4;
		case PixelFormat::RGBA_8888:		return GL_UNSIGNED_BYTE;
		case PixelFormat::LUMINANCE_ALPHA:	return GL_UNSIGNED_BYTE;
		case PixelFormat::LUMINANCE:		return GL_UNSIGNED_BYTE;
		case PixelFormat::ALPHA:			return GL_UNSIGNED_BYTE;
	}
    return GL_UNSIGNED_BYTE;
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

// ***********************************************************************
GLenum TextureWrapMode::GetGLenum(TextureWrapMode::Enum wrapMode)
{
    if(wrapMode == TextureWrapMode::Clamp)
        return GL_CLAMP_TO_EDGE;
    if(wrapMode == TextureWrapMode::Mirror)
        return GL_MIRRORED_REPEAT;
    return GL_REPEAT;
}


