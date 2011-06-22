/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "UniformTypes.h"
#include "../../System/Containers/StringHashMap.h"
#include "../../Math/2D/Matrix2D.h"
#include "../../Math/3D/Matrix3D.h"

namespace Gdk
{
	// ============================================================================
	class UniformValue
	{
	public:
		// Public Properties
		// -----------------------

		// Public Methods
		// -----------------------

		// Simple Accessors
		UniformTypes::Enum GetUniformType()	{ return this->uniformType; }
		int GetArraySize()					{ return this->arraySize; }

		int GetValueBufferSize()			{ return UniformTypes::SizeOf(this->uniformType) * this->arraySize; }
		const void* GetValueBuffer()		{ return (const void*) this->valueBuffer; }

		// Get
		// --------
		float GetFloat(int arrayIndex = 0);
		void GetFloat2(float* output, int arrayIndex = 0);
		void GetFloat2(Vector2* vector, int arrayIndex = 0);
		void GetFloat3(float* output, int arrayIndex = 0);
		void GetFloat3(Vector3* vector, int arrayIndex = 0);
		void GetFloat3(Vector2* vector, float* value3, int arrayIndex = 0);
		void GetFloat4(float* output, int arrayIndex = 0);
		void GetFloat4(Vector3* vector, float* value4, int arrayIndex = 0);

		Int32 GetInt(int arrayIndex = 0);
		void GetInt2(Int32* output, int arrayIndex = 0);
		void GetInt3(Int32* output, int arrayIndex = 0);
		void GetInt4(Int32* output, int arrayIndex = 0);

		bool GetBool(int arrayIndex = 0);
		void GetBool2(bool* output, int arrayIndex = 0);
		void GetBool3(bool* output, int arrayIndex = 0);
		void GetBool4(bool* output, int arrayIndex = 0);

		void GetMatrix2(float* output, int arrayIndex = 0);			// 2x2 = 4 floats
		void GetMatrix2(Matrix2D* output, int arrayIndex = 0);

		void GetMatrix3(float* output, int arrayIndex = 0);			// 3x3 = 9 floats
		void GetMatrix3(Matrix2D* output, int arrayIndex = 0);
		void GetMatrix3(Matrix3D* output, int arrayIndex = 0);

		void GetMatrix4(float* output, int arrayIndex = 0);			// 4x4 = 16 floats
		void GetMatrix4(Matrix3D* output, int arrayIndex = 0);

		Int32 GetSampler(int arrayIndex = 0);

		// Set
		// --------
		void SetFloat(float value, int arrayIndex = 0);
		void SetFloat2(float* values, int arrayIndex = 0);
		void SetFloat2(float x, float y, int arrayIndex = 0);
		void SetFloat2(const Vector2& vector, int arrayIndex = 0);
		void SetFloat3(float* values, int arrayIndex = 0);
		void SetFloat3(float x, float y, float z, int arrayIndex = 0);
		void SetFloat3(const Vector2& vector, float value3, int arrayIndex = 0);
		void SetFloat3(const Vector3& vector, int arrayIndex = 0);
		void SetFloat4(float* values, int arrayIndex = 0);
		void SetFloat4(float x, float y, float z, float w, int arrayIndex = 0);
		void SetFloat4(const Vector3& vector, float value4, int arrayIndex = 0);

		void SetInt(Int32 value, int arrayIndex = 0);
		void SetInt2(Int32* values, int arrayIndex = 0);
		void SetInt3(Int32* values, int arrayIndex = 0);
		void SetInt4(Int32* values, int arrayIndex = 0);

		void SetBool(bool value, int arrayIndex = 0);
		void SetBool2(bool* values, int arrayIndex = 0);
		void SetBool3(bool* values, int arrayIndex = 0);
		void SetBool4(bool* values, int arrayIndex = 0);

		void SetMatrix2(float* values, int arrayIndex = 0);
		void SetMatrix2(const Matrix2D& value, int arrayIndex = 0);

		void SetMatrix3(float* values, int arrayIndex = 0);
		void SetMatrix3(const Matrix2D& value, int arrayIndex = 0);
		void SetMatrix3(const Matrix3D& value, int arrayIndex = 0);

		void SetMatrix4(float* values, int arrayIndex = 0);
		void SetMatrix4(const Matrix3D& value, int arrayIndex = 0);

		void SetSampler(Int32 textureUnitIndex, int arrayIndex = 0);

		// Parses the given string and stores it's value in the parameter
		void ParseValue(const char* str);

		// Binding
		// ---------------
		friend class Shader;
		friend class ShaderUniform;

		void Bind(UniformValue* uniformValue);
		void UnBind();
		UniformValue* GetBinding()	{ return this->binding; }

		// DTor
		virtual ~UniformValue();

	protected:

		// Internal Properties
		// -----------------------

		UniformTypes::Enum	uniformType;
		int					arraySize;
		void*				valueBuffer;
		
		// Stamping (used to detect changes)
		UInt32				updateStamp;
		
		// Bindings
		UniformValue*		binding;

		// Internal Methods
		// -----------------------

		// CTor
		friend class SharedUniformValueSet;
		UniformValue(UniformTypes::Enum uniformType, int arraySize);
	};

	// Container types for UniformValue
	typedef vector<UniformValue*>			UniformValueVector;
	typedef StringHashMap<UniformValue*>	UniformValueNameMap;
	

} // namespace 
