/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	// ============================================================================
	namespace ShaderProfile
	{
		enum Enum
		{
			PROFILE_TYPE_MASK = 0xF,
			Any = 0x0,
			GLES = 0x1,
			GLSL = 0x2,

			PROFILE_VERSION_MASK = 0xF0,
			GLSL_3_3 = 0x10 | GLSL,
			GLSL_4_0 = 0x20 | GLSL
		};
	}

	// ===============================================================
	namespace PixelFormat
	{
		enum Enum
		{
			RGB_565 = 1,
			RGB_888,	
			RGBA_5551,
			RGBA_4444,
			RGBA_8888,
			LUMINANCE_ALPHA,
			LUMINANCE,
			ALPHA,
		};

		short GetBytesPerPixel(PixelFormat::Enum pixelFormat);
	}

	// ===============================================================
	namespace CullingMode
	{
		enum Enum
		{
			Disabled = 0,
			Clockwise = 1,				CW = 1,
			CounterClockwise = 2,		CCW = 2,
		};
	}

	// ===============================================================
	namespace CompareFunction
	{
		enum Enum
		{
			Never			= GL_NEVER,
			Always			= GL_ALWAYS,
			Equal			= GL_EQUAL,
			NotEqual		= GL_NOTEQUAL,
			Less			= GL_LESS,
			LessOrEqual		= GL_LEQUAL,
			Greater			= GL_GREATER,
			GreaterOrEqual	= GL_GEQUAL,
		};
	}

	// ===============================================================
	namespace BlendFactor
	{
		enum Enum
		{
			 Zero					= GL_ZERO, 
			 One					= GL_ONE, 
			 SrcColor				= GL_SRC_COLOR,
			 OneMinusSrcColor		= GL_ONE_MINUS_SRC_COLOR, 
			 DestColor				= GL_DST_COLOR,
			 OneMinusDestColor		= GL_ONE_MINUS_DST_COLOR,
			 SrcAlpha				= GL_SRC_ALPHA,
			 OneMinusSrcAlpha		= GL_ONE_MINUS_SRC_ALPHA, 
			 DestAlpha				= GL_DST_ALPHA, 
			 OneMinusDestAlpha		= GL_ONE_MINUS_DST_ALPHA, 
			 ConstantColor			= GL_CONSTANT_COLOR,
			 OneMinusConstantColor	= GL_ONE_MINUS_CONSTANT_COLOR, 
			 ConstantAlpha			= GL_CONSTANT_ALPHA, 
			 OneMinusConstantAlpha	= GL_ONE_MINUS_CONSTANT_ALPHA, 
			 SrcAlphaSaturate		= GL_SRC_ALPHA_SATURATE,					// Only valid as a Source blend factor
		};
	}

	// ===============================================================
	namespace BlendEquation
	{
		enum Enum
		{
			Add						= GL_FUNC_ADD,
			Subtract				= GL_FUNC_SUBTRACT,
			ReverseSubtract			= GL_FUNC_REVERSE_SUBTRACT,
		};
	}

	// ===============================================================
	namespace BlendMode
	{
		enum Enum
		{
			Normal			= 0x0000,			// D = Sc * Sa + Dc * (1 - Sa)
			Add,								// D = Sc + Dc
			Multiply,							// D = Sc * Dc
			MultiplyAdd,						// D = Dc + Sc * Dc
			Subtract,							// D = Dc - Sc
			MultiplySubtract,					// D = Dc - Sc * Dc
		};

		void GetBlendSettings(
			BlendMode::Enum blendMode,
			BlendFactor::Enum& sourceRGBFactor, BlendFactor::Enum& destRGBFactor, BlendEquation::Enum& rgbBlendEquation, 
			BlendFactor::Enum& sourceAlphaFactor, BlendFactor::Enum& destAlphaFactor, BlendEquation::Enum& alphaBlendEquation
			);

		const char* ToString(BlendMode::Enum value);
	}

	// ===============================================================
	namespace TextureUnit
	{
		enum Enum
		{
			Unit_0 = GL_TEXTURE0,
			Unit_1 = GL_TEXTURE1,
			Unit_2 = GL_TEXTURE2,
			Unit_3 = GL_TEXTURE3,
			Unit_4 = GL_TEXTURE4,
			Unit_5 = GL_TEXTURE5,
			Unit_6 = GL_TEXTURE6,
			Unit_7 = GL_TEXTURE7,
			Unit_8 = GL_TEXTURE8,
			Unit_9 = GL_TEXTURE9,
			Unit_10 = GL_TEXTURE10,
			Unit_11 = GL_TEXTURE11,
			Unit_12 = GL_TEXTURE12,
			Unit_13 = GL_TEXTURE13,
			Unit_14 = GL_TEXTURE14,
			Unit_15 = GL_TEXTURE15,
		};

		// Converts a 0-based index to the appropriate TextureUnit identifier
		inline TextureUnit::Enum FromIndex(int index) { return (TextureUnit::Enum) (index + GL_TEXTURE0); }
		inline int ToIndex(TextureUnit::Enum textureUnit) { return (int)textureUnit - (int)GL_TEXTURE0; }

		const int MaxUnits = 16;
	}

	// ===============================================================
	namespace TextureWrapMode
	{
		enum Enum
		{
			Clamp				= GL_CLAMP_TO_EDGE,
			MirroredRepeat		= GL_MIRRORED_REPEAT,
			Repeat				= GL_REPEAT,
		};
	}

	// ===============================================================
	namespace TextureFilterMode
	{
		enum Enum
		{
			Nearest,
			Linear
		};
	}

	// ===============================================================
	namespace TextureTarget
	{
		enum Enum
		{
			Texture2D			= GL_TEXTURE_2D,
			TextureCubeMap		= GL_TEXTURE_CUBE_MAP,
		};
	}

	// ===============================================================
	namespace ClearBuffers
	{
		enum Enum
		{
			Color			= GL_COLOR_BUFFER_BIT,
			Depth			= GL_DEPTH_BUFFER_BIT,
			Stencil			= GL_STENCIL_BUFFER_BIT,

			ColorDepth		= Color + Depth,
			ColorStencil	= Color + Stencil,
			DepthStencil	= Depth + Stencil,
			All				= Color + Depth + Stencil,
		};
	}

} // namespace 
