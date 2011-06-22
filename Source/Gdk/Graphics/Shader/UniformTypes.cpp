/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "UniformTypes.h"
#include "../../System/StringUtilities.h"

using namespace Gdk;

// ******************************************************************************************
const char* UniformTypes::ToString(UniformTypes::Enum type)
{
	static const char* sFloat = "Float";
	static const char* sFloat2 = "Float2";
	static const char* sFloat3 = "Float3";
	static const char* sFloat4 = "Float4";
	static const char* sInt = "Int";
	static const char* sInt2 = "Int2";
	static const char* sInt3 = "Int3";
	static const char* sInt4 = "Int4";
	static const char* sBool = "Bool";
	static const char* sBool2 = "Bool2";
	static const char* sBool3 = "Bool3";
	static const char* sBool4 = "Bool4";
	static const char* sMatrix2 = "Matrix2";
	static const char* sMatrix3 = "Matrix3";
	static const char* sMatrix4 = "Matrix4";
	static const char* sSampler2D = "Sampler2D";
	static const char* sSamplerCube = "SamplerCube";
	
	switch(type)
	{
		case UniformTypes::Float:			return sFloat;
		case UniformTypes::Float2:			return sFloat2;
		case UniformTypes::Float3:			return sFloat3;
		case UniformTypes::Float4:			return sFloat4;
		case UniformTypes::Int:			return sInt;
		case UniformTypes::Int2:			return sInt2;
		case UniformTypes::Int3:			return sInt3;
		case UniformTypes::Int4:			return sInt4;
		case UniformTypes::Bool:			return sBool;
		case UniformTypes::Bool2:			return sBool2;
		case UniformTypes::Bool3:			return sBool3;
		case UniformTypes::Bool4:			return sBool4;
		case UniformTypes::Matrix2:		return sMatrix2;
		case UniformTypes::Matrix3:		return sMatrix3;
		case UniformTypes::Matrix4:		return sMatrix4;
		case UniformTypes::Sampler2D:		return sSampler2D;
		case UniformTypes::SamplerCube:	return sSamplerCube;
	}
	return StringUtilities::Empty;
}

// ******************************************************************************************
int UniformTypes::SizeOf(UniformTypes::Enum type)
{
	switch(type)
	{
		case UniformTypes::Float:		return sizeof(float);
		case UniformTypes::Float2:		return sizeof(float[2]);
		case UniformTypes::Float3:		return sizeof(float[3]);
		case UniformTypes::Float4:		return sizeof(float[4]);

		case UniformTypes::Int:			return sizeof(Int32);
		case UniformTypes::Int2:		return sizeof(Int32[2]);
		case UniformTypes::Int3:		return sizeof(Int32[3]);
		case UniformTypes::Int4:		return sizeof(Int32[4]);

		case UniformTypes::Bool:		return sizeof(Int32);
		case UniformTypes::Bool2:		return sizeof(Int32[2]);
		case UniformTypes::Bool3:		return sizeof(Int32[3]);
		case UniformTypes::Bool4:		return sizeof(Int32[4]);

		case UniformTypes::Matrix2:		return sizeof(float[2][2]);
		case UniformTypes::Matrix3:		return sizeof(float[3][3]);
		case UniformTypes::Matrix4:		return sizeof(float[4][4]);

		case UniformTypes::Sampler2D:	return sizeof(Int32);
		case UniformTypes::SamplerCube:	return sizeof(Int32);
	}

	return 0;
}
