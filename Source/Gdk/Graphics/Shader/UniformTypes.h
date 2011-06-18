
#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ============================================================================
	namespace UniformTypes
	{
		enum Enum
		{
			Float				= GL_FLOAT,
			Float2				= GL_FLOAT_VEC2,
			Float3				= GL_FLOAT_VEC3,
			Float4				= GL_FLOAT_VEC4,
			Int					= GL_INT,
			Int2				= GL_INT_VEC2,
			Int3				= GL_INT_VEC3,
			Int4				= GL_INT_VEC4,
			Bool				= GL_BOOL,
			Bool2				= GL_BOOL_VEC2,
			Bool3				= GL_BOOL_VEC3,
			Bool4				= GL_BOOL_VEC4,
			Matrix2				= GL_FLOAT_MAT2,
			Matrix3				= GL_FLOAT_MAT3,
			Matrix4				= GL_FLOAT_MAT4,
			Sampler2D			= GL_SAMPLER_2D,
			SamplerCube			= GL_SAMPLER_CUBE,
		};

		const char* ToString(UniformTypes::Enum type);

		// Returns the size of the given data type, in bytes
		int SizeOf(UniformTypes::Enum type);
	}

} // namespace 
