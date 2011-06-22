/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ============================================================================
	namespace ShaderAttributeType
	{
		enum Enum
		{
			Float				= GL_FLOAT,
			Float2				= GL_FLOAT_VEC2,
			Float3				= GL_FLOAT_VEC3,
			Float4				= GL_FLOAT_VEC4,
			Matrix2				= GL_FLOAT_MAT2,
			Matrix3				= GL_FLOAT_MAT3,
			Matrix4				= GL_FLOAT_MAT4,
		};
	}

	// ============================================================================
	class ShaderAttribute
	{
	public:
		// Public Properties
		// -----------------------

		string						Name;
		ShaderAttributeType::Enum	Type;
		GLint						Size;
		GLint						Location;

		// CTor
		ShaderAttribute(const char* name, ShaderAttributeType::Enum type, GLint size, GLint location);
	};

	// Container types for ShaderAttribute
	typedef vector<ShaderAttribute*>			ShaderAttributeVector;
	typedef StringHashMap<ShaderAttribute*>		ShaderAttributeNameMap;

} // namespace 
