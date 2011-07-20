/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "ShaderAttribute.h"
#include "ShaderTechnique.h"
#include "SharedUniformValueSet.h"

namespace Gdk
{
	// ============================================================================
	class Shader
	{
	public:

		// Public: Shader Properties
		// ------------------------------
		
		// Parameters/Uniforms exposed by this shader
		SharedUniformValueSet	Parameters;

		// Attributes exposed by this shader
		ShaderAttributeVector	Attributes;
		ShaderAttributeNameMap	AttributesByName;

		// Techniques
		ShaderTechniqueVector	Techniques;
		ShaderTechniqueNameMap	TechniquesByName;

		ShaderTechnique*		CurrentTechnique;

		// Public: Render State Properties
		// ------------------------------

		// Render State
		struct _RenderState
		{
			// ----------------
			struct _ColorMask
			{
				bool Apply;
				bool RedWriteEnabled;
				bool GreenWriteEnabled;
				bool BlueWriteEnabled;
				bool AlphaWriteEnabled;
			} ColorMask;

			// ----------------
			struct _Blending
			{
				bool Apply;
				bool Enabled;
				Color ConstantColor;
				BlendFactor::Enum SourceRGBFactor;
				BlendFactor::Enum DestRGBFactor;
				BlendEquation::Enum RGBBlendEquation;
				BlendFactor::Enum SourceAlphaFactor;
				BlendFactor::Enum DestAlphaFactor;
				BlendEquation::Enum AlphaBlendEquation;
			} Blending;

			// ----------------
			struct _Depth
			{
				bool Apply;
				bool TestEnabled;
				bool WriteEnabled;
				CompareFunction::Enum TestFunction;
				bool BiasEnabled;
				float BiasFactor;
				float BiasUnits;
			} Depth;
 
			// ----------------
			struct _Culling
			{
				bool Apply;
				CullingMode::Enum CullMode;
			} Culling;

		} RenderState;


		// Public Methods
		// ------------------------------
		
		// DTor
		~Shader();

		// Attributes Methods
		ShaderAttribute* GetAttributeByName(const char* name);

		// Technique Methods
		ShaderTechnique* GetTechniqueByName(const char* name);

		// Makes this the active shader  (All shader parameters/uniforms must have been set before calling this method)
		void Apply();

		// Creation
		static Shader* FromStream(Stream* stream);
		static Shader* FromFile(const char *shaderFilePath);

		// Asset Interfaces
		static const AssetType::Enum ASSET_TYPE = AssetType::Shader;
		static Shader* FromAsset(AssetLoadContext* context);
		void ReleaseChildAssets() {}

		// Utility methods for compiling & linking GL shaders
		static GLuint CompileGLShaderFromSource(const char *shaderSource, GLenum shaderType);
		static void LinkProgram(GLuint programId);

	protected:

		// Internal Properties
		// ------------------------------


		// Internal Methods
		// ------------------------------

		Shader();
	};

} // namespace 
