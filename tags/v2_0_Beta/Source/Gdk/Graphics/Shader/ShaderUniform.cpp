/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ShaderUniform.h"

using namespace Gdk;

// ***********************************************************************
ShaderUniform::ShaderUniform(const char* name, UniformTypes::Enum uniformType, GLint size, GLint location)
	: UniformValue(uniformType, size), name(name), location(location)
{
	// Determine the proper SetUniformXXX method depending on the uniform type
	switch(uniformType)
	{
		case UniformTypes::Float:		SetUniform = &SetUniformFloat1;		break;
		case UniformTypes::Float2:		SetUniform = &SetUniformFloat2;		break;
		case UniformTypes::Float3:		SetUniform = &SetUniformFloat3;		break;
		case UniformTypes::Float4:		SetUniform = &SetUniformFloat4;		break;

		case UniformTypes::Int:			SetUniform = &SetUniformInt1;		break;
		case UniformTypes::Int2:		SetUniform = &SetUniformInt2;		break;
		case UniformTypes::Int3:		SetUniform = &SetUniformInt3;		break;
		case UniformTypes::Int4:		SetUniform = &SetUniformInt4;		break;

		case UniformTypes::Bool:		SetUniform = &SetUniformInt1;		break;
		case UniformTypes::Bool2:		SetUniform = &SetUniformInt2;		break;
		case UniformTypes::Bool3:		SetUniform = &SetUniformInt3;		break;
		case UniformTypes::Bool4:		SetUniform = &SetUniformInt4;		break;

		case UniformTypes::Matrix2:		SetUniform = &SetUniformMatrix2;	break;
		case UniformTypes::Matrix3:		SetUniform = &SetUniformMatrix3;	break;
		case UniformTypes::Matrix4:		SetUniform = &SetUniformMatrix4;	break;
		
		case UniformTypes::Sampler2D:	
		case UniformTypes::SamplerCube:
			SetUniform = &SetUniformInt1;		
			break;
	}

	// Pre store the size of the value buffer
	valueBufferSize = this->GetValueBufferSize();

	// Setup tracking
	this->lastAppliedUniform = NULL;
	this->lastAppliedStamp = 0;
}

// ***********************************************************************
ShaderUniform::~ShaderUniform()
{
}

// ***********************************************************************
void ShaderUniform::Apply()
{
	// Do we have a binding?
	if(binding != NULL)
	{
		// Walk up the binding list
		UniformValue* topBinding = binding;
		while(topBinding->binding != NULL)
			topBinding = topBinding->binding;

		// Have we already applied this binding at this update stamp?
		if(this->lastAppliedUniform == topBinding && this->lastAppliedStamp == topBinding->updateStamp)
			return;

		// Track this binding as the last one we applied
		this->lastAppliedUniform = topBinding;
		this->lastAppliedStamp = topBinding->updateStamp;

		// Copy the binding value to our local value buffer
		memcpy(this->valueBuffer, topBinding->valueBuffer, valueBufferSize);
	}
	else // No Binding
	{
		// Were we the last applied uniform & our value is unchanged?
		if(this->lastAppliedUniform == this && this->lastAppliedStamp == this->updateStamp)
			return;

		// Track this change
		this->lastAppliedUniform = this;
		this->lastAppliedStamp = this->updateStamp;
	}

	// If we made it this far, than our local value needs to be applied to the shader :D

	// Set the actual shader uniform
	SetUniform(this->valueBuffer, this->arraySize, this->location);
}

// ***********************************************************************
void ShaderUniform::SetUniformFloat1(void* data, int arraySize, GLint location)
{
	glUniform1fv(location, arraySize, (GLfloat*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformFloat2(void* data, int arraySize, GLint location)
{
	glUniform2fv(location, arraySize, (GLfloat*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformFloat3(void* data, int arraySize, GLint location)
{
	glUniform3fv(location, arraySize, (GLfloat*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformFloat4(void* data, int arraySize, GLint location)
{
	glUniform4fv(location, arraySize, (GLfloat*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformInt1(void* data, int arraySize, GLint location)
{
	glUniform1iv(location, arraySize, (GLint*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformInt2(void* data, int arraySize, GLint location)
{
	glUniform2iv(location, arraySize, (GLint*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformInt3(void* data, int arraySize, GLint location)
{
	glUniform3iv(location, arraySize, (GLint*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformInt4(void* data, int arraySize, GLint location)
{
	glUniform4iv(location, arraySize, (GLint*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformMatrix2(void* data, int arraySize, GLint location)
{
	glUniformMatrix2fv(location, arraySize, GL_FALSE, (GLfloat*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformMatrix3(void* data, int arraySize, GLint location)
{
	glUniformMatrix3fv(location, arraySize, GL_FALSE, (GLfloat*)data);
}

// ***********************************************************************
void ShaderUniform::SetUniformMatrix4(void* data, int arraySize, GLint location)
{
	glUniformMatrix4fv(location, arraySize, GL_FALSE, (GLfloat*)data);
}
