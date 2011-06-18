
#include "BasePCH.h"
#include "Graphics.h"

#include "2D/Renderer2D.h"
#include "3D/BillboardRenderer.h"

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

// ================================================================================================
// == Init() / Shutdown()
// ================================================================================================

// *****************************************************
void Graphics::Init()
{
	// Setup the initial render states
	// -------------------------------

	// Get various System/OpenGL graphics properties
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &supportedTextureUnits);
	if(supportedTextureUnits > TextureUnit::MaxUnits)
		supportedTextureUnits = TextureUnit::MaxUnits;

	vendorString.assign((const char*)glGetString(GL_VENDOR));
	rendererString.assign((const char*)glGetString(GL_RENDERER));
	versionString.assign((const char*)glGetString(GL_VERSION));

	// Log the vendor/renderer details
	LOG_SYSTEM(L"GL Vendor: %hs", vendorString.c_str());
	LOG_SYSTEM(L"GL Renderer: %hs", rendererString.c_str());
	LOG_SYSTEM(L"GL Version: %hs", versionString.c_str());

	// Pre-set all static properties
	for(int i=0; i<supportedTextureUnits; i++)
	{
		currentTextureIds[i] = GL_INVALID_VALUE;
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

// *****************************************************
void Graphics::InitAssetDependencies()
{
	// Initialize Graphics Sub-Managers
	// ------------------------------------------

	Renderer2D::Init();
	BillboardRenderer::Init();
}

// *****************************************************
void Graphics::Shutdown()
{
	// Shutdown Graphics Sub-Managers
	// ------------------------------------------

	BillboardRenderer::Shutdown();
	Renderer2D::Shutdown();
}

// *****************************************************
void Graphics::Update(float elapsedSeconds)
{
    // Bind the OS frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, osFrameBufferId);

    // Setup the viewport to full screen
    glViewport(0, 0, Application::GetWidth(), Application::GetHeight());
}

// ================================================================================================
// == Platform Interfaces
// ================================================================================================

// *****************************************************
void Graphics::Platform_SetOSFrameBuffers(GLuint frameBufferId, GLuint colorRenderBufferId, GLuint depthRenderBufferId, GLuint stencilRenderBufferId)
{
    osFrameBufferId = frameBufferId;
    osColorRenderBufferId = colorRenderBufferId;
    osDepthRenderBufferId = depthRenderBufferId;
    osStencilRenderBufferId = stencilRenderBufferId;
}

// ================================================================================================
// == Screen Dimensions & Coordinate Conversions
// ================================================================================================

// *****************************************************
int Graphics::GetScreenWidth()
{
	return Application::GetWidth();
}

// *****************************************************
int Graphics::GetScreenHeight()
{
	return Application::GetHeight();
}

// *****************************************************
Vector2 Graphics::GetScreenBounds()
{
	return Vector2((float)GetScreenWidth(), (float)GetScreenHeight());
}

// *****************************************************
Vector2 Graphics::ScreenToUnit(Vector2 screenCoordinate)
{
	// Convert the pixel coordinate to [0 to 1) unit space
	return Vector2(
		screenCoordinate.X / Application::GetWidth(),
		screenCoordinate.Y / Application::GetHeight()
		);
}

// *****************************************************
Vector2 Graphics::ScreenToSymmetric(Vector2 screenCoordinate)
{
	// Convert the pixel coordinate to (-1 to 1) symmetric space
	return Vector2(
		2.0f * screenCoordinate.X / Application::GetWidth() - 1.0f,
		2.0f * screenCoordinate.Y / Application::GetHeight() - 1.0f
		);
}

// *****************************************************
Vector2 Graphics::UnitToScreen(Vector2 unitCoordinate)
{
	// Convert the [0 to 1) unit coordinate to pixel space
	return Vector2(
		unitCoordinate.X * Application::GetWidth(),
		unitCoordinate.Y * Application::GetHeight()
		);
}

// *****************************************************
Vector2 Graphics::SymmetricToScreen(Vector2 symmetricCoordinate)
{
	// Convert the (-1 to 1) symmetric coordinate to pixel space
	return Vector2(
		(0.5f * symmetricCoordinate.X + 0.5f) * Application::GetWidth(),
		(0.5f * symmetricCoordinate.Y + 0.5f) * Application::GetHeight()
		);
}


// ================================================================================================
// == Basics
// ================================================================================================

// *****************************************************
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

// *****************************************************
void Graphics::Clear(ClearBuffers::Enum buffers, const Color& color, float depth, int stencil)
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
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
	glClear((GLbitfield)buffers);
}

// ================================================================================================
// == Culling
// ================================================================================================

// *****************************************************
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

// ================================================================================================
// == Depth
// ================================================================================================

// *****************************************************
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

// *****************************************************
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

// *****************************************************
void Graphics::SetDepthTestFunction(CompareFunction::Enum depthFunction, bool forceChange)
{
	// Only change state if we need to
	if(currentDepthCompareFunction != depthFunction || forceChange)
	{
		glDepthFunc((GLenum)depthFunction);
		currentDepthCompareFunction = depthFunction;
	}
}


// *****************************************************
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

// *****************************************************
void Graphics::SetDepthBias(float factor, float units, bool forceChange)
{
	// bias = (factor * DZ) + (units * EPSILON)
	// where:
	//		DZ = change in depth of the current polygon, across the current pixel
	//		EPSILON = minimum noticable change.

	// Only change state if we need to
	if(currentDepthBiasFactor != factor || currentDepthBiasUnits != units || forceChange)
	{
		glPolygonOffset(factor, units);
		currentDepthBiasFactor = factor;
		currentDepthBiasUnits = units;
	}
}

// ================================================================================================
// == Blending
// ================================================================================================

// *****************************************************
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

// *****************************************************
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

// *****************************************************
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

// *****************************************************
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

// *****************************************************
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

// ================================================================================================
// == Texturing
// ================================================================================================

// *****************************************************
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

// *****************************************************
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

// ================================================================================================
// == Scissoring
// ================================================================================================

// *****************************************************
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

// *****************************************************
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

// ================================================================================================
// == Shaders
// ================================================================================================

// *****************************************************
void Graphics::SetActiveShaderProgram(GLuint shaderProgramId, bool forceChange)
{
	// Only change state if we need to :)
	if(shaderProgramId != currentShaderProgramId || forceChange)
	{
		glUseProgram(shaderProgramId);
		currentShaderProgramId = shaderProgramId;
	}
}

// ================================================================================================
// == Vertex Attributes
// ================================================================================================

// *****************************************************
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

// *****************************************************
void Graphics::BindVertexBuffer(GLuint glVertexBufferId, bool forceChange)
{
	// Only change state if we need to :)
	if(glVertexBufferId != currentVertexBufferId || forceChange)
	{
		glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferId);
		currentVertexBufferId = glVertexBufferId;
	}
}

// *****************************************************
void Graphics::BindIndexBuffer(GLuint glIndexBufferId, bool forceChange)
{
	// Only change state if we need to :)
	if(glIndexBufferId != currentIndexBufferId || forceChange)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndexBufferId);
		currentIndexBufferId = glIndexBufferId;
	}
}
