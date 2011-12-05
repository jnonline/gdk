/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Shader profiles supported by the GDK shader system
    // =================================================================================
	namespace ShaderProfile
	{
		enum Enum
		{
            /// This mask value will get the "Profile Type" from an enum value
			PROFILE_TYPE_MASK = 0xF,
            
            /// Profile Type:  Any (shader supports all profiles)
			Any = 0x0,
            
            /// Profile Type:  GLES (shader supports OpenGL ES, 2.0 or higher)
			GLES = 0x1,
            
            /// Profile Type:  GLSL (shader supports OpenGL GLSL, 2.0 or higher)
			GLSL = 0x2,

            /// This mask gets the "Profile Version" from an enum value.  A version of 0 means the default lowest supported version (IE, GLSL 2.0)
			PROFILE_VERSION_MASK = 0xF0,
            
            /// Profile Version:   GLSL 3.3 is required
			GLSL_3_3 = 0x10 | GLSL,
            
            /// Profile Version:   GLSL 4.0 is required
			GLSL_4_0 = 0x20 | GLSL
		};
	}

	// =================================================================================
    ///	@brief
    ///		Pixel formats supported by the GDK.  These pixel formats are a common subset 
    ///     that are guaranteed available on all platforms.
    // =================================================================================
	namespace PixelFormat
	{
		enum Enum
		{
            /// 16-Bit:  5-Red, 6-Green, 5-Blue
			RGB_565 = 1,
            
            /// 24-Bit:  8-Red, 8-Green, 8-Blue
			RGB_888,
            
            /// 16-Bit:  5-Red, 5-Green, 5-Blue, 1-Alpha
			RGBA_5551,
            
            /// 16-Bit:  4-Red, 4-Green, 4-Blue, 4-Alpha
			RGBA_4444,
            
            /// 32-Bit:  8-Red, 8-Green, 8-Blue, 8-Alpha
			RGBA_8888,
            
            /// 16-Bit:  8-Luminance, 8-Alpha
			LUMINANCE_ALPHA,
            
            /// 8-Bit:  8-Luminance
			LUMINANCE,
            
            /// 8-Bit:  8-Alpha
			ALPHA,
		};

        // *****************************************************************
        /// @brief
        ///     Gets the number of bytes per pixel for a given pixel format
        // *****************************************************************
		short GetBytesPerPixel(PixelFormat::Enum pixelFormat);
        
        // *****************************************************************
        /// @brief
        ///     Gets the GL texture format enum value for the given pixel format
        // *****************************************************************
        GLuint GetGLTextureFormat(PixelFormat::Enum pixelFormat);
        
        // *****************************************************************
        /// @brief
        ///     Get the GL pixel type enum value for the given pixel format
        // *****************************************************************
		GLuint GetGLPixelType(PixelFormat::Enum pixelFormat);
	}

	// =================================================================================
    ///	@brief
    ///		Enumeration of supported culling modes
    // =================================================================================
	namespace CullingMode
	{
		enum Enum
		{
            /// No backface culling, all traiangles are rendererd
			Disabled = 0,
            
            /// Faces with clockwise vertex ordering are culled
			Clockwise = 1,				
            
            /// Faces with clockwise vertex ordering are culled
            CW = 1,
            
            /// Faces with clockwise vertex ordering are culled
			CounterClockwise = 2,		
            
            /// Faces with clockwise vertex ordering are culled
            CCW = 2,
		};
	}

	// =================================================================================
    ///	@brief
    ///		Enumeration of comparison functions used by the GDK graphics system
    // =================================================================================
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

	// =================================================================================
    ///	@brief
    ///		Enumeration of blend factors by the GDK graphics system
    // =================================================================================
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

	// =================================================================================
    ///	@brief
    ///		Enumeration of blend equations used by the GDK graphics system
    // =================================================================================
	namespace BlendEquation
	{
		enum Enum
		{
			Add						= GL_FUNC_ADD,
			Subtract				= GL_FUNC_SUBTRACT,
			ReverseSubtract			= GL_FUNC_REVERSE_SUBTRACT,
		};
	}

    // =================================================================================
    ///	@brief
    ///		Enumeration of common blending modes used by the GDK graphics system
    /// @remarks
    ///     The blend modes map to common setups of GL blend functions and blend equations
    ///     The comments for each blend mode explain the math of the blending.
    ///   @par
    ///     Sc = Source Color
    ///     Sa = Source Alpha
    ///     Dc = Destination Color
    ///     Da = Destination Alpha
    // =================================================================================
	namespace BlendMode
	{
		enum Enum
		{
            /// Standard blending.  [Sc * Sa + Dc * (1 - Sa)]
			Normal = 0,
            
            /// Additive blending.  [Sc + Dc]
			Add,
            
            /// Multiplicative blending.  [Sc * Dc]
			Multiply,
            
            /// Multiply & Add blending.  [Dc + Sc * Dc]
			MultiplyAdd,
            
            /// Subtractive blending.  [Dc - Sc]
			Subtract,
            
            /// Multiple & Subtract blending.  [Dc - Sc * Dc]
			MultiplySubtract,
		};

        // *****************************************************************
        /// @brief
        ///     Gets the blend factors & blend equations that are used by a given blend mode.
        /// @param blendMode
        ///     The blend mode to get the functions & equations of.
        /// @see Graphics::SetBlendMode()
        // *****************************************************************
		void GetBlendSettings(
			BlendMode::Enum blendMode,
			BlendFactor::Enum& sourceRGBFactor, BlendFactor::Enum& destRGBFactor, BlendEquation::Enum& rgbBlendEquation, 
			BlendFactor::Enum& sourceAlphaFactor, BlendFactor::Enum& destAlphaFactor, BlendEquation::Enum& alphaBlendEquation
			);

		// *****************************************************************
        /// @brief
        ///     Gets a display-friendly string for the given enum value
        // *****************************************************************
		const char* ToString(BlendMode::Enum value);
	}

	// =================================================================================
    ///	@brief
    ///		Enumeration of the GL texture units
    // =================================================================================
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

		// *****************************************************************
        /// @brief
        ///     Gets the the texture unit enum for a given 0-based index
        // *****************************************************************
		inline TextureUnit::Enum FromIndex(int index) { return (TextureUnit::Enum) (index + GL_TEXTURE0); }
		
        // *****************************************************************
        /// @brief
        ///     Gets the 0-based index of a texture unit from the enum value
        // *****************************************************************
		inline int ToIndex(TextureUnit::Enum textureUnit) { return (int)textureUnit - (int)GL_TEXTURE0; }

		/// Maximum number of texture units usable in the GDK
		const int MaxUnits = 16;
	}

	// =================================================================================
    ///	@brief
    ///		Enumeration of texture wrapping modes
    // =================================================================================
	namespace TextureWrapMode
	{
		enum Enum
		{
            /// Repeats the texture at texture coordinate edges
			Wrap = 0,

            /// Mirrors the texture at texture coordinate edges
			Mirror = 1,
            
            /// Clamps the texture coordinates between 0 and 1, disabling wrapping
			Clamp = 2,
		};
        
        GLenum GetGLenum(TextureWrapMode::Enum wrapMode);
	}

	// =================================================================================
    ///	@brief
    ///		Enumeration of texture filtering modes
    // =================================================================================
	namespace TextureFilterMode
	{
		enum Enum
		{
            /// No filtering, use the nearest texel color
			Nearest = 0,
            
            /// Bi-Linear filtering
			Bilinear = 1,
		};
	}

	// =================================================================================
    ///	@brief
    ///		Enumeration of types of textures usable in GL
    // =================================================================================
	namespace TextureTarget
	{
		enum Enum
		{
			Texture2D			= GL_TEXTURE_2D,
			TextureCubeMap		= GL_TEXTURE_CUBE_MAP,
		};
	}

    /// @}
    
} // namespace 
