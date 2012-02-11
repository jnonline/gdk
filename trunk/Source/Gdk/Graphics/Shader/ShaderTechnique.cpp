/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ShaderTechnique.h"

using namespace Gdk;

// ***********************************************************************
ShaderTechnique::ShaderTechnique()
{
	this->vertexShaderId = GL_INVALID_VALUE;
	this->fragmentShaderId = GL_INVALID_VALUE;
	this->shaderProgramId = GL_INVALID_VALUE;
}

// ***********************************************************************
ShaderTechnique::~ShaderTechnique()
{
	// Clean up the uniforms
	for(ShaderUniformVector::iterator iter = Uniforms.begin(); iter != Uniforms.end(); iter++)
	{
		ShaderUniform* uniform = *iter;
		GdkDelete(uniform);
	}
	Uniforms.clear();
	UniformsByName.Clear();

	// Delete the shaders & the shader program
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteProgram(shaderProgramId);
}

// ***********************************************************************
ShaderUniform* ShaderTechnique::GetUniformByName(const char* name)
{
	// Get the uniform with this name
	ShaderUniformNameMap::Iterator iter = UniformsByName.Find(name);
	if(iter == UniformsByName.End())
		return NULL;

	return iter->second;
}

// ***********************************************************************
void ShaderTechnique::SetupUniforms(SharedUniformValueSet* shaderParameters)
{
	// Setup the Uniforms
	// -------------------------

	// Get the # of uniforms
	GLint numUniforms;
	glGetProgramiv(shaderProgramId, GL_ACTIVE_UNIFORMS, &numUniforms);

	// Are there any uniforms?
	if(numUniforms > 0)
	{
		// Get the max size of the uniform name string
		GLint maxUniformNameLength;
		glGetProgramiv(shaderProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

		// Allocate a buffer to store the uniform name
		char* uniformName = (char*) GdkAlloc(sizeof(char) * maxUniformNameLength);

		// Loop through the uniforms
		for(int uniformIndex = 0; uniformIndex < numUniforms; uniformIndex++)
		{
			GLint size;
			GLenum type;

			// Get the information for this uniform
			glGetActiveUniform(shaderProgramId, uniformIndex, maxUniformNameLength, NULL, &size, &type, uniformName);
			GLint location = glGetUniformLocation(shaderProgramId, uniformName);
            
            // Is this uniform an array property and the name has an array index on the end?
            int uniformNameLen = strlen(uniformName);
            if (size > 1 && uniformNameLen > 3 
                && uniformName[uniformNameLen-3] == '['
                && uniformName[uniformNameLen-2] == '0'
                && uniformName[uniformNameLen-1] == ']'
                )
            {
				// cut off the element indexer portion of the string
                uniformName[uniformNameLen-3] = 0;
            }

			// Create a ShaderUniform to hold this uniform info
			ShaderUniform* uniform = GdkNew ShaderUniform(uniformName, (UniformTypes::Enum)type, size, location);

			// Add the uniform to the vector & map
			Uniforms.push_back(uniform);
			UniformsByName.Add(uniformName, uniform);

			// Bind this uniform to the corresponding shader parameter
			UniformValue* shaderParameter = shaderParameters->Get(uniformName);
			ASSERT(shaderParameter != NULL, "Uniform [%s] has no matching Shader Parameter", uniformName);
			uniform->Bind(shaderParameter);
		}

		// Release the name buffer
		GdkFree(uniformName);
	}
}