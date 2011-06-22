/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "UniformValue.h"

namespace Gdk
{
	
	// ============================================================================
	class ShaderUniform : public UniformValue
	{
	public:

		// Public Methods
		// -----------------------

		// Simple Accessors
		const string& GetName()	{ return this->name; }	
		GLint GetLocation()		{ return this->location; }

		// DTor
		virtual ~ShaderUniform();

	private:

		// Internal Properties
		// -----------------------

		string	name;
		GLint	location;

		// Used to track value/binding changes across Shader::Apply() calls
		UInt32				lastAppliedStamp;
		UniformValue*		lastAppliedUniform;
		int					valueBufferSize;

		// Internal Methods
		// -----------------------

		// Only Shader's can Construct & Apply these
		friend class Shader;
		friend class ShaderTechnique;

		// CTor
		ShaderUniform(const char* name, UniformTypes::Enum uniformType, GLint size, GLint location);

		// Calls glUniformXX to set the actual value of the shader (assumes the shader is already active via glUseProgram)
		void Apply();

		// Function Pointer for the proper SetUniformXXX method
		typedef void (*SetUniformMethod)(void* data, int arraySize, GLint location);
		SetUniformMethod SetUniform;

		// These methods all conform to the "SetUniformMethod" signature, and apply a uniform based on the respective type
		static void SetUniformFloat1(void* data, int arraySize, GLint location);
		static void SetUniformFloat2(void* data, int arraySize, GLint location);
		static void SetUniformFloat3(void* data, int arraySize, GLint location);
		static void SetUniformFloat4(void* data, int arraySize, GLint location);
		static void SetUniformInt1(void* data, int arraySize, GLint location);
		static void SetUniformInt2(void* data, int arraySize, GLint location);
		static void SetUniformInt3(void* data, int arraySize, GLint location);
		static void SetUniformInt4(void* data, int arraySize, GLint location);
		static void SetUniformMatrix2(void* data, int arraySize, GLint location);
		static void SetUniformMatrix3(void* data, int arraySize, GLint location);
		static void SetUniformMatrix4(void* data, int arraySize, GLint location);
	};

	// Container types for ShaderUniform
	typedef vector<ShaderUniform*>			ShaderUniformVector;
	typedef StringHashMap<ShaderUniform*>	ShaderUniformNameMap;


} // namespace 
