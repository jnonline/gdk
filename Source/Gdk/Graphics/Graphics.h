/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Enums.h"
#include "Shader/Shader.h"
#include "Shader/SharedUniformValueSet.h"

namespace Gdk
{
	/// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Provides access to the GDK Graphics system.
    ///	@remarks 
    ///		This static-only class contains properties, methods, and events for 
    ///     controlling the GDK Graphics system. 
    // =================================================================================
    class Graphics
	{

	public:

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Screen Size Information
        /// @{

		static int GetScreenWidth();
		static int GetScreenHeight();
		static Vector2 GetScreenBounds();
        
        /// @}
        // ---------------------------------
        /// @name OS Framebuffer Information
        /// @{
        
        static GLuint GetOSFrameBufferId();
        static GLuint GetOSColorRenderBufferId();
        static GLuint GetOSDepthRenderBufferId();
        static GLuint GetOSStencilRenderBufferId();
		
        /// @}
        // ---------------------------------
        /// @name OpenGL Information
        /// @{
        
		static const string& GetGLVendor();
		static const string& GetGLRenderer();
		static const string& GetGLVersion();
		static int GetNumSupportedTextureUnits();
		static ShaderProfile::Enum GetShaderProfile();
        
        /// @}
        // ---------------------------------
        /// @name Pixel Coordinate Methods
        /// @{
        
        static void SetProjectionToScreenOrtho();
        static void SetProjectionToUnitOrtho();
        static void SetProjectionToSymmetricOrtho();
        
		static Vector2 ScreenToUnit(Vector2 screenCoordinate);			// Converts a screen coordinate to [0 to 1] unit space
		static Vector2 ScreenToSymmetric(Vector2 screenCoordinate);		// Converts a screen coordinate to [-1 to 1] symmetric space
		static Vector2 UnitToScreen(Vector2 unitCoordinate);			// Converts a unit coordinate [0 to 1] to screen space
		static Vector2 SymmetricToScreen(Vector2 symmetricCoordinate);	// Converts a symmetric coordinate [-1 to 1] to screen space
        
        /// @}
        // ---------------------------------
        /// @name Basic Methods
        /// @{
        
        static void Clear(
            bool clearColor = true, bool clearDepth = true, bool clearStencil = true,
            const Color& color = Color::TRANSPARENT_BLACK, float depth = 1.0f, int stencil = 0
            );
		static void SetColorWriteMask(bool red, bool green, bool blue, bool alpha, bool forceChange = false);
		
        /// @}
        // ---------------------------------
        /// @name Scissoring Control Methods
        /// @{
        
		static void EnableScissorTest(bool enabled, bool forceChange = false);
		static void SetScissorRect(GLint x, GLint y, GLsizei width, GLsizei height, bool forceChange = false);

        /// @}
        // ---------------------------------
        /// @name Culling Renderstate Methods
        /// @{

		static void SetCullingMode(CullingMode::Enum cullingMode, bool forceChange = false);
		
        /// @}
        // ---------------------------------
        /// @name Depth Renderstate Methods
        /// @{
        
		static void EnableDepthTest(bool enabled, bool forceChange = false);
		static void EnableDepthWrite(bool enabled, bool forceChange = false);
		static void SetDepthTestFunction(CompareFunction::Enum depthFunction, bool forceChange = false);
		static void EnableDepthBias(bool enabled, bool forceChange = false);
		static void SetDepthBias(float factor, float units, bool forceChange = false);

		/// @}
        // ---------------------------------
        /// @name Blending Renderstate Methods
        /// @{
        
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

        /// @}
        // ---------------------------------
        /// @name Texture Unit Control Methods
        /// @{
                
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

        /// @}
        // ---------------------------------
        /// @name Shader Unit Control Methods
        /// @{
        
		static void SetActiveShaderProgram(GLuint shaderProgramId, bool forceChange = false);

		/// @}
        // ---------------------------------
        /// @name Vertex & Index Buffer Control Methods
        /// @{
        
        /// The max number of vertex attributes supported.
		static const int MAX_VERTEX_ATTRIBUTES = 8;
        
		static void EnableVertexAttribArray(int index, bool enabled, bool forceChange = false);
        static void EnableVertexAttribArrays(int numToEnable);
		static void BindVertexBuffer(GLuint glVertexBufferId, bool forceChange = false);
		static void BindIndexBuffer(GLuint glIndexBufferId, bool forceChange = false);

        /// @}
        // ---------------------------------
        /// @name Global Uniforms
        /// @{

        /// @brief
        ///     Global uniforms for sharing and binding in multiple shaders
        /// @remarks
        ///     This Uniform Set contains all the global uniforms for use in any shader. 
        ///     Common shader uniforms such as WVP matrices, lights, materials, etc
        ///     should all be in this global set & shaders themselves should bind to these values.
		static SharedUniformValueSet GlobalUniformsSet;

		/// Provides direct access to various global uniforms
		static struct _GlobalUniforms
		{
			/// W/V/P matrices
			UniformValue* World;
			UniformValue* View;
			UniformValue* Projection;

			UniformValue* WorldView;
			UniformValue* WorldViewProjection;

			/// Camera
			UniformValue* CameraPosition;
			UniformValue* WorldUp;

			/// Material
			struct _Material
			{
				UniformValue* Emissive;
				UniformValue* Ambient;
				UniformValue* Diffuse;
				UniformValue* Specular;
				UniformValue* Shininess;
				UniformValue* Alpha;

			} Material;

			/// Lighting
			UniformValue* AmbientLight;
			UniformValue* NumActiveLights;
			UniformValue* LightPositionsAndFalloffs;
			UniformValue* LightColors;

		} GlobalUniforms;
		
        /// @}
        
    public:

        // INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
        
        // Only the platform application should call these, they are not safe for the game code to call...
        static void Platform_SetOSFrameBuffers(GLuint frameBufferId, GLuint colorRenderBufferId, GLuint depthRenderBufferId, GLuint stencilRenderBufferId);
        
        /// @}
        
        /// @endcond
        
	private:
		
		// Internal Properties
		// =====================================================
        
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
		static ShaderProfile::Enum shaderProfile;

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
        
        // Internal Methods
		// =====================================================
        
		// Init / Update
		static void Init();
		static void InitAssetDependencies();
		static void Update(float elapsedSeconds);
		static void Shutdown();
        
        friend class Application;
	};
    
    /// @}
}

