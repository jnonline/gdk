/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Shader.h"

using namespace Gdk;

// ***********************************************************************
Shader::Shader()
{
	this->CurrentTechnique = NULL;
	
	// Setup default render states
	// --------------------------

	// Color Mask
	this->RenderState.ColorMask.Apply = false;
	this->RenderState.ColorMask.RedWriteEnabled = true;
	this->RenderState.ColorMask.GreenWriteEnabled = true;
	this->RenderState.ColorMask.BlueWriteEnabled = true;
	this->RenderState.ColorMask.AlphaWriteEnabled = true;

	// Blending
	this->RenderState.Blending.Apply = false;
	this->RenderState.Blending.Enabled = true;
	this->RenderState.Blending.ConstantColor = Color::TRANSPARENT_BLACK;
	this->RenderState.Blending.SourceRGBFactor = BlendFactor::SrcAlpha;
	this->RenderState.Blending.DestRGBFactor = BlendFactor::OneMinusSrcAlpha;
	this->RenderState.Blending.RGBBlendEquation = BlendEquation::Add;
	this->RenderState.Blending.SourceAlphaFactor = BlendFactor::SrcAlpha;
	this->RenderState.Blending.DestAlphaFactor = BlendFactor::OneMinusSrcAlpha;
	this->RenderState.Blending.AlphaBlendEquation = BlendEquation::Add;

	// Depth
	this->RenderState.Depth.Apply = false;
	this->RenderState.Depth.TestEnabled = true;
	this->RenderState.Depth.WriteEnabled = true;
	this->RenderState.Depth.TestFunction = CompareFunction::Less;
	this->RenderState.Depth.BiasEnabled = true;
	this->RenderState.Depth.BiasFactor = 0.0f;
	this->RenderState.Depth.BiasUnits = 0.0f;
	
	// Depth
	this->RenderState.Culling.Apply = false;
	this->RenderState.Culling.CullMode = CullingMode::Clockwise;
}

// ***********************************************************************
Shader::~Shader()
{
	// Clean up the attributes
	for(ShaderAttributeVector::iterator iter = Attributes.begin(); iter != Attributes.end(); iter++)
	{
		ShaderAttribute* attribute = *iter;
		GdkDelete(attribute);
	}
	Attributes.clear();
	AttributesByName.Clear();

	// Clean up the techniques
	for(ShaderTechniqueVector::iterator iter = Techniques.begin(); iter != Techniques.end(); iter++)
	{
		ShaderTechnique* technique = *iter;
		GdkDelete(technique);
	}
	Techniques.clear();
	TechniquesByName.Clear();
}

// ***********************************************************************
void Shader::Apply()
{
	// Get the current technique in use
	ShaderTechnique* technique = this->CurrentTechnique;
	ASSERT(technique != NULL, L"Shader::Apply called with a NULL CurrentTechnique");

	// Bind the shader program
	Graphics::SetActiveShaderProgram(technique->shaderProgramId);

	// Set shader uniforms
	// ---------------------------

	// Loop through the shader uniforms
	for(ShaderUniformVector::iterator iter = technique->Uniforms.begin(); iter != technique->Uniforms.end(); iter++)
	{
		// Apply this uniform
		ShaderUniform* uniform = *iter;
		uniform->Apply();
	}

	// Apply render states
	// ---------------------------

	// Color Mask?
	if(this->RenderState.ColorMask.Apply)
	{
		Graphics::SetColorWriteMask(
			this->RenderState.ColorMask.RedWriteEnabled,
			this->RenderState.ColorMask.GreenWriteEnabled,
			this->RenderState.ColorMask.BlueWriteEnabled,
			this->RenderState.ColorMask.AlphaWriteEnabled
			);
	}

	// Blending?
	if(this->RenderState.Blending.Apply)
	{
		Graphics::EnableBlending(this->RenderState.Blending.Enabled);
		Graphics::SetBlendColorConstant(this->RenderState.Blending.ConstantColor);
		Graphics::SetBlendMode(
			this->RenderState.Blending.SourceRGBFactor, 
			this->RenderState.Blending.DestRGBFactor, 
			this->RenderState.Blending.RGBBlendEquation,
			this->RenderState.Blending.SourceAlphaFactor, 
			this->RenderState.Blending.DestAlphaFactor, 
			this->RenderState.Blending.AlphaBlendEquation
			);
	}

	// Depth?
	if(this->RenderState.Depth.Apply)
	{
		Graphics::EnableDepthTest(this->RenderState.Depth.TestEnabled);
		Graphics::EnableDepthWrite(this->RenderState.Depth.WriteEnabled);
		Graphics::SetDepthTestFunction(this->RenderState.Depth.TestFunction);
		Graphics::EnableDepthBias(this->RenderState.Depth.BiasEnabled);
		Graphics::SetDepthBias(this->RenderState.Depth.BiasFactor, this->RenderState.Depth.BiasUnits);
	}

	// Culling
	if(this->RenderState.Culling.Apply)
	{
		Graphics::SetCullingMode(this->RenderState.Culling.CullMode);
	}
}

// ***********************************************************************
ShaderAttribute* Shader::GetAttributeByName(const char* name)
{
	// Get the attribute with this name
	ShaderAttributeNameMap::Iterator iter = AttributesByName.Find(name);
	if(iter == AttributesByName.End())
		return NULL;

	return iter->second;
}

// ***********************************************************************
ShaderTechnique* Shader::GetTechniqueByName(const char* name)
{
	// Get the uniform with this name
	ShaderTechniqueNameMap::Iterator iter = TechniquesByName.Find(name);
	if(iter == TechniquesByName.End())
		return NULL;

	return iter->second;
}


// ###########################################################################################
//
// Shader Loading & Compiling
//
// ###########################################################################################

// ***********************************************************************
namespace ShaderFlags
{
	enum Enum
    {
        ShaderIsText            = 0x0001
    };
}

// ***********************************************************************
namespace ShaderParameterFlags
{
	enum Enum
    {
        BindToGlobal			= 0x0001
    };
}

// ***********************************************************************
namespace ShaderRenderStateFlags
{
	enum Enum
    {
        ColorMaskApply          = 0x00000001,
        BlendingApply           = 0x00000002,
        DepthApply              = 0x00000004,
        CullingApply            = 0x00000008,
										
        ColorRedWriteEnabled    = 0x00000100,
        ColorGreenWriteEnabled  = 0x00000200,
        ColorBlueWriteEnabled   = 0x00000400,
        ColorAlphaWriteEnabled  = 0x00000800,
		
        BlendingEnabled         = 0x00001000,
        UseSimpleBlendMode      = 0x00002000,
        DepthTestEnabled        = 0x00004000,
        DepthWriteEnabled       = 0x00008000,

        DepthBiasEnabled        = 0x00010000
    };
}


// ***********************************************************************
Shader* Shader::FromFile(const char *shaderFilePath)
{
	// Open the file as a stream
	FileStream fileStream(shaderFilePath, FileMode::Read);

	// Create a shader from the stream
	Shader* shader = FromStream(&fileStream);

	// Close the stream
	fileStream.Close();

	return shader;
}

// ***********************************************************************
Shader* Shader::FromAsset(AssetLoadContext* context)
{
	return FromStream(context->AssetStream);
}		


// ***********************************************************************
Shader* Shader::FromStream(Stream* stream)
{
	// Create the shader object
	Shader* shader = GdkNew Shader();

	// Load the version & shader flags
	UInt16 version = stream->ReadUInt16();
	UInt16 shaderFlags = stream->ReadUInt16();
    
    GDK_NOT_USED(version);
    
	// Load RenderState settings
	// ===============================

	// Load & parse the render state flags
	UInt32 renderStateFlags = stream->ReadUInt32();

	// RS-Apply flags
	shader->RenderState.ColorMask.Apply = (renderStateFlags & ShaderRenderStateFlags::ColorMaskApply) > 0;
	shader->RenderState.Blending.Apply = (renderStateFlags & ShaderRenderStateFlags::BlendingApply) > 0;
	shader->RenderState.Depth.Apply = (renderStateFlags & ShaderRenderStateFlags::DepthApply) > 0;
	shader->RenderState.Culling.Apply = (renderStateFlags & ShaderRenderStateFlags::CullingApply) > 0;

	// Color Mask
	shader->RenderState.ColorMask.RedWriteEnabled = (renderStateFlags & ShaderRenderStateFlags::ColorRedWriteEnabled) > 0;
	shader->RenderState.ColorMask.GreenWriteEnabled = (renderStateFlags & ShaderRenderStateFlags::ColorGreenWriteEnabled) > 0;
	shader->RenderState.ColorMask.BlueWriteEnabled = (renderStateFlags & ShaderRenderStateFlags::ColorBlueWriteEnabled) > 0;
	shader->RenderState.ColorMask.AlphaWriteEnabled = (renderStateFlags & ShaderRenderStateFlags::ColorAlphaWriteEnabled) > 0;
	
	// Blending
	shader->RenderState.Blending.Enabled = (renderStateFlags & ShaderRenderStateFlags::BlendingEnabled) > 0;
	shader->RenderState.Blending.ConstantColor.R = stream->ReadUInt8();
	shader->RenderState.Blending.ConstantColor.G = stream->ReadUInt8();
	shader->RenderState.Blending.ConstantColor.B = stream->ReadUInt8();
	shader->RenderState.Blending.ConstantColor.A = stream->ReadUInt8();
	BlendMode::Enum blendMode = (BlendMode::Enum)stream->ReadUInt16();
	shader->RenderState.Blending.SourceRGBFactor = (BlendFactor::Enum)stream->ReadUInt16();
	shader->RenderState.Blending.DestRGBFactor = (BlendFactor::Enum)stream->ReadUInt16();
	shader->RenderState.Blending.RGBBlendEquation = (BlendEquation::Enum)stream->ReadUInt16();
	shader->RenderState.Blending.SourceAlphaFactor = (BlendFactor::Enum)stream->ReadUInt16();
	shader->RenderState.Blending.DestAlphaFactor = (BlendFactor::Enum)stream->ReadUInt16();
	shader->RenderState.Blending.AlphaBlendEquation = (BlendEquation::Enum)stream->ReadUInt16();
	
	// Does the shader binary use the Simple BlendMode?
	if((renderStateFlags & ShaderRenderStateFlags::UseSimpleBlendMode) > 0)
	{
		BlendMode::GetBlendSettings(blendMode, 
			shader->RenderState.Blending.SourceRGBFactor,
			shader->RenderState.Blending.DestRGBFactor,
			shader->RenderState.Blending.RGBBlendEquation,
			shader->RenderState.Blending.SourceAlphaFactor,
			shader->RenderState.Blending.DestAlphaFactor,
			shader->RenderState.Blending.AlphaBlendEquation
			);
	}

	// Depth 
	shader->RenderState.Depth.TestEnabled = (renderStateFlags & ShaderRenderStateFlags::DepthTestEnabled) > 0;
	shader->RenderState.Depth.WriteEnabled = (renderStateFlags & ShaderRenderStateFlags::DepthWriteEnabled) > 0;
	shader->RenderState.Depth.BiasEnabled = (renderStateFlags & ShaderRenderStateFlags::DepthBiasEnabled) > 0;
	shader->RenderState.Depth.TestFunction = (CompareFunction::Enum) stream->ReadUInt16();
	shader->RenderState.Depth.BiasFactor = stream->ReadFloat();
	shader->RenderState.Depth.BiasUnits = stream->ReadFloat();
  
	// Culling
	shader->RenderState.Culling.CullMode = (CullingMode::Enum) stream->ReadUInt16();

	// Load the attributes
	// ===============================

	// Get the number of attributes
	UInt16 numAttributes = stream->ReadUInt16();

	// Loop through the attributes
	for(UInt16 index=0; index < numAttributes; index++)
	{
		// Read the attribute properties
		string attributeName = stream->ReadString();
		UInt16 attribueType = stream->ReadUInt16();
		UInt16 attributeArraySize = stream->ReadUInt16();
		UInt16 attributeLocation = stream->ReadUInt16();

		// Create an attribute definition
		ShaderAttribute* attribute = GdkNew ShaderAttribute(
			attributeName.c_str(), 
			(ShaderAttributeType::Enum)attribueType, 
			attributeArraySize, 
			attributeLocation
			);
		
		// Store the attribute in the shader
		shader->Attributes.push_back(attribute);
		shader->AttributesByName.Add(attributeName.c_str(), attribute);
	}

	// Load the parameters  (uniform values)
	// ===============================

	// Get the # of parameters
	UInt16 numParameters = stream->ReadUInt16();
	
	// Loop through the parameters
	for(UInt16 index=0; index < numParameters; index++)
	{
		// Read the parameter details
		UInt16 parameterFlags = stream->ReadUInt16();
		string parameterName = stream->ReadString();
		UniformTypes::Enum parameterType = (UniformTypes::Enum)stream->ReadUInt16();
		UInt16 parameterArraySize = stream->ReadUInt16();
		
		// Create a uniform value for this parameter
		UniformValue* parameter = shader->Parameters.Add(parameterName.c_str(), parameterType, parameterArraySize);

		// Is this a BindToGlobal parameter?
		if((parameterFlags & ShaderParameterFlags::BindToGlobal) > 0)
		{
			// Get the global parameter that we're binding to
			string parameterBinding = stream->ReadString();
			UniformValue* sharedUniformValue = Graphics::GlobalUniformsSet.Get(parameterBinding.c_str());
			ASSERT(sharedUniformValue != NULL, L"Unable to bind shader parameter [%hs] to the shared global uniform [%hs]", parameterName.c_str(), parameterBinding.c_str());

			// Bind!
			parameter->Bind(sharedUniformValue);
		}
		else
		{
			// Calculate the size of the value data
			size_t valueDataSize = UniformTypes::SizeOf(parameterType) * parameterArraySize;

			// Read the value data directly into the uniform
			stream->Read(parameter->valueBuffer, valueDataSize);
		}
	}

	// Load Techniques
	// ===============================

	// Get the # of techniques
	UInt16 numTechniques = stream->ReadUInt16();

	// Loop through the techniques
	for(UInt16 index=0; index < numTechniques; index++)
	{
		// Load the technique's Supported/Excluded platforms & devices
		// ------------------------------------------------------------

		// Read the supported platforms
		UInt16 numSupportedPlatforms = stream->ReadUInt16();
		vector<UInt16> supportedPlatforms;
		supportedPlatforms.reserve(numSupportedPlatforms);
		for(UInt16 platformIndex=0; platformIndex < numSupportedPlatforms; platformIndex++)
			supportedPlatforms.push_back( stream->ReadUInt16() );

		// Read the supported devices
		UInt16 numSupportedDevices = stream->ReadUInt16();
		vector<UInt16> supportedDevices;
		supportedDevices.reserve(numSupportedDevices);
		for(UInt16 deviceIndex=0; deviceIndex < numSupportedDevices; deviceIndex++)
			supportedDevices.push_back( stream->ReadUInt16() );

		// Read the excluded platforms
		UInt16 numExcludedPlatforms = stream->ReadUInt16();
		vector<UInt16> excludedPlatforms;
		excludedPlatforms.reserve(numExcludedPlatforms);
		for(UInt16 platformIndex=0; platformIndex < numExcludedPlatforms; platformIndex++)
			excludedPlatforms.push_back( stream->ReadUInt16() );

		// Read the excluded devices
		UInt16 numExcludedDevices = stream->ReadUInt16();
		vector<UInt16> excludedDevices;
		excludedDevices.reserve(numExcludedDevices);
		for(UInt16 deviceIndex=0; deviceIndex < numExcludedDevices; deviceIndex++)
			excludedDevices.push_back( stream->ReadUInt16() );

		// Check if we should include this technique on this platform/device
		// ------------------------------------------------------------------

		bool techniqueSupported = true;
		UInt16 platformType = (UInt16) Device::GetPlatformType();
		UInt16 deviceType = (UInt16) Device::GetDeviceType();

		// Do we have a strict list of supported platforms?  (0 platforms = all platforms are supported)
		if(supportedPlatforms.size() > 0)
		{
			// Is this platform NOT in the supported list?
			vector<UInt16>::iterator iter = find(supportedPlatforms.begin(), supportedPlatforms.end(), platformType);
			if(iter == supportedPlatforms.end())
				techniqueSupported = false;
		}

		// Do we have a strict list of supported devices?  (0 devices = all devices are supported)
		if(supportedDevices.size() > 0)
		{
			// Is this device NOT in the supported list?
			vector<UInt16>::iterator iter = find(supportedDevices.begin(), supportedDevices.end(), deviceType);
			if(iter == supportedDevices.end())
				techniqueSupported = false;
		}
		
		// Check if this platform is specifically excluded
		vector<UInt16>::iterator excludedPlatformIter = find(excludedPlatforms.begin(), excludedPlatforms.end(), platformType);
		if(excludedPlatformIter != excludedPlatforms.end())
			techniqueSupported = false;

		// Check if this device is specifically excluded
		vector<UInt16>::iterator excludedDeviceIter = find(excludedDevices.begin(), excludedDevices.end(), deviceType);
		if(excludedDeviceIter != excludedDevices.end())
			techniqueSupported = false;

		// Is this technique not supported?
		if(techniqueSupported == false)
		{
			// Skip the rest of the technique in the stream
			string techniqueName = stream->ReadString();
			UInt32 vertexShaderSize = stream->ReadUInt32();
			stream->Seek(vertexShaderSize, SeekOrigin::Current);
			UInt32 fragmentShaderSize = stream->ReadUInt32();
			stream->Seek(fragmentShaderSize, SeekOrigin::Current);

			// Move on to the next technique
			continue;
		}

		// Load the technique
		// ----------------------------

		// Create the technique
		ShaderTechnique* technique = GdkNew ShaderTechnique();

		// Load the technique name
		technique->Name = stream->ReadString();

		// Is the shader in text format?
		if(shaderFlags & ShaderFlags::ShaderIsText)
		{
			// Load & Compile Shaders
			// -------------------------

			// Get the size of the Vertex Shader
			UInt32 vertexShaderSize = stream->ReadUInt32();

			// Allocate room & load the Vertex Shader
			char* vertexShaderText = (char*) GdkAlloc(vertexShaderSize + 1);
			vertexShaderText[vertexShaderSize] = 0;
			stream->Read(vertexShaderText, vertexShaderSize); 

			// Get the size of the Fragment Shader
			UInt32 fragmentShaderSize = stream->ReadUInt32();

			// Allocate room & load the Fragment Shader
			char* fragmentShaderText = (char*) GdkAlloc(fragmentShaderSize + 1);
			fragmentShaderText[fragmentShaderSize] = 0;
			stream->Read(fragmentShaderText, fragmentShaderSize);

			// Compile the sources into shaders
			technique->vertexShaderId = CompileGLShaderFromSource(vertexShaderText, GL_VERTEX_SHADER);
			technique->fragmentShaderId = CompileGLShaderFromSource(fragmentShaderText, GL_FRAGMENT_SHADER);

			// Free the text buffers
			GdkFree(vertexShaderText);
			GdkFree(fragmentShaderText);
		}
		else
		{
			// TODO(P2): Binary shader loading
			ASSERT(false, L"Binary shader loading is not implemented");
		}

		// Create & Link Shader Program
		// -------------------------------

		// Create a program
		technique->shaderProgramId = glCreateProgram();
		ASSERT(technique->shaderProgramId != 0, L"glCreateProgram() failed");

		// Attach the shaders to the program
		glAttachShader(technique->shaderProgramId, technique->vertexShaderId);
		glAttachShader(technique->shaderProgramId, technique->fragmentShaderId);

		// Bind the attribute locations
		for(ShaderAttributeVector::iterator iter = shader->Attributes.begin(); iter != shader->Attributes.end(); iter++)
		{
			ShaderAttribute* attribute = *iter;

			// Bind this attribute's location
			glBindAttribLocation(technique->shaderProgramId, attribute->Location, attribute->Name.c_str());
		}

		// Link the shader program
		LinkProgram(technique->shaderProgramId);	

		// Setup the uniforms for this technique
		technique->SetupUniforms(&(shader->Parameters));

		// Add the technique to the shader
		shader->Techniques.push_back(technique);
		shader->TechniquesByName.Add(technique->Name.c_str(), technique);
	}

	// Make sure we found at least 1 supported platform
	ASSERT(shader->Techniques.size() > 0, L"No supported techniques were found in this shader");

	// Default the shader to the first technique
	shader->CurrentTechnique = shader->Techniques[0];
	
	// ===============================

	// return the shader
	return shader;
}



// ***********************************************************************
void Shader::LinkProgram(GLuint shaderProgramId)
{
	// Link the program
	glLinkProgram(shaderProgramId);
	
	// Verify the link succeeded!
	GLint linked = 0;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		// Shader compiler failed

		// Get the length of the program link log
		GLint infoLen = 0;
		glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &infoLen);

		// is there a log?
		if(infoLen > 1)
		{
			// Get the log string
			char* infoLog = (char*) GdkAlloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(shaderProgramId, infoLen, NULL, infoLog);

			// ASSERT the failure
			ASSERT(false, L"Shader linking failed:  %hs", infoLog);
			
			GdkFree(infoLog);
		}
		else
		{
			// ASSERT
			ASSERT(false, L"Shader linking Failed:  Generic Failure");
		}
	}
}

// ***********************************************************************
GLuint Shader::CompileGLShaderFromSource(const char *shaderSource, GLenum shaderType)
{
	// Create the shader object
	GLuint shaderId = glCreateShader(shaderType);
	ASSERT(shaderId != 0, L"glCreateShader() failed");

	// Is this not a GL ES platform?
	#ifndef GDK_OPENGL_ES
	{
		// Strip precision qualifiers from the shader code:
		//  1)  precision mediump float;					// Everything from the "Precision" keyword to the ";"
		//  2)  any leftover 'lowp' 'mediump' or 'highp'

		string modifiedSource(shaderSource);

		modifiedSource = StringUtilities::Replace(modifiedSource.c_str(), "precision lowp float;", "");
		modifiedSource = StringUtilities::Replace(modifiedSource.c_str(), "precision mediump float;", "");
		modifiedSource = StringUtilities::Replace(modifiedSource.c_str(), "precision highp float;", "");

		modifiedSource = StringUtilities::Replace(modifiedSource.c_str(), "lowp", "");
		modifiedSource = StringUtilities::Replace(modifiedSource.c_str(), "mediump", "");
		modifiedSource = StringUtilities::Replace(modifiedSource.c_str(), "highp", "");

		// Load the shader source
		const char* newSource = modifiedSource.c_str();
		glShaderSource(shaderId, 1, &newSource, NULL);
	}
	#else
	{
		// This is a GLES platform, use the source directly

		// Load the shader source
		glShaderSource(shaderId, 1, &shaderSource, NULL);
	}
	#endif

	// Compile the shader
	glCompileShader(shaderId);

	// Verify the compile succeeded
	GLint compiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		// Shader compiler failed

		// Get the length of the shader compiler log
		GLint infoLen = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);

		// is there a log?
		if(infoLen > 1)
		{
			// Get the log string
			char* infoLog = (char*) GdkAlloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shaderId, infoLen, NULL, infoLog);

			// ASSERT the failure
			ASSERT(false, L"Shader compilation failed:  %hs \nShader text: %hs", infoLog, shaderSource);
			
			GdkFree(infoLog);
		}
		else
		{
			// ASSERT
			ASSERT(false, L"Shader Compilation Failed: Generic Failure");
		}
	}

	// Save the ID of the new GL shader
	return shaderId;
}
