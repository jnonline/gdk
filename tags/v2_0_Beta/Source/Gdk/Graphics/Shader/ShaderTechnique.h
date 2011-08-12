/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "ShaderUniform.h"
#include "ShaderAttribute.h"

namespace Gdk
{
	// ============================================================================
	class ShaderTechnique
	{
	public:

		// Public Properties
		// ------------------------------
		
		string Name;
		ShaderProfile::Enum		Profile;

		// Uniforms used by this technique
		ShaderUniformVector		Uniforms;
		ShaderUniformNameMap	UniformsByName;		

		// Public Methods
		// ------------------------------
		
		// DTor
		~ShaderTechnique(); 

		// Uniforms Methods
		ShaderUniform* GetUniformByName(const char* name);

	protected:

		// Internal Properties
		// ------------------------------

		GLuint vertexShaderId;
		GLuint fragmentShaderId;
		GLuint shaderProgramId;

		// Internal Methods
		// ------------------------------

		// CTor
		friend class Shader;
		ShaderTechnique();

		// Utility methods
		void SetupUniforms(class SharedUniformValueSet* shaderParameters);
	};

	// Container types for ShaderTechnique
	typedef vector<ShaderTechnique*>			ShaderTechniqueVector;
	typedef StringHashMap<ShaderTechnique*>		ShaderTechniqueNameMap;

} // namespace 
