/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "Enums.h"
#include "Shader/Shader.h"
#include "Shader/SharedUniformValueSet.h"

namespace Gdk
{
	// =============================================================

	class Graphics
	{
	private:
		Graphics();
	    
		// Internal: Application Interface Methods
		// -----------------------------------

		friend class Application;

		// Init / Update
		static void Init();
		static void InitAssetDependencies();
		static void Update(float elapsedSeconds);
		static void Shutdown();


	public:

		// Public: Interfaces
		// -----------------------------------

		// Screen Size & Bounds
		static int GetScreenWidth();
		static int GetScreenHeight();
		static Vector2 GetScreenBounds();

		// Pixel coordinate conversions
		static Vector2 ScreenToUnit(Vector2 screenCoordinate);			// Converts a screen coordinate to [0 to 1] unit space
		static Vector2 ScreenToSymmetric(Vector2 screenCoordinate);		// Converts a screen coordinate to [-1 to 1] symmetric space
		static Vector2 UnitToScreen(Vector2 unitCoordinate);			// Converts a unit coordinate [0 to 1] to screen space
		static Vector2 SymmetricToScreen(Vector2 symmetricCoordinate);	// Converts a symmetric coordinate [-1 to 1] to screen space

		// System Properties
        static GLuint GetOSFrameBufferId()          { return osFrameBufferId; }
        static GLuint GetOSColorRenderBufferId()    { return osColorRenderBufferId; }
        static GLuint GetOSDepthRenderBufferId()    { return osDepthRenderBufferId; }
        static GLuint GetOSStencilRenderBufferId()    { return osStencilRenderBufferId; }
		
		// OpenGL Properties
		static const string& GetGLVendor()			{ return vendorString; }
		static const string& GetGLRenderer()		{ return rendererString; }
		static const string& GetGLVersion()			{ return versionString; }
		static int GetNumSupportedTextureUnits()    { return supportedTextureUnits; }

		// Basics
		static void SetColorWriteMask(bool red, bool green, bool blue, bool alpha, bool forceChange = false);
		static void Clear(ClearBuffers::Enum buffers = ClearBuffers::Color, const Color& color = Color::TRANSPARENT_BLACK, float depth = 1.0f, int stencil = 0);

		// Culling
		static void SetCullingMode(CullingMode::Enum cullingMode, bool forceChange = false);
		
		// Depth
		static void EnableDepthTest(bool enabled, bool forceChange = false);
		static void EnableDepthWrite(bool enabled, bool forceChange = false);
		static void SetDepthTestFunction(CompareFunction::Enum depthFunction, bool forceChange = false);
		static void EnableDepthBias(bool enabled, bool forceChange = false);
		static void SetDepthBias(float factor, float units, bool forceChange = false);

		// Blending
		static void EnableBlending(bool enabled, bool forceChange = false);
		static void SetBlendColorConstant(const Color& color, bool forceChange = false);
		static void SetBlendMode(BlendMode::Enum blendMode, bool forceChange = false);
		static void SetBlendMode(
			BlendFactor::Enum sourceFactor, BlendFactor::Enum destFactor, BlendEquation::Enum blendEquation,
			bool forceChange = false
			);
		static void SetBlendMode(
			BlendFactor::Enum sourceRGBFactor, BlendFactor::Enum destRGBFactor, BlendEquation::Enum rgbBlendEquation,
			BlendFactor::Enum sourceAlphaFactor, BlendFactor::Enum destAlphaFactor, BlendEquation::Enum alphaBlendEquation,
			bool forceChange = false
			);

		// Texture
		static void SetActiveTextureUnit(
			TextureUnit::Enum textureUnit = TextureUnit::Unit_0, 
			bool forceChange = false
			);
		static void BindTexture(
			GLuint glTextureId, 
			TextureUnit::Enum textureUnit = TextureUnit::Unit_0, 
			TextureTarget::Enum textureTarget = TextureTarget::Texture2D,
			bool forceChange = false
			);

		// Scissoring
		static void EnableScissorTest(bool enabled, bool forceChange = false);
		static void SetScissorRect(GLint x, GLint y, GLsizei width, GLsizei height, bool forceChange = false);

		// Shader
		static void SetActiveShaderProgram(GLuint shaderProgramId, bool forceChange = false);

		// Vertex Attributes
		static const int MAX_VERTEX_ATTRIBUTES = 8;
		static void EnableVertexAttribArray(int index, bool enabled, bool forceChange = false);
		static void BindVertexBuffer(GLuint glVertexBufferId, bool forceChange = false);
		static void BindIndexBuffer(GLuint glIndexBufferId, bool forceChange = false);

		// Public: Shared\Global Uniform Values
		// -----------------------------------

		// A set of shared shader uniform values, managed at the global Graphics level
		static SharedUniformValueSet GlobalUniformsSet;

		// Quick accessor for various shared parameters
		static struct _GlobalUniforms
		{
			// W/V/P matrices
			UniformValue* World;
			UniformValue* View;
			UniformValue* Projection;

			UniformValue* WorldView;
			UniformValue* WorldViewProjection;

			// Camera
			UniformValue* CameraPosition;
			UniformValue* WorldUp;

			// Material
			struct _Material
			{
				UniformValue* Emissive;
				UniformValue* Ambient;
				UniformValue* Diffuse;
				UniformValue* Specular;
				UniformValue* Shininess;
				UniformValue* Alpha;

			} Material;

			// Lighting
			UniformValue* AmbientLight;
			UniformValue* NumActiveLights;
			UniformValue* LightPositionsAndFalloffs;
			UniformValue* LightColors;

		} GlobalUniforms;
		

        // Public: Platform Host Interfaces
		// -----------------------------------
        
        // Only the platform application should call these, they are not safe for the game code to call...
        static void Platform_SetOSFrameBuffers(GLuint frameBufferId, GLuint colorRenderBufferId, GLuint depthRenderBufferId, GLuint stencilRenderBufferId);
        
	private:
		
		// Internal Properties
		// -----------------------------------

        // OS Frame & Render buffers
        static GLuint osFrameBufferId;
        static GLuint osColorRenderBufferId;
        static GLuint osDepthRenderBufferId;
        static GLuint osStencilRenderBufferId;
        
		// System/OpenGL properties
		static string vendorString;
		static string rendererString;
		static string versionString;
		static GLint supportedTextureUnits;

		// Basics
		static bool currentRedWriteMask;
		static bool currentGreenWriteMask;
		static bool currentBlueWriteMask;
		static bool currentAlphaWriteMask;

		// Culling
		static CullingMode::Enum currentCullingMode;

		// Depth
		static bool currentDepthTestEnabled;
		static bool currentDepthWriteEnabled;
		static CompareFunction::Enum currentDepthCompareFunction;
		static bool currentDepthBiasEnabled;
		static float currentDepthBiasFactor;
		static float currentDepthBiasUnits;

		// Blending
		static bool currentBlendingEnabled;
		static Color currentBlendColorConstant;
		static BlendFactor::Enum currentSourceRGBFactor;
		static BlendFactor::Enum currentDestRGBFactor;
		static BlendEquation::Enum currentRGBBlendEquation;
		static BlendFactor::Enum currentSourceAlphaFactor;
		static BlendFactor::Enum currentDestAlphaFactor;
		static BlendEquation::Enum currentAlphaBlendEquation;

		// Texturing
		static GLuint currentTextureIds[TextureUnit::MaxUnits];
		static TextureUnit::Enum currentActiveTextureUnit;

		// Scissoring
		static bool currentScissorTestEnabled;
		static GLint currentScissorX;
		static GLint currentScissorY;
		static GLsizei currentScissorWidth;
		static GLsizei currentScissorHeight;

		// Shader
		static GLuint currentShaderProgramId;

		// Vertex Attributes
		static bool currentEnabledVertexAttributes[MAX_VERTEX_ATTRIBUTES];
		static GLuint currentVertexBufferId;
		static GLuint currentIndexBufferId;
	};
}

