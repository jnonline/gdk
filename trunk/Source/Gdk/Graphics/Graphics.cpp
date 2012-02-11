/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Graphics.h"

using namespace Gdk;

// Static globals
// ---------------------------------------------

// OS Frame & Render Buffers
GLuint Graphics::osFrameBufferId;
GLuint Graphics::osColorRenderBufferId;
GLuint Graphics::osDepthRenderBufferId;
GLuint Graphics::osStencilRenderBufferId;

// System Properties
string Graphics::vendorString;
string Graphics::rendererString;
string Graphics::versionString;
GLint Graphics::supportedTextureUnits;
ShaderProfile::Enum Graphics::shaderProfile;

// Basics
bool Graphics::currentRedWriteMask;
bool Graphics::currentGreenWriteMask;
bool Graphics::currentBlueWriteMask;
bool Graphics::currentAlphaWriteMask;

// Culling
CullingMode::Enum Graphics::currentCullingMode;

// Depth
bool Graphics::currentDepthTestEnabled;
bool Graphics::currentDepthWriteEnabled;
CompareFunction::Enum Graphics::currentDepthCompareFunction;
bool Graphics::currentDepthBiasEnabled;
float Graphics::currentDepthBiasFactor;
float Graphics::currentDepthBiasUnits;

// Blending
bool Graphics::currentBlendingEnabled;
Color Graphics::currentBlendColorConstant;
BlendFactor::Enum Graphics::currentSourceRGBFactor;
BlendFactor::Enum Graphics::currentDestRGBFactor;
BlendEquation::Enum Graphics::currentRGBBlendEquation;
BlendFactor::Enum Graphics::currentSourceAlphaFactor;
BlendFactor::Enum Graphics::currentDestAlphaFactor;
BlendEquation::Enum Graphics::currentAlphaBlendEquation;

// Texturing
GLuint Graphics::currentTextureIds[];
TextureUnit::Enum Graphics::currentActiveTextureUnit;

// Scissoring
bool Graphics::currentScissorTestEnabled;
GLint Graphics::currentScissorX;
GLint Graphics::currentScissorY;
GLsizei Graphics::currentScissorWidth;
GLsizei Graphics::currentScissorHeight;

// Shader
GLuint Graphics::currentShaderProgramId = 0;

// Vertex Attributes
bool Graphics::currentEnabledVertexAttributes[];
GLuint Graphics::currentVertexBufferId = 0;
GLuint Graphics::currentIndexBufferId = 0;

// Global Shader Parameters
SharedUniformValueSet Graphics::GlobalUniformsSet;
Graphics::_GlobalUniforms Graphics::GlobalUniforms;

// *****************************************************************
/// @brief
///     Initializes the GDK Graphics sub-system
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Graphics::Init()
{
	// Setup the initial render states
	// -------------------------------

	// Get the 

	// Get various System/OpenGL graphics properties
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &supportedTextureUnits);
	if(supportedTextureUnits > TextureUnit::MaxUnits)
		supportedTextureUnits = TextureUnit::MaxUnits;

	vendorString.assign((const char*)glGetString(GL_VENDOR));
	rendererString.assign((const char*)glGetString(GL_RENDERER));
	versionString.assign((const char*)glGetString(GL_VERSION));

	// Log the vendor/renderer details
	LOG_SYSTEM("GL Vendor: %s", vendorString.c_str());
	LOG_SYSTEM("GL Renderer: %s", rendererString.c_str());
	LOG_SYSTEM("GL Version: %s", versionString.c_str());

	// Pre-set all static properties
	for(int i=0; i<supportedTextureUnits; i++)
	{
		currentTextureIds[i] = GL_INVALID_VALUE;
	}

	// Determine the maximum supported shader profile
	if(Device::GetDeviceType() == DeviceType::PC)
	{
		// PC Device, Using GLSL
		shaderProfile = ShaderProfile::GLSL;

		// Check for higher level profiles
		char* shaderVersionString = (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
		int majorSL = 0, minorSL = 0;
		GDK_SSCANF(shaderVersionString, "%d.%d", &majorSL, &minorSL);
		
		if(majorSL >= 4)
			shaderProfile = ShaderProfile::GLSL_4_0;
		if(majorSL >= 3 && minorSL >= 3)
			shaderProfile = ShaderProfile::GLSL_3_3;
	}
	else
	{
		// Mobile or Web device, using GLES
		shaderProfile = ShaderProfile::GLES;
	}


	// Basics
	SetColorWriteMask(true, true, true, true, true);

	// Culling
	SetCullingMode(CullingMode::Clockwise, true);

	// Depth
	EnableDepthTest(false, true);
	EnableDepthWrite(true, true);
	SetDepthTestFunction(CompareFunction::Less, true);
	EnableDepthBias(true, true);
	SetDepthBias(0.0f, 0.0f, true);

	// Blending
	EnableBlending(true, true);
	SetBlendColorConstant(Color(0,0,0,0), true);
	SetBlendMode(BlendMode::Normal, true);

	// Scissor
	EnableScissorTest(false, true);
	SetScissorRect(0, 0, Application::GetWidth(), Application::GetHeight(), true);

	// Vertex Attributes
	for(int i=0; i<MAX_VERTEX_ATTRIBUTES; i++)
		EnableVertexAttribArray(i, false, true);

	// Setup the global shared shader parameters
	// ------------------------------------------

	// M/V/P
	
	GlobalUniforms.World						= GlobalUniformsSet.Add("World", UniformTypes::Matrix4);
	GlobalUniforms.View							= GlobalUniformsSet.Add("View", UniformTypes::Matrix4);
	GlobalUniforms.Projection					= GlobalUniformsSet.Add("Projection", UniformTypes::Matrix4);

	GlobalUniforms.WorldView					= GlobalUniformsSet.Add("WorldView", UniformTypes::Matrix4);
	GlobalUniforms.WorldViewProjection			= GlobalUniformsSet.Add("WorldViewProjection", UniformTypes::Matrix4);

	// Camera
	GlobalUniforms.CameraPosition				= GlobalUniformsSet.Add("CameraPosition", UniformTypes::Float3);
	GlobalUniforms.WorldUp						= GlobalUniformsSet.Add("WorldUp", UniformTypes::Float3);

	GlobalUniforms.WorldUp->SetFloat3(0.0f, 1.0f, 0.0f);

	// Material
	GlobalUniforms.Material.Emissive			= GlobalUniformsSet.Add("Material.Emissive", UniformTypes::Float4);
	GlobalUniforms.Material.Ambient				= GlobalUniformsSet.Add("Material.Ambient", UniformTypes::Float4);
	GlobalUniforms.Material.Diffuse				= GlobalUniformsSet.Add("Material.Diffuse", UniformTypes::Float4);
	GlobalUniforms.Material.Specular			= GlobalUniformsSet.Add("Material.Specular", UniformTypes::Float4);
	GlobalUniforms.Material.Shininess			= GlobalUniformsSet.Add("Material.Shininess", UniformTypes::Float);
	GlobalUniforms.Material.Alpha				= GlobalUniformsSet.Add("Material.Alpha", UniformTypes::Float);

	GlobalUniforms.Material.Diffuse->SetFloat4(1.0f, 1.0f, 1.0f, 1.0f);
	GlobalUniforms.Material.Alpha->SetFloat(1.0f);

	// Lighting
	GlobalUniforms.AmbientLight					= GlobalUniformsSet.Add("AmbientLight", UniformTypes::Float3);
	GlobalUniforms.NumActiveLights				= GlobalUniformsSet.Add("NumActiveLights", UniformTypes::Int);
	GlobalUniforms.LightPositionsAndFalloffs	= GlobalUniformsSet.Add("LightPositionsAndFalloffs", UniformTypes::Float4, 8);
	GlobalUniforms.LightColors					= GlobalUniformsSet.Add("LightColors", UniformTypes::Float3, 8);

	GlobalUniforms.AmbientLight->SetFloat3(0.0f, 0.0f, 0.0f);
	GlobalUniforms.NumActiveLights->SetInt(0);
	GlobalUniforms.LightPositionsAndFalloffs->SetFloat4(0.0f, 0.0f, 0.0f, 1000.0f, 0);
	GlobalUniforms.LightColors->SetFloat3(0.0f, 0.0f, 0.0f, 0);
}

// *****************************************************************
/// @brief
///     Initializes any Graphics systems that depend on assets
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Graphics::InitAssetDependencies()
{
	// Initialize Graphics Sub-Managers
	// ------------------------------------------

    Drawing2D::Init();
	BillboardRenderer::Init();
}

// *****************************************************************
/// @brief
///     Shuts down the GDK graphics sub-system
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Graphics::Shutdown()
{
	// Shutdown Graphics Sub-Managers
	// ------------------------------------------

	BillboardRenderer::Shutdown();
    Drawing2D::Shutdown();
}

// *****************************************************************
/// @brief
///     The per-frame update method for the graphics system
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Graphics::Update(float elapsedSeconds)
{
    // Bind the OS frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, osFrameBufferId);

    // Setup the viewport to full screen
    glViewport(0, 0, Application::GetWidth(), Application::GetHeight());
}

// *****************************************************************
/// @brief
///     Tells the GDK graphics system about the OS supplied frame buffer & render buffers
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Graphics::Platform_SetOSFrameBuffers(GLuint frameBufferId, GLuint colorRenderBufferId, GLuint depthRenderBufferId, GLuint stencilRenderBufferId)
{
    osFrameBufferId = frameBufferId;
    osColorRenderBufferId = colorRenderBufferId;
    osDepthRenderBufferId = depthRenderBufferId;
    osStencilRenderBufferId = stencilRenderBufferId;
}

// *****************************************************************
/// @brief
///     Gets the width of the application window / view
// *****************************************************************
int Graphics::GetScreenWidth()
{
	return Application::GetWidth();
}

// *****************************************************************
/// @brief
///     Gets the height of the application window / view
// *****************************************************************
int Graphics::GetScreenHeight()
{
	return Application::GetHeight();
}

// *****************************************************************
/// @brief
///     Gets the size of the application window / view
// *****************************************************************
Vector2 Graphics::GetScreenBounds()
{
	return Vector2((float)GetScreenWidth(), (float)GetScreenHeight());
}

// *****************************************************************
/// @brief
///     Gets the OpenGL ID of the OS created main frame buffer
// *****************************************************************
GLuint Graphics::GetOSFrameBufferId()				
{
    return osFrameBufferId; 
}
// *****************************************************************
/// @brief
///     Gets the OpenGL ID of the OS created main color render buffer
// *****************************************************************
GLuint Graphics::GetOSColorRenderBufferId()		
{
    return osColorRenderBufferId; 
}

// *****************************************************************
/// @brief
///     Gets the OpenGL ID of the OS created main depth render buffer
// *****************************************************************
GLuint Graphics::GetOSDepthRenderBufferId()		
{
    return osDepthRenderBufferId; 
}

// *****************************************************************
/// @brief
///     Gets the OpenGL ID of the OS created main stencil render buffer
// *****************************************************************
GLuint Graphics::GetOSStencilRenderBufferId()		
{
    return osStencilRenderBufferId; 
}

// *****************************************************************
/// @brief
///     Gets the display-friend vendor name of the underlying OpenGL driver
// *****************************************************************
const string& Graphics::GetGLVendor()
{
    return vendorString; 
}

// *****************************************************************
/// @brief
///     Gets the display-friend renderer name of the underlying OpenGL driver
// *****************************************************************
const string& Graphics::GetGLRenderer()
{
    return rendererString; 
}

// *****************************************************************
/// @brief
///     Gets the display-friend version of the underlying OpenGL driver
// *****************************************************************
const string& Graphics::GetGLVersion()
{ 
    return versionString; 
}

// *****************************************************************
/// @brief
///     Gets the number of texture units supported by the underlying graphics system
// *****************************************************************
int Graphics::GetNumSupportedTextureUnits()
{ 
    return supportedTextureUnits; 
}

// *****************************************************************
/// @brief
///     Gets the best shader profile supported by the underlying graphics system
// *****************************************************************
ShaderProfile::Enum Graphics::GetShaderProfile()
{ 
    return shaderProfile; 
}

// *****************************************************************
/// @brief
///     Converts a screen coordinate to [0 to 1] unit space
// *****************************************************************
Vector2 Graphics::ScreenToUnit(Vector2 screenCoordinate)
{
	// Convert the pixel coordinate to [0 to 1) unit space
	return Vector2(
		screenCoordinate.X / Application::GetWidth(),
		screenCoordinate.Y / Application::GetHeight()
		);
}

// *****************************************************************
/// @brief
///     Converts a screen coordinate to [-1 to 1] symmetric space
// *****************************************************************
Vector2 Graphics::ScreenToSymmetric(Vector2 screenCoordinate)
{
	// Convert the pixel coordinate to (-1 to 1) symmetric space
	return Vector2(
		2.0f * screenCoordinate.X / Application::GetWidth() - 1.0f,
		2.0f * (1.0f - screenCoordinate.Y / Application::GetHeight()) - 1.0f
		);
}

// *****************************************************************
/// @brief
///     Converts a unit coordinate [0 to 1] to screen space
// *****************************************************************
Vector2 Graphics::UnitToScreen(Vector2 unitCoordinate)
{
	// Convert the [0 to 1) unit coordinate to pixel space
	return Vector2(
		unitCoordinate.X * Application::GetWidth(),
		unitCoordinate.Y * Application::GetHeight()
		);
}

// *****************************************************************
/// @brief
///     Converts a symmetric coordinate [-1 to 1] to screen space
// *****************************************************************
Vector2 Graphics::SymmetricToScreen(Vector2 symmetricCoordinate)
{
	// Convert the (-1 to 1) symmetric coordinate to pixel space
	return Vector2(
		(0.5f * symmetricCoordinate.X + 0.5f) * Application::GetWidth(),
		(0.5f * symmetricCoordinate.Y + 0.5f) * Application::GetHeight()
		);
}


// *****************************************************************
/// @brief
///     Sets the write mask for the color buffer.
/// @param red
///     If true, the red channel will be written to the in framebuffer
/// @param green
///     If true, the green channel will be written to the in framebuffer
/// @param blue
///     If true, the blue channel will be written to the in framebuffer
/// @param alpha
///     If true, the alpha channel will be written to the in framebuffer
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::SetColorWriteMask(bool red, bool green, bool blue, bool alpha, bool forceChange)
{
	// Only change state if we need to :)
	if( forceChange || currentRedWriteMask != red || currentGreenWriteMask != green || currentBlueWriteMask != blue || currentAlphaWriteMask != alpha)
	{
		glColorMask((GLboolean)red, (GLboolean)green, (GLboolean)blue, (GLboolean)alpha);
		red = currentRedWriteMask;
		green = currentGreenWriteMask;
		blue = currentBlueWriteMask;
		alpha = currentAlphaWriteMask;
	}
}

// *****************************************************************
/// @brief
///     Clears the frame buffer
/// @param clearColor
///     If true, the color render buffer will be cleared.  (default: true)
/// @param clearDepth
///     If true, the depth render buffer will be cleared.  (default: true)
/// @param clearStencil
///     If true, the stencil render buffer will be cleared.  (default: false)
/// @param color
///     A color value that the color render buffer will be set to, if clearColor == true.  (default: 0,0,0,0)
/// @param depth
///     A float value that the depth render buffer will be set to, if clearDepth == true.  (default: 1.0)
/// @param stencil
///     A integer value that the stencil render buffer will be set to, if clearStencil == true.  (default: 0)
/// @remarks
///     This method will re-enable the color mask & depth writing when clearing those buffers
// *****************************************************************
void Graphics::Clear(
    bool clearColor, bool clearDepth, bool clearStencil,
    const Color& color, float depth, int stencil
    )
{
	glClearColor(
		color.R * Color::NORMALIZATION_SCALAR,
		color.G * Color::NORMALIZATION_SCALAR,
		color.B * Color::NORMALIZATION_SCALAR,
		color.A * Color::NORMALIZATION_SCALAR
		);
	
#ifdef GDKPLATFORM_IOS
	glClearDepthf(depth);
#else
	glClearDepth(depth);
#endif

	glClearStencil(stencil);
    
    // Set the bits of which buffers to clear
    GLuint buffers = 0;
    if(clearColor)
    {
        Graphics::SetColorWriteMask(true, true, true, true);
        buffers |= GL_COLOR_BUFFER_BIT;
    }
    if(clearDepth)
    {
        Graphics::EnableDepthWrite(true);
        buffers |= GL_DEPTH_BUFFER_BIT;
    }
    if(clearStencil)
    {
        buffers |= GL_STENCIL_BUFFER_BIT;
    }

	glClear((GLbitfield)buffers);
}

// *****************************************************************
/// @brief
///     Sets the current culling mode
/// @param cullingMode
///     The culling mode to set
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::SetCullingMode(CullingMode::Enum cullingMode, bool forceChange)
{
	// Only change state if we need to :)
	if(cullingMode != currentCullingMode || forceChange)
	{
		if(cullingMode == CullingMode::Disabled)
			glDisable(GL_CULL_FACE);
		else
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			if(cullingMode == CullingMode::CCW)
				glFrontFace(GL_CW); // Set the 'back' face to CCW, which is culled
			else
				glFrontFace(GL_CCW); // Set the 'back' face to CW, which is culled
		}

		currentCullingMode = cullingMode;
	}
}

// *****************************************************************
/// @brief
///     Enables or Disables depth testing
/// @remarks
///     Depth testing will cause the Z-value of a pixel to be tested
///     against the depth buffer using the depth test function.  If the 
///     test passes, the pixel will be written to the frame buffer.
/// @param enabled
///     true to enable depth testing.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for GL_DEPTH_TEST for more information 
///     on depth testing
// *****************************************************************
void Graphics::EnableDepthTest(bool enabled, bool forceChange)
{
	// Only change state if we need to
	if(currentDepthTestEnabled != enabled || forceChange)
	{
		if(enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		currentDepthTestEnabled = enabled;
	}
}

// *****************************************************************
/// @brief
///     Enables or Disables depth writing
/// @remarks
///     If enabled, the Z-value of a pixel will be written to the depth buffer.
///     Assuming the depth test passed or depth testing is disabled.
/// @param enabled
///     true to enable depth writing.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for glDepthMask for more information 
///     on depth writing
// *****************************************************************
void Graphics::EnableDepthWrite(bool enabled, bool forceChange)
{
	// Only change state if we need to
	if(currentDepthWriteEnabled != enabled || forceChange)
	{
		if(enabled)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);

		currentDepthWriteEnabled = enabled;
	}
}

// *****************************************************************
/// @brief
///     Sets the comparison function used for depth testing.
/// @param depthFunction
///     The comparison function to use
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::SetDepthTestFunction(CompareFunction::Enum depthFunction, bool forceChange)
{
	// Only change state if we need to
	if(currentDepthCompareFunction != depthFunction || forceChange)
	{
		glDepthFunc((GLenum)depthFunction);
		currentDepthCompareFunction = depthFunction;
	}
}


// *****************************************************************
/// @brief
///     Enables or Disables depth biasing
/// @param enabled
///     true to enable depth biasing.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::EnableDepthBias(bool enabled, bool forceChange)
{
	// Only change state if we need to
	if(currentDepthBiasEnabled != enabled || forceChange)
	{
		if(enabled)
			glEnable(GL_POLYGON_OFFSET_FILL);
		else
			glDisable(GL_POLYGON_OFFSET_FILL);

		currentDepthBiasEnabled = enabled;
	}
}

// *****************************************************************
/// @brief
///     Sets the depth bias amount
/// @remarks
///     bias = (factor * DZ) + (units * EPSILON)
///     where:
///		@li   DZ = change in depth of the current polygon, across the current pixel
///		@li   EPSILON = minimum noticable change.
/// @param factor
///     A scalar applied to the change in the Z-value
/// @param units
///     The number of float epsilons to bias by
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for glPolygonOffset() for more information 
///     on depth biasing
// *****************************************************************
void Graphics::SetDepthBias(float factor, float units, bool forceChange)
{
	// Only change state if we need to
	if(currentDepthBiasFactor != factor || currentDepthBiasUnits != units || forceChange)
	{
		glPolygonOffset(factor, units);
		currentDepthBiasFactor = factor;
		currentDepthBiasUnits = units;
	}
}

// *****************************************************************
/// @brief
///     Enables or Disables frame buffer blending
/// @param enabled
///     true to enable blending.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::EnableBlending(bool enabled, bool forceChange)
{
	// Only change state if we need to
	if(currentBlendingEnabled != enabled || forceChange)
	{
		if(enabled)
			glEnable(GL_BLEND);
		else
			glEnable(GL_BLEND);

		currentBlendingEnabled = enabled;
	}
}

// *****************************************************************
/// @brief
///     Sets the constant color value used by the Constant blend factor.
/// @param color
///     The color value value for the constant
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::SetBlendColorConstant(const Color& color, bool forceChange)
{
	// Only change state if we need to
	if(currentBlendColorConstant != color || forceChange)
	{
		float scalar = 1.0f / 255.0f;
		glBlendColor(color.R * scalar, color.G * scalar, color.B * scalar, color.A * scalar);
		currentBlendColorConstant = color;
	}
}

// *****************************************************************
/// @brief
///     Sets up blending to use one of the common GDK blend modes
/// @param blendMode
///     The common blend mode to setup
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::SetBlendMode(BlendMode::Enum blendMode, bool forceChange)
{
	BlendFactor::Enum sourceRGBFactor;
	BlendFactor::Enum destRGBFactor;
	BlendEquation::Enum rgbBlendEquation;
	BlendFactor::Enum sourceAlphaFactor;
	BlendFactor::Enum destAlphaFactor;
	BlendEquation::Enum alphaBlendEquation;

	// Get the blend settings for the given mode
	BlendMode::GetBlendSettings(
		blendMode,
		sourceRGBFactor, destRGBFactor, rgbBlendEquation,
		sourceAlphaFactor, destAlphaFactor, alphaBlendEquation
		);

	// Apply these blend settings
	SetBlendMode(
		sourceRGBFactor, destRGBFactor, rgbBlendEquation,
		sourceAlphaFactor, destAlphaFactor, alphaBlendEquation,
		forceChange
		);
}

// *****************************************************************
/// @brief
///     Sets up the source & destination factors and the equation used for blending
/// @remarks
///     For more details on advanced blending setup, see the glBlendEquation() & glBlendFunc()
///     documentation in OpenGL
/// @param sourceFactor
///     Factor to use for the source component of the blend
/// @param destFactor
///     Factor to use for the destination component of the blend
/// @param blendEquation
///     Blend equation to use when doing the blending
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for glBlendEquation() & glBlendFunc() 
///     for information on advanced blending setup
// *****************************************************************
void Graphics::SetBlendMode(
	BlendFactor::Enum sourceFactor, BlendFactor::Enum destFactor, BlendEquation::Enum blendEquation,
	bool forceChange
	)
{
	// Only change state if we 'need' to
	if( forceChange ||
		currentSourceRGBFactor != sourceFactor || currentSourceAlphaFactor != sourceFactor ||
		currentDestRGBFactor != destFactor || currentDestAlphaFactor != destFactor)
	{
		glBlendFunc((GLenum)sourceFactor, (GLenum)destFactor);
		currentSourceRGBFactor = currentSourceAlphaFactor = sourceFactor;
		currentDestRGBFactor = currentDestAlphaFactor = destFactor;
	}

	if( forceChange ||
		currentRGBBlendEquation != blendEquation || currentAlphaBlendEquation != blendEquation)
	{
		glBlendEquation((GLenum)blendEquation);
		currentRGBBlendEquation = currentAlphaBlendEquation = blendEquation;
	}
}

// *****************************************************************
/// @brief
///     Sets up the source & destination factors and the equation used for blending.
/// @remarks
///     This method allows you to specify different factors and equations for the color
///     and the alpha channels.
/// @param sourceRGBFactor
///     Factor to use for the source color component of the blend
/// @param destRGBFactor
///     Factor to use for the destination color component of the blend
/// @param rgbBlendEquation
///     Blend equation to use when blending the color value
/// @param sourceAlphaFactor
///     Factor to use for the source alpha component of the blend
/// @param destAlphaFactor
///     Factor to use for the destination alpha component of the blend
/// @param alphaBlendEquation
///     Blend equation to use when blending the alpha value
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for glBlendEquationSeparate() & 
///     glBlendFuncSeparate() for information on advanced blending setup
// *****************************************************************
void Graphics::SetBlendMode(
	BlendFactor::Enum sourceRGBFactor, BlendFactor::Enum destRGBFactor, BlendEquation::Enum rgbBlendEquation,
	BlendFactor::Enum sourceAlphaFactor, BlendFactor::Enum destAlphaFactor, BlendEquation::Enum alphaBlendEquation,
	bool forceChange
	)
{
	// Only change state if we 'need' to
	if( forceChange ||
		currentSourceRGBFactor != sourceRGBFactor || currentSourceAlphaFactor != sourceAlphaFactor ||
		currentDestRGBFactor != destRGBFactor || currentDestAlphaFactor != destAlphaFactor)
	{
		glBlendFuncSeparate((GLenum)sourceRGBFactor, (GLenum)destRGBFactor, (GLenum)sourceAlphaFactor, (GLenum)destAlphaFactor);
		currentSourceRGBFactor = sourceRGBFactor;
		currentSourceAlphaFactor = sourceAlphaFactor;
		currentDestRGBFactor = destRGBFactor;
		currentDestAlphaFactor = destAlphaFactor;
	}

	if( forceChange ||
		currentRGBBlendEquation != rgbBlendEquation || currentAlphaBlendEquation != alphaBlendEquation)
	{
		glBlendEquationSeparate((GLenum)rgbBlendEquation, (GLenum)alphaBlendEquation);
		currentRGBBlendEquation = rgbBlendEquation;
		currentAlphaBlendEquation = alphaBlendEquation;
	}
}

// *****************************************************************
/// @brief
///     Sets the active texture unit used by GL for texture state changes
/// @param textureUnit
///     The texture unit to make active.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::SetActiveTextureUnit(
	TextureUnit::Enum textureUnit, 
	bool forceChange
	)
{
	// Only change state if we need to :)
	if(textureUnit != currentActiveTextureUnit || forceChange)
	{
		glActiveTexture((GLenum)textureUnit);
		currentActiveTextureUnit = textureUnit;
	}
}

// *****************************************************************
/// @brief
///     Binds an OpenGL texture ID to a texture unit.
/// @param glTextureId
///     ID of the OpenGL texture to be bound.
/// @param textureUnit
///     The texture unit to be bound to
/// @param textureTarget
///     The type of texture being bound.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::BindTexture(
	GLuint glTextureId, 
	TextureUnit::Enum textureUnit, 
	TextureTarget::Enum textureTarget,
	bool forceChange
	)
{
	int textureIndex = TextureUnit::ToIndex(textureUnit);

	// Only change state if we need to :)
	if(glTextureId != currentTextureIds[textureIndex] || forceChange)
	{
		// Make sure this is the active texture unit
		SetActiveTextureUnit(textureUnit, forceChange);
		
		// Bind the texture
		glBindTexture((GLenum)textureTarget, glTextureId);
		currentTextureIds[textureIndex] = glTextureId;
	}
}

// *****************************************************************
/// @brief
///     Enables or Disables scissor testing
/// @param enabled
///     true to enable scissor testing.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for GL_SCISSOR_TEST & glScissor()
///     for information on scissor testing
// *****************************************************************
void Graphics::EnableScissorTest(
	bool enabled, 
	bool forceChange
	)
{
	// Only change state if we need to :)
	if(enabled != currentScissorTestEnabled || forceChange)
	{
		if(enabled == true)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);

		currentScissorTestEnabled = enabled;
	}
}

// *****************************************************************
/// @brief
///     Sets the rectangular screen region used for scissor testing
/// @param x
///     X coordinate of the scissor rectangle, in screen/window coordinates
/// @param y
///     Y coordinate of the scissor rectangle, in screen/window coordinates
/// @param width
///     Width of the scissor rectangle, in screen/window coordinates
/// @param height
///     Height of the scissor rectangle, in screen/window coordinates
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for glScissor() for information on scissor testing
// *****************************************************************
void Graphics::SetScissorRect(
	GLint x, GLint y, GLsizei width, GLsizei height,
	bool forceChange
	)
{
	// Only change state if we need to :)
	if (x != currentScissorX ||
		y != currentScissorY ||
		width != currentScissorWidth ||
		height != currentScissorHeight ||
		forceChange )
	{
		glScissor(x, y, width, height);

		currentScissorX = x;
		currentScissorY = y;
		currentScissorWidth = width;
		currentScissorHeight = height;
	}
}

// *****************************************************************
/// @brief
///     Sets the active shader program used by GL
/// @param shaderProgramId
///     ID of the shader program to set as active
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::SetActiveShaderProgram(GLuint shaderProgramId, bool forceChange)
{
	// Only change state if we need to :)
	if(shaderProgramId != currentShaderProgramId || forceChange)
	{
		glUseProgram(shaderProgramId);
		currentShaderProgramId = shaderProgramId;
	}
}

// *****************************************************************
/// @brief
///     Enables or Disables the vertex attribute array on a given index.
/// @param index
///     Index of the vertex attribute array to enable/disable
/// @param enabled
///     true to enable the vertex attribute array
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
/// @note
///     See the OpenGL documentation for glEnableVertexAttribArray() for 
///     more information
// *****************************************************************
void Graphics::EnableVertexAttribArray(int index, bool enabled, bool forceChange)
{
	// Only change state if we need to :)
	if(currentEnabledVertexAttributes[index] != enabled || forceChange)
	{
		if(enabled)
			glEnableVertexAttribArray(index);
		else
			glDisableVertexAttribArray(index);
		currentEnabledVertexAttributes[index] = enabled;
	}
}

// *****************************************************************
/// @brief
///     Enables the first N vertex attributes arrays and disables the rest
/// @param numToEnable
///     Number of the vertex attribute arrays to enable
/// @note
///     See the OpenGL documentation for glEnableVertexAttribArray() for 
///     more information
// *****************************************************************
void Graphics::EnableVertexAttribArrays(int numToEnable)
{
	// Only change state if we need to :)
	EnableVertexAttribArray(0, numToEnable>0, false);
    EnableVertexAttribArray(1, numToEnable>1, false);
    EnableVertexAttribArray(2, numToEnable>2, false);
    EnableVertexAttribArray(3, numToEnable>3, false);
    EnableVertexAttribArray(4, numToEnable>4, false);
    EnableVertexAttribArray(5, numToEnable>5, false);
    EnableVertexAttribArray(6, numToEnable>6, false);
    EnableVertexAttribArray(7, numToEnable>7, false);
}

// *****************************************************************
/// @brief
///     Sets the active vertex buffer for use by OpenGL
/// @param glVertexBufferId
///     Id of the OpenGL vertex buffer to be bound.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::BindVertexBuffer(GLuint glVertexBufferId, bool forceChange)
{
	// Only change state if we need to :)
	if(glVertexBufferId != currentVertexBufferId || forceChange)
	{
		glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferId);
		currentVertexBufferId = glVertexBufferId;
	}
}

// *****************************************************************
/// @brief
///     Sets the active index buffer for use by OpenGL
/// @param glIndexBufferId
///     Id of the OpenGL index buffer to be bound.
/// @param forceChange
///     Skips the GDK's graphics state update caching system and forces the 
///     underlying graphics system to be updated.  (default: false)
// *****************************************************************
void Graphics::BindIndexBuffer(GLuint glIndexBufferId, bool forceChange)
{
	// Only change state if we need to :)
	if(glIndexBufferId != currentIndexBufferId || forceChange)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndexBufferId);
		currentIndexBufferId = glIndexBufferId;
	}
}
