/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "UniformValue.h"

using namespace Gdk;

// ***********************************************************************
UniformValue::UniformValue(UniformTypes::Enum uniformType, int arraySize)
{
	// Store the type & size
	this->uniformType = uniformType;
	this->arraySize = arraySize;

	// Calculate the size of the uniform value
	int valueSize = UniformTypes::SizeOf(uniformType);
	valueSize *= arraySize;

	// Allocate a chunk of memory to hold the uniform value
	this->valueBuffer = GdkAlloc(valueSize);
	memset(valueBuffer, 0, valueSize);

	updateStamp = 1;
	binding = NULL;
}

// ***********************************************************************
UniformValue::~UniformValue()
{
	// Free the value buffer
	GdkFree(valueBuffer);
}

// ***********************************************************************
// Utility macro to easily check that the array index & type are valid for the current instance
#define CHECK_SIZE_AND_TYPE(arrayIndex, type) \
	ASSERT(this->uniformType == type, "Incorrect uniform type [Attempted: %s][Actual: %s]", UniformTypes::ToString(type), UniformTypes::ToString(this->uniformType)); \
	ASSERT(arrayIndex >= 0 && arrayIndex < this->arraySize, "Array index out of bounds on uniform value");

// ===================================================================================================
// Get Methods
// ===================================================================================================

// ***********************************************************************
float UniformValue::GetFloat(int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float);
	return ((float*)valueBuffer)[arrayIndex];
}

// ***********************************************************************
void UniformValue::GetFloat2(float* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float2);
	int arrayOffset = arrayIndex * 2;
	output[0] = ((float*)valueBuffer)[arrayOffset];
	output[1] = ((float*)valueBuffer)[arrayOffset+1];
}

// ***********************************************************************
void UniformValue::GetFloat2(Vector2* vector, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float2);
	int arrayOffset = arrayIndex * 2;
	vector->X = ((float*)valueBuffer)[arrayOffset];
	vector->Y = ((float*)valueBuffer)[arrayOffset+1];
}


// ***********************************************************************
void UniformValue::GetFloat3(float* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float3);
	int arrayOffset = arrayIndex * 3;
	output[0] = ((float*)valueBuffer)[arrayOffset];
	output[1] = ((float*)valueBuffer)[arrayOffset+1];
	output[2] = ((float*)valueBuffer)[arrayOffset+2];
}

// ***********************************************************************
void UniformValue::GetFloat3(Vector2* vector, float* value3, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float3);
	int arrayOffset = arrayIndex * 3;
	vector->X = ((float*)valueBuffer)[arrayOffset];
	vector->Y = ((float*)valueBuffer)[arrayOffset+1];
	*value3 = ((float*)valueBuffer)[arrayOffset+2];
}

// ***********************************************************************
void UniformValue::GetFloat3(Vector3* vector, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float3);
	int arrayOffset = arrayIndex * 3;
	vector->X = ((float*)valueBuffer)[arrayOffset];
	vector->Y = ((float*)valueBuffer)[arrayOffset+1];
	vector->Z = ((float*)valueBuffer)[arrayOffset+2];
}


// ***********************************************************************
void UniformValue::GetFloat4(float* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float4);
	int arrayOffset = arrayIndex * 4;
	output[0] = ((float*)valueBuffer)[arrayOffset];
	output[1] = ((float*)valueBuffer)[arrayOffset+1];
	output[2] = ((float*)valueBuffer)[arrayOffset+2];
	output[3] = ((float*)valueBuffer)[arrayOffset+3];
}

// ***********************************************************************
void UniformValue::GetFloat4(Vector3* vector, float* value4, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float4);
	int arrayOffset = arrayIndex * 4;
	vector->X = ((float*)valueBuffer)[arrayOffset];
	vector->Y = ((float*)valueBuffer)[arrayOffset+1];
	vector->Z = ((float*)valueBuffer)[arrayOffset+2];
	*value4 = ((float*)valueBuffer)[arrayOffset+3];
}


// ***********************************************************************
Int32 UniformValue::GetInt(int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int);
	return ((Int32*)valueBuffer)[arrayIndex];
}

// ***********************************************************************
void UniformValue::GetInt2(Int32* output, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int2);
	int arrayOffset = arrayIndex * 2;
	output[0] = ((Int32*)valueBuffer)[arrayOffset];
	output[1] = ((Int32*)valueBuffer)[arrayOffset+1];
}

// ***********************************************************************
void UniformValue::GetInt3(Int32* output, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int3);
	int arrayOffset = arrayIndex * 3;
	output[0] = ((Int32*)valueBuffer)[arrayOffset];
	output[1] = ((Int32*)valueBuffer)[arrayOffset+1];
	output[2] = ((Int32*)valueBuffer)[arrayOffset+2];
}

// ***********************************************************************
void UniformValue::GetInt4(Int32* output, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int4);
	int arrayOffset = arrayIndex * 4;
	output[0] = ((Int32*)valueBuffer)[arrayOffset];
	output[1] = ((Int32*)valueBuffer)[arrayOffset+1];
	output[2] = ((Int32*)valueBuffer)[arrayOffset+2];
	output[3] = ((Int32*)valueBuffer)[arrayOffset+3];
}


// ***********************************************************************
bool UniformValue::GetBool(int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool);
	return ((Int32*)valueBuffer)[arrayIndex] != 0;
}

// ***********************************************************************
void UniformValue::GetBool2(bool* output, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool2);
	int arrayOffset = arrayIndex * 2;
	output[0] = ((Int32*)valueBuffer)[arrayOffset] != 0;
	output[1] = ((Int32*)valueBuffer)[arrayOffset+1] != 0;
}

// ***********************************************************************
void UniformValue::GetBool3(bool* output, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool3);
	int arrayOffset = arrayIndex * 3;
	output[0] = ((Int32*)valueBuffer)[arrayOffset] != 0;
	output[1] = ((Int32*)valueBuffer)[arrayOffset+1] != 0;
	output[2] = ((Int32*)valueBuffer)[arrayOffset+2] != 0;
}

// ***********************************************************************
void UniformValue::GetBool4(bool* output, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool4);
	int arrayOffset = arrayIndex * 4;
	output[0] = ((Int32*)valueBuffer)[arrayOffset] != 0;
	output[1] = ((Int32*)valueBuffer)[arrayOffset+1] != 0;
	output[2] = ((Int32*)valueBuffer)[arrayOffset+2] != 0;
	output[3] = ((Int32*)valueBuffer)[arrayOffset+3] != 0;
}


// ***********************************************************************
void UniformValue::GetMatrix2(float* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix2);
	float* temp = (float*)valueBuffer + arrayIndex * 4;
	memcpy(output, temp, sizeof(float[4]));
}

// ***********************************************************************
void UniformValue::GetMatrix2(Matrix2D* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix2);
	float* temp = (float*)valueBuffer + arrayIndex * 4;
	memcpy(&(output->M11), temp, sizeof(float[2]));
	memcpy(&(output->M21), temp + 2, sizeof(float[2]));
}


// ***********************************************************************
void UniformValue::GetMatrix3(float* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix3);
	float* temp = (float*)valueBuffer + arrayIndex * 9;
	memcpy(output, temp, sizeof(float[9]));
}

// ***********************************************************************
void UniformValue::GetMatrix3(Matrix2D* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix3);
	float* temp = (float*)valueBuffer + arrayIndex * 9;
	memcpy(&(output->M11), temp, sizeof(float[9]));
}

// ***********************************************************************
void UniformValue::GetMatrix3(Matrix3D* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix3);
	float* temp = (float*)valueBuffer + arrayIndex * 9;
	memcpy(&(output->M11), temp, sizeof(float[3]));
	memcpy(&(output->M21), temp+3, sizeof(float[3]));
	memcpy(&(output->M31), temp+6, sizeof(float[3]));
}


// ***********************************************************************
void UniformValue::GetMatrix4(float* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix4);
	float* temp = (float*)valueBuffer + arrayIndex * 16;
	memcpy(output, temp, sizeof(float[16]));
}

// ***********************************************************************
void UniformValue::GetMatrix4(Matrix3D* output, int arrayIndex)	
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix4);
	float* temp = (float*)valueBuffer + arrayIndex * 16;
	memcpy(&(output->M11), temp, sizeof(float[16]));
}

// ***********************************************************************
Int32 UniformValue::GetSampler(int arrayIndex)	
{
	ASSERT(this->uniformType == UniformTypes::Sampler2D || this->uniformType == UniformTypes::SamplerCube, 
		"Incorrect shader uniform value type [Attempted: Sampler***][Actual: %s]", UniformTypes::ToString(this->uniformType));
	ASSERT(arrayIndex >= 0 && arrayIndex < this->arraySize, "Array index out of bounds attempting to get shader uniform value");

	return ((Int32*)valueBuffer)[arrayIndex];
}

// ===================================================================================================
// Set Methods
// ===================================================================================================

// ***********************************************************************
void UniformValue::SetFloat(float value, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float);
	float* buffer = (float*)valueBuffer + arrayIndex;

	// If the value has changed, copy it & update the stamp
	if(buffer[0] != value)
	{
		buffer[0] = value;
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat2(float* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float2);
	float* buffer = (float*)valueBuffer + arrayIndex * 2;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[2])) != 0)
	{
		memcpy(buffer, values, sizeof(float[2]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat2(float x, float y, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float2);
	float* buffer = (float*)valueBuffer + arrayIndex * 2;
	
	// Build a temporary buffer
	float values[2];
	values[0] = x;
	values[1] = y;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[2])) != 0)
	{
		memcpy(buffer, values, sizeof(float[2]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat2(const Vector2& vector, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float2);
	float* buffer = (float*)valueBuffer + arrayIndex * 2;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, &(vector.X), sizeof(float[2])) != 0)
	{
		memcpy(buffer, &(vector.X), sizeof(float[2]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat3(float* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float3);
	float* buffer = (float*)valueBuffer + arrayIndex * 3;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[3])) != 0)
	{
		memcpy(buffer, values, sizeof(float[3]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat3(float x, float y, float z, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float3);
	float* buffer = (float*)valueBuffer + arrayIndex * 3;
	
	float values[3];
	values[0] = x;
	values[1] = y;
	values[2] = z;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[3])) != 0)
	{
		memcpy(buffer, values, sizeof(float[3]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat3(const Vector2& vector, float value3, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float3);
	float* buffer = (float*)valueBuffer + arrayIndex * 3;
	
	float values[3];
	values[0] = vector.X;
	values[1] = vector.Y;
	values[2] = value3;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[3])) != 0)
	{
		memcpy(buffer, values, sizeof(float[3]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat3(const Vector3& vector, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float3);
	float* buffer = (float*)valueBuffer + arrayIndex * 3;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, &(vector.X), sizeof(float[3])) != 0)
	{
		memcpy(buffer, &(vector.X), sizeof(float[3]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat4(float* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float4);
	float* buffer = (float*)valueBuffer + arrayIndex * 4;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[4])) != 0)
	{
		memcpy(buffer, values, sizeof(float[4]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat4(float x, float y, float z, float w, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float4);
	float* buffer = (float*)valueBuffer + arrayIndex * 4;

	float values[4];
	values[0] = x;
	values[1] = y;
	values[2] = z;
	values[3] = w;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[4])) != 0)
	{
		memcpy(buffer, values, sizeof(float[4]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetFloat4(const Vector3& vector, float value4, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Float4);
	float* buffer = (float*)valueBuffer + arrayIndex * 4;

	float values[4];
	values[0] = vector.X;
	values[1] = vector.Y;
	values[2] = vector.Z;
	values[3] = value4;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[4])) != 0)
	{
		memcpy(buffer, values, sizeof(float[4]));
		updateStamp++;
	}
}


// ***********************************************************************
void UniformValue::SetInt(Int32 value, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex;

	// If the value has changed, copy it & update the stamp
	if(buffer[0] != value)
	{
		buffer[0] = value;
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetInt2(Int32* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int2);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex * 2;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(Int32[2])) != 0)
	{
		memcpy(buffer, values, sizeof(Int32[2]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetInt3(Int32* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int3);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex * 3;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(Int32[3])) != 0)
	{
		memcpy(buffer, values, sizeof(Int32[3]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetInt4(Int32* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Int4);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex * 4;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(Int32[4])) != 0)
	{
		memcpy(buffer, values, sizeof(Int32[4]));
		updateStamp++;
	}
}


// ***********************************************************************
void UniformValue::SetBool(bool value, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex;

	Int32 values[1];
	values[0] = value;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(Int32[1])) != 0)
	{
		memcpy(buffer, values, sizeof(Int32[1]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetBool2(bool* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool2);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex * 2;

	Int32 intValues[2];
	intValues[0] = values[0];
	intValues[1] = values[1];

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(Int32[2])) != 0)
	{
		memcpy(buffer, values, sizeof(Int32[2]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetBool3(bool* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool3);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex * 3;

	Int32 intValues[3];
	intValues[0] = values[0];
	intValues[1] = values[1];
	intValues[2] = values[2];

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(Int32[3])) != 0)
	{
		memcpy(buffer, values, sizeof(Int32[3]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetBool4(bool* values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Bool4);
	Int32* buffer = (Int32*)valueBuffer + arrayIndex * 4;

	Int32 intValues[4];
	intValues[0] = values[0];
	intValues[1] = values[1];
	intValues[2] = values[2];
	intValues[3] = values[3];

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(Int32[4])) != 0)
	{
		memcpy(buffer, values, sizeof(Int32[4]));
		updateStamp++;
	}
}


// ***********************************************************************
void UniformValue::SetMatrix2(float *values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix2);
	float* buffer = (float*)valueBuffer + arrayIndex * 4;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[4])) != 0)
	{
		memcpy(buffer, values, sizeof(float[4]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetMatrix2(const Matrix2D& value, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix2);
	float* buffer = (float*)valueBuffer + arrayIndex * 4;

	// If the value has changed, copy it & update the stamp
	if( memcmp(buffer, &(value.M11), sizeof(float[2])) != 0 ||
		memcmp(buffer+2, &(value.M21), sizeof(float[2])) != 0)
	{
		memcpy(buffer, &(value.M11), sizeof(float[2]));
		memcpy(buffer+2, &(value.M21), sizeof(float[2]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetMatrix3(float *values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix3);
	float* buffer = (float*)valueBuffer + arrayIndex * 9;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[9])) != 0)
	{
		memcpy(buffer, values, sizeof(float[9]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetMatrix3(const Matrix2D& value, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix3);
	float* buffer = (float*)valueBuffer + arrayIndex * 9;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, &(value.M11), sizeof(float[9])) != 0)
	{
		memcpy(buffer, &(value.M11), sizeof(float[9]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetMatrix3(const Matrix3D& value, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix3);
	float* buffer = (float*)valueBuffer + arrayIndex * 9;

	// If the value has changed, copy it & update the stamp
	if( memcmp(buffer, &(value.M11), sizeof(float[3])) != 0 ||
		memcmp(buffer+3, &(value.M21), sizeof(float[3])) != 0 ||
		memcmp(buffer+6, &(value.M31), sizeof(float[3])) != 0)
	{
		memcpy(buffer, &(value.M11), sizeof(float[3]));
		memcpy(buffer+3, &(value.M21), sizeof(float[3]));
		memcpy(buffer+6, &(value.M31), sizeof(float[3]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetMatrix4(float *values, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix4);
	float* buffer = (float*)valueBuffer + arrayIndex * 16;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, values, sizeof(float[16])) != 0)
	{
		memcpy(buffer, values, sizeof(float[16]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetMatrix4(const Matrix3D& value, int arrayIndex)
{
	CHECK_SIZE_AND_TYPE(arrayIndex, UniformTypes::Matrix4);
	float* buffer = (float*)valueBuffer + arrayIndex * 16;

	// If the value has changed, copy it & update the stamp
	if(memcmp(buffer, &(value.M11), sizeof(float[16])) != 0)
	{
		memcpy(buffer, &(value.M11), sizeof(float[16]));
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::SetSampler(Int32 textureUnitIndex, int arrayIndex)
{
	ASSERT(this->uniformType == UniformTypes::Sampler2D || this->uniformType == UniformTypes::SamplerCube, 
		"Incorrect shader uniform value type [Attempted: Sampler***][Actual: %s]", UniformTypes::ToString(this->uniformType));
	ASSERT(arrayIndex >= 0 && arrayIndex < this->arraySize, "Array index out of bounds attempting to get shader uniform value");

	Int32* buffer = (Int32*)valueBuffer + arrayIndex;

	// If the value has changed, copy it & update the stamp
	if(buffer[0] != textureUnitIndex)
	{
		buffer[0] = textureUnitIndex;
		updateStamp++;
	}
}

// ***********************************************************************
void UniformValue::ParseValue(const char* str)
{
	// Handle the parse based on the type
	switch(this->uniformType)
	{
		case UniformTypes::Float:
			GDK_SSCANF(str, "%f", (float*)valueBuffer);
			break;

		case UniformTypes::Float2:
			GDK_SSCANF(str, "%f,%f", (float*)valueBuffer, (float*)valueBuffer + 1);
			break;

		case UniformTypes::Float3:
			GDK_SSCANF(str, "%f,%f,%f", (float*)valueBuffer, (float*)valueBuffer + 1, (float*)valueBuffer + 2);
			break;

		case UniformTypes::Float4:
			((float*)valueBuffer)[3] = 1.0f;	// In case only 3 elements are specified, default the 4th to 1.0f  (full alpha on color values..)
			GDK_SSCANF(str, "%f,%f,%f,%f", (float*)valueBuffer, (float*)valueBuffer + 1, (float*)valueBuffer + 2, (float*)valueBuffer + 3);
			break;

		case UniformTypes::Int:
		case UniformTypes::Bool:
		case UniformTypes::Sampler2D:
		case UniformTypes::SamplerCube:
			GDK_SSCANF(str, "%d", (Int32*)valueBuffer);
			break;

		case UniformTypes::Int2:
		case UniformTypes::Bool2:
			GDK_SSCANF(str, "%d,%d", (Int32*)valueBuffer, (Int32*)valueBuffer + 1);
			break;

		case UniformTypes::Int3:
		case UniformTypes::Bool3:
			GDK_SSCANF(str, "%d,%d,%d", (Int32*)valueBuffer, (Int32*)valueBuffer + 1, (Int32*)valueBuffer + 2);
			break;

		case UniformTypes::Int4:
		case UniformTypes::Bool4:
			GDK_SSCANF(str, "%d,%d,%d,%d", (Int32*)valueBuffer, (Int32*)valueBuffer + 1, (Int32*)valueBuffer + 2, (Int32*)valueBuffer + 3);
			break;

		case UniformTypes::Matrix2:
			GDK_SSCANF(str, "%f,%f,%f,%f", 
				(float*)valueBuffer, (float*)valueBuffer + 1, 
				(float*)valueBuffer + 2, (float*)valueBuffer + 3
				);
			break;

		case UniformTypes::Matrix3:
			GDK_SSCANF(str, "%f,%f,%f,%f,%f,%f,%f,%f,%f", 
				(float*)valueBuffer,     (float*)valueBuffer + 1, (float*)valueBuffer + 2, 
				(float*)valueBuffer + 3, (float*)valueBuffer + 4, (float*)valueBuffer + 5,
				(float*)valueBuffer + 6, (float*)valueBuffer + 7, (float*)valueBuffer + 8
				);
			break;

		case UniformTypes::Matrix4:
			GDK_SSCANF(str, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
				(float*)valueBuffer,     (float*)valueBuffer + 1, (float*)valueBuffer + 2, (float*)valueBuffer + 3,
				(float*)valueBuffer + 4, (float*)valueBuffer + 5, (float*)valueBuffer + 6, (float*)valueBuffer + 7,
				(float*)valueBuffer + 8, (float*)valueBuffer + 9, (float*)valueBuffer + 10, (float*)valueBuffer + 11,
				(float*)valueBuffer + 12, (float*)valueBuffer + 13, (float*)valueBuffer + 14, (float*)valueBuffer + 15
				);
			break;
	}
	
	updateStamp++;
}

// ===================================================================================================
// Binding Methods
// ===================================================================================================

// ***********************************************************************
void UniformValue::Bind(UniformValue* uniformValue)
{
	// If the caller passed in NULL, then we UnBind()
	if(uniformValue == NULL)
	{
		// UnBind!
		this->UnBind();
		return;
	}

	// Verify the type & size match
	ASSERT(uniformValue->uniformType == this->uniformType, "Attempt to bind to shared uniform value with mismatched types. [Binding: %s] [This: %s]",
		UniformTypes::ToString(uniformValue->uniformType), 
		UniformTypes::ToString(this->uniformType)
		);
	ASSERT(uniformValue->arraySize == this->arraySize, "Attempt to bind to shared uniform value with mismatched array size. [Binding: %d] [This: %d]",
		uniformValue->arraySize, 
		this->arraySize
		);

	// Store the binding
	this->binding = uniformValue;
	updateStamp++;
}

// ***********************************************************************
void UniformValue::UnBind()
{
	// Sever the binding
	this->binding = NULL;
	updateStamp++;
}
