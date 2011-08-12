/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Renderer2D.h"
#include "../../Assets/SharedAssets.h"

using namespace Gdk;


// ********************************************************************************
const char* Renderer2DPrimitiveType::ToString(Renderer2DPrimitiveType::Enum primitiveType)
{
	static const char* points = "Points";
	static const char* lines = "Lines";
	static const char* triangles = "Triangles";
	static const char* quads = "Quads";

	switch(primitiveType)
	{
		case Renderer2DPrimitiveType::Points:		return points;
		case Renderer2DPrimitiveType::Lines:		return lines;
		case Renderer2DPrimitiveType::Triangles:	return triangles;
		case Renderer2DPrimitiveType::Quads:		return quads;
	};
	
	return StringUtilities::Unknown;
}

// ********************************************************************************
const char* Renderer2DVertexType::ToString(Renderer2DVertexType::Enum vertexType)
{
	static const char* p2c4 = "P2C4";
	static const char* p2t2c4 = "P2T2C4";

	switch(vertexType)
	{
		case Renderer2DVertexType::P2C4:		return p2c4;
		case Renderer2DVertexType::P2T2C4:		return p2t2c4;
	};
	
	return StringUtilities::Unknown;
}

// ********************************************************************************

// Static Instantiations
void* Renderer2D::batchBuffer = NULL;
void* Renderer2D::endOfTheBuffer = NULL;
UInt32 Renderer2D::bufferSize = 0;
Renderer2DBatchSet Renderer2D::batches;

Shader* Renderer2D::texturedShader = NULL;
Shader* Renderer2D::nonTexturedShader = NULL;
Matrix2D Renderer2D::worldTransform = Matrix2D::IDENTITY;
bool Renderer2D::applyWorldTransform = false;

// Public Statics
Renderer2DSortMode::Enum Renderer2D::SortMode = Renderer2DSortMode::ZOrder;


// ********************************************************************************
void Renderer2D::Init()
{
	// Setup internal buffers
	// ----------------------------------------

	// Allocate the batch buffer
	bufferSize = 512000;
	batchBuffer = GdkAlloc(bufferSize);
	endOfTheBuffer = batchBuffer;

	// Set the default shaders
	SetTexturedShader(NULL);
	SetNonTexturedShader(NULL);

	// Setup the default world transform
	SetWorldTransform(Matrix2D::IDENTITY);

	// Reserve the batch index vector
	batches.reserve(200);
}	

// ********************************************************************************
void Renderer2D::Shutdown()
{
	// Free the batch buffer
	if(batchBuffer != NULL)
		GdkFree(batchBuffer);
	batchBuffer = NULL;
	
	// Null & clear other properties
	endOfTheBuffer = NULL;
	batches.clear();
}

// ********************************************************************************
void Renderer2D::SetTexturedShader(Shader* shader)
{
	// If the user passed in NULL, we default to the global shared shader
	if(shader == NULL)
		texturedShader = SharedAssets::Shaders.Renderer2D.Textured;
	else
		texturedShader = shader;
}

// ********************************************************************************
void Renderer2D::SetNonTexturedShader(Shader* shader)
{
	// If the user passed in NULL, we default to the global shared shader
	if(shader == NULL)
		nonTexturedShader = SharedAssets::Shaders.Renderer2D.NonTextured;
	else
		nonTexturedShader = shader;
}

// ********************************************************************************
void Renderer2D::SetWorldTransform(const Matrix2D& world)
{
	worldTransform = world;
	applyWorldTransform = worldTransform.IsIdentity() == false;
}

// ********************************************************************************
bool CompareBatchesByRenderState(Renderer2DBatchHeader* a, Renderer2DBatchHeader* b) 
{ 
	// Compare Shader
	if( a->ShaderPtr < b->ShaderPtr )
		return true;
	else if(a->ShaderPtr > b->ShaderPtr)
		return false;

	// Compare Texture 1
	if( a->TextureID1 < b->TextureID1 )
		return true;
	else if(a->TextureID1 > b->TextureID1)
		return false;

	// Compare Texture 2
	if( a->TextureID2 < b->TextureID2 )
		return true;
	else if(a->TextureID2 > b->TextureID2)
		return false;

	// Compare BlendMode
	if( a->BlendMode < b->BlendMode )
		return true;
	else if(a->BlendMode > b->BlendMode)
		return false;
	
	// Compare CallOrder
	if( a->CallOrder < b->CallOrder )
		return true;

	return false;
}

// ********************************************************************************
bool CompareBatchesByZOrder(Renderer2DBatchHeader* a, Renderer2DBatchHeader* b) 
{ 
	// Compare Z
	if( a->Z < b->Z )
		return true;
	else if(a->Z > b->Z)
		return false;
	
	// Compare CallOrder
	if( a->CallOrder < b->CallOrder )
		return true;

	return false;

}
// ********************************************************************************
void Renderer2D::Flush()
{
	// Sort the batches
	// ----------------------

	switch(SortMode)
	{
		case Renderer2DSortMode::RenderState:
			sort(batches.begin(), batches.end(), CompareBatchesByRenderState);
			break;

		case Renderer2DSortMode::ZOrder:
			sort(batches.begin(), batches.end(), CompareBatchesByZOrder);
			break;

		default:
			// Unsorted
			break;
	}

	// Render the batches
	// ----------------------

	// Unbind any vertex & index buffers
	Graphics::BindVertexBuffer(0);
	Graphics::BindIndexBuffer(0);

	// Enable blending
	Graphics::EnableBlending(true);

	// Disable depth write/testing & culling
	Graphics::EnableDepthTest(false);
	Graphics::EnableDepthWrite(false);
	Graphics::SetCullingMode(CullingMode::Disabled);

	// All vertices have at least 2 vertex attribute arrays
	Graphics::EnableVertexAttribArray(0, true);
	Graphics::EnableVertexAttribArray(1, true);
	//Graphics::EnableVertexAttribArray(2, false);   // not necessary (this is done below)
	
	// Disable the rest of the attribute arrays
	for(int attributeIndex=3; attributeIndex<Graphics::MAX_VERTEX_ATTRIBUTES; attributeIndex++)
		Graphics::EnableVertexAttribArray(attributeIndex, false);

	// Loop through the batches
	for(Renderer2DBatchSetIterator batchIter = batches.begin(); batchIter != batches.end(); batchIter++)
	{
		// Get this batch
		Renderer2DBatchHeader* batch = *batchIter;

		// Get a pointer to the vertex data at the end of the batch
		void* vertexData = &(batch[1]);

		// Bind textures (if necessary)
		if(batch->TextureID1 != 0)
			Graphics::BindTexture(batch->TextureID1, TextureUnit::Unit_0);
		if(batch->TextureID2 != 0)
			Graphics::BindTexture(batch->TextureID2, TextureUnit::Unit_1);

		// Apply the shader
		batch->ShaderPtr->Apply();

		// Setup the VertexType
		if(batch->VertexType == Renderer2DVertexType::P2C4)
		{
			// Turn off 3rd attribute array
			Graphics::EnableVertexAttribArray(2, false);

			// Set the vertex data pointers
			GLuint stride = sizeof(VertexP2C4);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, vertexData);
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)((long)vertexData + sizeof(GLfloat[2])) );
		}
		else
		{
			// Turn on 3rd attribute array
			Graphics::EnableVertexAttribArray(2, true);

			// Set the vertex data pointers
			GLuint stride = sizeof(VertexP2T2C4);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, vertexData);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)((long)vertexData + sizeof(GLfloat[2])) );
			glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)((long)vertexData + sizeof(GLfloat[4])) );
		}			

		// Setup blending
		Graphics::SetBlendMode(batch->BlendMode);

		// Do the drawing based on the PrimitiveType
		switch(batch->PrimitiveType)
		{
			case Renderer2DPrimitiveType::Quads:
				// Use the index buffer to draw the quads
				glDrawElements(GL_TRIANGLES, batch->NumPrimitives * 6, GL_UNSIGNED_SHORT, SharedAssets::QuadIndexBuffer);
				break;

			case Renderer2DPrimitiveType::Triangles:
				// Draw the triangles
				glDrawArrays(GL_TRIANGLES, 0, batch->NumPrimitives * 3);
				break;

			case Renderer2DPrimitiveType::Lines:
				// Draw the triangles
				glDrawArrays(GL_LINES, 0, batch->NumPrimitives * 2);
				break;

			case Renderer2DPrimitiveType::Points:
				// Draw the triangles
				glDrawArrays(GL_POINTS, 0, batch->NumPrimitives);
				break;
		}
	}

	Graphics::EnableDepthWrite(true);

	// Clear the batches
	endOfTheBuffer = batchBuffer;
	batches.clear();
}

// ********************************************************************************
void Renderer2D::LogBatches()
{
	LOG_INFO(L"Renderer2D::LogBatches() - Logging batch info");
	LOG_INFO(L"-------------------------------------------------");

	if(batches.size() == 0)
	{
		LOG_INFO(L"No batches");
	}

	// Loop through the batches
	for(size_t index = 0; index < batches.size(); index++)
	{
		Renderer2DBatchHeader* batch = batches[index];

		// Log the batch info header row
		if(index % 20 == 0)
		{
			LOG_INFO(L" Z Depth    | CallOrder | PrimitiveType | VertexType | Texture1 | Texture2 | BlendMode | #Primitives | Batch Size | Expected Size");
		}

		// Get the vertex size & total vertices data size for the given primitive type
		int vertexSize = sizeof(VertexP2C4);
		if(batch->VertexType == Renderer2DVertexType::P2T2C4)
			vertexSize = sizeof(VertexP2T2C4);
		int totalVerticesSize = batch->NumPrimitives * (int)(batch->PrimitiveType) * vertexSize;

		// Calculate the actual size of this batch
		int actualBatchSize = 0;
		if(index == batches.size() - 1)
			actualBatchSize = (unsigned int)endOfTheBuffer - (unsigned int)batch;
		else
			actualBatchSize = (unsigned int)batches[index+1] - (unsigned int)batch;

		// Calculate the expected batch size
		int expectedBatchSize = sizeof(Renderer2DBatchHeader) + totalVerticesSize;

		// Log the batch info
		LOG_INFO(L"%5.5f | %9d | %13hs | %10hs | %7d | %7d | %9hs | %10d | %10d | %13d",
			batch->Z, 
			batch->CallOrder, 
			Renderer2DPrimitiveType::ToString((Renderer2DPrimitiveType::Enum)batch->PrimitiveType),
			Renderer2DVertexType::ToString((Renderer2DVertexType::Enum)batch->VertexType),
			batch->TextureID1,
			batch->TextureID2,
			BlendMode::ToString(batch->BlendMode),
			batch->NumPrimitives,
			actualBatchSize, 
			expectedBatchSize
			);
		
		// Check for matching sizes
		ASSERT(actualBatchSize == expectedBatchSize, L"The actual & expected batch sizes do NOT match!!  (Actual: %5d)(Expected: %5d)", actualBatchSize, expectedBatchSize);
	}

	LOG_INFO(L"-------------------------------------------------");
}

// ********************************************************************************
Renderer2DBatchHeader* Renderer2D::GetBatch(
		UInt32 requestedBatchSize,
        UInt32 numPrimitives,
		UInt8 primitiveType, 
		UInt8 vertexType, 
		GLuint textureId1,
		GLuint textureId2,
		BlendMode::Enum blendMode, 
		float z,
		Shader* shader
		)
{
	Renderer2DBatchHeader* batch;

	// Calculate the current number of bytes in the buffer
	UInt32 currentBufferUsage = (UInt32)endOfTheBuffer - (UInt32)batchBuffer;

	// Will the buffer NOT have enough space for the new data?
	if(currentBufferUsage + requestedBatchSize + sizeof(Renderer2DBatchHeader) > (UInt32)bufferSize) 
	{
		// Log a message that the buffer has overflowed
		LOG_WARN(L"Renderer2D batching buffer has overflowed and will auto-flush.  Consider flushing more often, increasing the batch buffer size, or reducing your draw calls");

		// Flush the batch
		Flush(); 
	}

	// Is there a current batch?
	if(batches.size() > 0)
	{
		// Get the current batch
		batch = batches.back();
		
		// Is the render data the same?
		if(	batch->PrimitiveType == primitiveType &&
			batch->VertexType == vertexType &&
			batch->TextureID1 == textureId1 &&
			batch->TextureID2 == textureId2 &&
			batch->BlendMode == blendMode &&
			batch->Z == z &&
			batch->ShaderPtr == shader)
		{
            // Is this a quad batch?
            if(batch->PrimitiveType == Renderer2DPrimitiveType::Quads)
            {
                // Make sure we wont pass the max allowed quads per batch
                if(batch->NumPrimitives + numPrimitives <= GDK_MAX_QUADS)
                {
                    return batch;
                }
            }
            else
            {
                // Lets use this existing batch
                return batch;
            }
		}
	}

	// No suitable batch, so we need to create a new one at the end of the batch buffer
	batch = (Renderer2DBatchHeader*) endOfTheBuffer;
	batches.push_back(batch);
	batch->CallOrder = batches.size();

	// Move the end-of-buffer pointer to the end of the new batch 
	endOfTheBuffer = &(batch[1]);

	// Setup the batch
	batch->PrimitiveType = primitiveType;
	batch->VertexType = vertexType;
	batch->TextureID1 = textureId1;
	batch->TextureID2 = textureId2;
	batch->BlendMode = blendMode;
	batch->Z = z;
	batch->ShaderPtr = shader;
	batch->NumPrimitives = 0;

	return batch;
}

// ********************************************************************************
Renderer2DBatchP2C4 Renderer2D::GetBatchP2C4(
	UInt32 numPrimitives,
	Renderer2DPrimitiveType::Enum primitiveType,
	GLuint textureId1,
	GLuint textureId2,
	BlendMode::Enum blendMode, 
	float z,
	Shader* shader
	)
{
	// Get the size (In Bytes) of the requsted vertex/primitive
	UInt32 batchDataSize = (UInt32)primitiveType * numPrimitives * sizeof(VertexP2C4);

	// Get a batch for this render data
	Renderer2DBatchHeader* batchHeader = GetBatch(
		batchDataSize,
        numPrimitives,
		(UInt8)primitiveType,
		(UInt8)Renderer2DVertexType::P2C4,
		textureId1, textureId2,
		blendMode, 
		z, 
		shader != NULL ? shader : nonTexturedShader
		);

	// Get the vertex buffer
	VertexP2C4* vertexBuffer = (VertexP2C4*) endOfTheBuffer;

	// Move the end of the batch buffer pointer ahead
	endOfTheBuffer = (void*)((UInt32)endOfTheBuffer + batchDataSize);

	// Add the primitive count to the batch
	batchHeader->NumPrimitives += numPrimitives;

	// Return the batch
	return Renderer2DBatchP2C4(batchHeader, vertexBuffer);
}

// ********************************************************************************
Renderer2DBatchP2T2C4 Renderer2D::GetBatchP2T2C4(
	UInt32 numPrimitives,
	Renderer2DPrimitiveType::Enum primitiveType,
	GLuint textureId1,
	GLuint textureId2,
	BlendMode::Enum blendMode, 
	float z,
	Shader* shader
	)
{
	// Get the size (In Bytes) of the requsted vertex/primitive
	UInt32 batchDataSize = (UInt32)primitiveType * numPrimitives * sizeof(VertexP2T2C4);

	// Get a batch for this render data
	Renderer2DBatchHeader* batchHeader = GetBatch(
		batchDataSize,
        numPrimitives,
		(UInt8)primitiveType,
		(UInt8)Renderer2DVertexType::P2T2C4,
		textureId1, textureId2,
		blendMode, 
		z, 
		shader != NULL ? shader : nonTexturedShader
		);

	// Get the vertex buffer
	VertexP2T2C4* vertexBuffer = (VertexP2T2C4*) endOfTheBuffer;

	// Move the end of the batch buffer pointer ahead
	endOfTheBuffer = (void*)((UInt32)endOfTheBuffer + batchDataSize);

	// Add the primitive count to the batch
	batchHeader->NumPrimitives += numPrimitives;

	// Return the batch
	return Renderer2DBatchP2T2C4(batchHeader, vertexBuffer);
}

// ********************************************************************************
void Renderer2D::DrawTexturedQuad(
			GLuint glTextureId, 
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX1, float destY1, float destX2, float destY2, 
			float destX3, float destY3, float destX4, float destY4, 
			const Color& color, BlendMode::Enum blendMode, float z
			)
{
	// Do we need to apply the world transform to the vertices?
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(destX1, destY1);
		worldTransform.TransformVertexInPlace(destX2, destY2);
		worldTransform.TransformVertexInPlace(destX3, destY3);
		worldTransform.TransformVertexInPlace(destX4, destY4);
	}

	// Get the batch for this render data
	Renderer2DBatchP2T2C4 batch = GetBatchP2T2C4(1, Renderer2DPrimitiveType::Quads, glTextureId, 0, blendMode, z, texturedShader);

	// Add the vertex data
	batch.AddVertex(destX1, destY1, srcX1, srcY1, color.R, color.G, color.B, color.A);
	batch.AddVertex(destX2, destY2, srcX2, srcY1, color.R, color.G, color.B, color.A);
	batch.AddVertex(destX3, destY3, srcX1, srcY2, color.R, color.G, color.B, color.A);
	batch.AddVertex(destX4, destY4, srcX2, srcY2, color.R, color.G, color.B, color.A);
}

// ********************************************************************************
void Renderer2D::DrawTexturedQuad(
			GLuint glTextureId, 
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX1, float destY1, float destX2, float destY2,
			const Color& color, BlendMode::Enum blendMode, float z
			)
{
	// Draw the Axis-Aligned quad as a free-form quad
	DrawTexturedQuad(
		glTextureId, 
		srcX1, srcY1, srcX2, srcY2, 
		destX1, destY1, destX2, destY1, destX1, destY2, destX2, destY2,
		color, blendMode, z
		);
}

// ********************************************************************************
void Renderer2D::DrawTexturedQuad(
			Renderer2DBatchP2T2C4& batch,
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX1, float destY1, float destX2, float destY2, 
			float destX3, float destY3, float destX4, float destY4,
			const Color& color
			)
{
	// Do we need to apply the world transform to the vertices?
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(destX1, destY1);
		worldTransform.TransformVertexInPlace(destX2, destY2);
		worldTransform.TransformVertexInPlace(destX3, destY3);
		worldTransform.TransformVertexInPlace(destX4, destY4);
	}

	// Add the vertex data
	batch.AddVertex(destX1, destY1, srcX1, srcY1, color.R, color.G, color.B, color.A);
	batch.AddVertex(destX2, destY2, srcX2, srcY1, color.R, color.G, color.B, color.A);
	batch.AddVertex(destX3, destY3, srcX1, srcY2, color.R, color.G, color.B, color.A);
	batch.AddVertex(destX4, destY4, srcX2, srcY2, color.R, color.G, color.B, color.A);
}

// ********************************************************************************
void Renderer2D::DrawImage(
			AtlasImage *image, 
			const Vector2& position, 
			const Color& color,
			float rotation, float scaleX, float scaleY, 
			BlendMode::Enum blendMode, float z
			)
{
	// Get the texture of the image's sheet
	Texture2D *texture = image->Sheet->Texture;

	// Calculate the extents of the destination quad, relative to the reference point
	float extentLeft = (image->CroppedImageX - image->Width * image->ReferencePoint.X);
	float extentRight = extentLeft + image->CroppedImageWidth;
	float extentTop = (image->CroppedImageY - image->Height * image->ReferencePoint.Y);
	float extentBottom = extentTop + image->CroppedImageHeight;

	float dx1 = extentLeft,		dy1 = extentTop;
	float dx2 = extentRight,	dy2 = extentTop;
	float dx3 = extentLeft,		dy3 = extentBottom;
	float dx4 = extentRight,	dy4 = extentBottom;

	// Apply scaling and/or rotation?
	if(rotation != 0.0f || scaleX != 1.0f || scaleY != 1.0f)
	{
		// Do a full rotate/scale/translate matrix
		Matrix2D matrix;
		matrix.ScaleRotateTranslate(scaleX, scaleY, position.X, position.Y, rotation);
		matrix.TransformVertexInPlace(dx1, dy1);
		matrix.TransformVertexInPlace(dx2, dy2);
		matrix.TransformVertexInPlace(dx3, dy3);
		matrix.TransformVertexInPlace(dx4, dy4);
	}
	else
	{
		// Just apply translation
		dx1 += position.X; dy1 += position.Y;
		dx2 += position.X; dy2 += position.Y;
		dx3 += position.X; dy3 += position.Y;
		dx4 += position.X; dy4 += position.Y;
	}

	// Draw the quad
	DrawTexturedQuad(
		texture->GLTextureId, 
		image->TopLeftTexCoord.X, image->TopLeftTexCoord.Y, image->BottomRightTexCoord.X, image->BottomRightTexCoord.Y,
		dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4, 
		color, blendMode, z
		);
}

// ********************************************************************************
void Renderer2D::DrawImage(
			Renderer2DBatchP2T2C4& batch,
			AtlasImage *image, 
			const Vector2& position, 
			const Color& color,
			float rotation, float scaleX, float scaleY
			)
{
	// Calculate the extents of the destination quad, relative to the reference point
	float extentLeft = (image->CroppedImageX - image->Width * image->ReferencePoint.X);
	float extentRight = extentLeft + image->CroppedImageWidth;
	float extentTop = (image->CroppedImageY - image->Height * image->ReferencePoint.Y);
	float extentBottom = extentTop + image->CroppedImageHeight;

	float dx1 = extentLeft,		dy1 = extentTop;
	float dx2 = extentRight,	dy2 = extentTop;
	float dx3 = extentLeft,		dy3 = extentBottom;
	float dx4 = extentRight,	dy4 = extentBottom;

	// Apply scaling and/or rotation?
	if(rotation != 0.0f || scaleX != 1.0f || scaleY != 1.0f)
	{
		// Do a full rotate/scale/translate matrix
		Matrix2D matrix;
		matrix.ScaleRotateTranslate(scaleX, scaleY, position.X, position.Y, rotation);
		matrix.TransformVertexInPlace(dx1, dy1);
		matrix.TransformVertexInPlace(dx2, dy2);
		matrix.TransformVertexInPlace(dx3, dy3);
		matrix.TransformVertexInPlace(dx4, dy4);
	}
	else
	{
		// Just apply translation
		dx1 += position.X; dy1 += position.Y;
		dx2 += position.X; dy2 += position.Y;
		dx3 += position.X; dy3 += position.Y;
		dx4 += position.X; dy4 += position.Y;
	}

	// Draw the quad
	DrawTexturedQuad(
		batch,
		image->TopLeftTexCoord.X, image->TopLeftTexCoord.Y, image->BottomRightTexCoord.X, image->BottomRightTexCoord.Y,
		dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4, 
		color
		);
}

// ********************************************************************************
void Renderer2D::DrawImage(
	AtlasImage *image, 
	const Vector2& topLeft, 
	const Vector2& bottomRight, 
	const Color& color, BlendMode::Enum blendMode, float z
	)
{
	// Get the texture of the image's sheet
	Texture2D *texture = image->Sheet->Texture;

	float destWidth = bottomRight.X - topLeft.X;
	float destHeight = bottomRight.Y - topLeft.Y;

	// Calculate the positions of the cropped image within the given destination coordinates
	float left = topLeft.X + destWidth * image->CroppedImageX / image->Width;
	float right = left + destWidth * image->CroppedImageWidth / image->Width;
	float top = topLeft.Y + destHeight * image->CroppedImageY / image->Height;
	float bottom = top + destHeight * image->CroppedImageHeight / image->Height;

	// Calculate the extents of the destination quad, relative to the reference point
	float dx1 = left,		dy1 = top;
	float dx2 = right,		dy2 = top;
	float dx3 = left,		dy3 = bottom;
	float dx4 = right,		dy4 = bottom;

	// Draw the quad
	DrawTexturedQuad(
		texture->GLTextureId, 
		image->TopLeftTexCoord.X, image->TopLeftTexCoord.Y, image->BottomRightTexCoord.X, image->BottomRightTexCoord.Y,
		dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4, 
		color, blendMode, z
		);
}

// ********************************************************************************
void Renderer2D::DrawImage(
	AtlasImage *image, 
	const Rectangle2& sourceRect, 
	const Rectangle2& destRect, 
	const Color& color, BlendMode::Enum blendMode, float z
	)
{
	// Get the texture of the image's sheet
	Texture2D *texture = image->Sheet->Texture;

	float widthTexelRatio = 1.0f / texture->Width;
	float heightTexelRatio = 1.0f / texture->Height;
	float texelX = sourceRect.Position.X * widthTexelRatio;
	float texelY = sourceRect.Position.Y * heightTexelRatio;
	float texelWidth = sourceRect.Size.X * widthTexelRatio;
	float texelHeight = sourceRect.Size.Y * heightTexelRatio;

	// Draw the quad
	DrawTexturedQuad(
		texture->GLTextureId, 
		image->TopLeftTexCoord.X + texelX,
		image->TopLeftTexCoord.Y + texelY,		
		image->TopLeftTexCoord.X + texelX + texelWidth,
		image->TopLeftTexCoord.Y + texelY + texelHeight,
		destRect.Position.X,					destRect.Position.Y,
		destRect.Position.X + destRect.Size.X,	destRect.Position.Y,
		destRect.Position.X,					destRect.Position.Y + destRect.Size.Y,
		destRect.Position.X + destRect.Size.X,	destRect.Position.Y + destRect.Size.Y,
		color, blendMode, z
		);
}

// ********************************************************************************
void Renderer2D::DrawSprite(Sprite* sprite)
{
	// Is the sprite visible?
	if(sprite->Visible)
	{
		// Get the current frame image
		AtlasImage *image = sprite->GetCurrentFrameImage();

		// Draw the atlas image with the sprite properties
		DrawImage(
			image,
			sprite->Position,
			sprite->Color,
			sprite->Rotation,
			sprite->Scale.X,
			sprite->Scale.Y,
			sprite->BlendMode,
			sprite->Z
			);
	}
}

// ********************************************************************************
Vector2 Renderer2D::DrawText(
	BMFont *font, 
	const wchar_t *text, 
	const Vector2& position, 
	const Color& color, 
	const Vector2& scale, 
	BlendMode::Enum blendMode,
	float z
	)
{
	Vector2 drawPos = position;

	// Walk through the input text
	const wchar_t *current = text;
	while(*current != 0)
	{
        wchar_t ch = *current;
		if(ch == L'\n')
		{
			// Move to the next line
			drawPos.X = position.X;
			drawPos.Y += font->lineHeight * scale.Y;
		}
		else
		{
            // Get the BMFontCharacter associated with this character
            BMFontCharacterMap::iterator iter = font->characters.find(ch);
            if(iter == font->characters.end())
            {
                // Character doesnt exist in the font
                LOG_WARN(L"The character [%lc][%u] doesn't exist in the character map for the font [%hs]", ch, (UInt32)ch, font->Name.c_str()); 
            }
            else
            {
                BMFontCharacter &fontChar = iter->second;

                // Get the texture used by this character
                Texture2D *texture = font->pages[fontChar.Page];

                // Calculate the texture coordinates of this letter
                float tx1 = (fontChar.X) * texture->TexelWidth;
                float ty1 = (fontChar.Y) * texture->TexelHeight;
                float tx2 = tx1 + (fontChar.Width) * texture->TexelWidth;
                float ty2 = ty1 + (fontChar.Height) * texture->TexelHeight;

                // Calculate the screen coords of the quad for this char
                float dx1 = drawPos.X + fontChar.XOffset * scale.X;
                float dy1 = drawPos.Y + fontChar.YOffset * scale.Y;
                float dx2 = dx1 + fontChar.Width  * scale.X;
                float dy2 = dy1 + fontChar.Height * scale.Y;

                // Draw this character image
                DrawTexturedQuad(texture->GLTextureId, tx1, ty1, tx2, ty2, dx1, dy1, dx2, dy2, color, blendMode, z);

                // Move the position to after the character
                drawPos.X += fontChar.XAdvance * scale.X;
            }
		}

		// next character
		current++;
	}

	return drawPos;
}

/*
// ********************************************************************************
void Renderer2D::DrawTexturedPolyLine(
	TexturedLineVertex* vertices, 
	int numVertices, 
	float lineWidth, 
	bool flipYOnInvertedX, 
	const Color& color, 
	BlendMode::Enum blendMode,
	float z
	)
{
	float halfWidth = lineWidth * 0.5f;

	// Need 4 vertex pointers for the LAST, START, END, & NEXT vertices
	TexturedLineVertex *v0 = NULL;
	TexturedLineVertex *v1 = NULL;
	TexturedLineVertex *v2 = NULL;
	TexturedLineVertex *v3 = NULL;

	int lastVertex = numVertices - 1;
	int lastNextVertex = numVertices - 2;

	Vector2 segmentDirection, side1Extent, side2Extent;

	// Loop through the line segments
	for(int index=0; index < lastVertex; index++)
	{
		// Get the START & END vertices
		v1 = &vertices[index];
		v2 = &vertices[index+1];

		// Get the NEXT vertex (if one exists)
		if(index < lastNextVertex)
			v3 = &vertices[index+2];
		else 
			v3 = NULL;

		// Get the direction vector of the segment
		segmentDirection = v2->Position - v1->Position;
		side1Extent = segmentDirection;
		side2Extent = segmentDirection;

		// Does the LAST vertex exist?
		if(v0 != NULL)
		{
			// Use the last vertex in the side 1 extent vector
			side1Extent += v1->Position - v0->Position ;
		}

		// Does the NEXT vertex exist?
		if(v3 != NULL)
		{
			// Use the NEXT vertex in the side 2 extent vector
			side2Extent += v3->Position - v2->Position ;
		}

		// Get the perpediculars of the extent vectors
		float temp = side1Extent.X;
		side1Extent.X = -side1Extent.Y; 
		side1Extent.Y = temp;
		temp = side2Extent.X;
		side2Extent.X = -side2Extent.Y; 
		side2Extent.Y = temp;

		// Normalize the side extents * scale them by the line width
		side1Extent.Normalize();
		side2Extent.Normalize();
		side1Extent *= halfWidth;
		side2Extent *= halfWidth;

		// Get the image & texture coords
		AtlasImage* image = v1->SegmentImage;
		float txLength = image->BottomRightTexCoord.X - image->TopLeftTexCoord.X;
		float tx1 = image->TopLeftTexCoord.X + txLength * v1->startTextureOffset;
		float ty1 = image->TopLeftTexCoord.Y;
		float tx2 = image->TopLeftTexCoord.X + txLength * v1->endTextureOffset;
		float ty2 = image->BottomRightTexCoord.Y;

		if(flipYOnInvertedX && (v1->Position.X > v2->Position.X))
		{
			ty1 = image->BottomRightTexCoord.Y;
			ty2 = image->TopLeftTexCoord.Y;
		}

		// Draw the quad
		DrawTexturedQuad(image->Sheet->Texture->GLTextureId,
			tx1, ty1, tx2, ty2,
			v1->Position.X + side1Extent.X,
			v1->Position.Y + side1Extent.Y,
			v2->Position.X + side2Extent.X,
			v2->Position.Y + side2Extent.Y,
			v1->Position.X - side1Extent.X,
			v1->Position.Y - side1Extent.Y,
			v2->Position.X - side2Extent.X,
			v2->Position.Y - side2Extent.Y,
			color, blendMode, z
			);

		// Copy the START vertex to the LAST vertex
		v0 = v1;
	}
}
*/


// #######################################################################################################################

// 2D Primitive Rendering

// ********************************************************************
void Renderer2D::DrawLine(
	const Vector2& start, const Vector2& end, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	Vector2 finalStart = start;
	Vector2 finalEnd = end;
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(finalStart.X, finalStart.Y);
		worldTransform.TransformVertexInPlace(finalEnd.X, finalEnd.Y);
	}

	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(1, Renderer2DPrimitiveType::Lines, 0, 0, blendMode, z, nonTexturedShader);

	// Add the vertex data
	batch.AddVertex(finalStart.X, finalStart.Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(finalEnd.X, finalEnd.Y, color.R, color.G, color.B, color.A);
}

// ********************************************************************
void Renderer2D::DrawRectangle(
	const Rectangle2& rectangle, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(2, Renderer2DPrimitiveType::Triangles, 0, 0, blendMode, z, nonTexturedShader);
	
	Vector2 positions[] = // TL, TR, BL, BR
	{
		Vector2(rectangle.Position.X, rectangle.Position.Y),
		Vector2(rectangle.Position.X + rectangle.Size.X, rectangle.Position.Y),
		Vector2(rectangle.Position.X, rectangle.Position.Y + rectangle.Size.Y),
		Vector2(rectangle.Position.X + rectangle.Size.X, rectangle.Position.Y + rectangle.Size.Y)
	};
	
	// Do we have a custom transform?
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(positions[0].X, positions[0].Y);
		worldTransform.TransformVertexInPlace(positions[1].X, positions[1].Y);
		worldTransform.TransformVertexInPlace(positions[2].X, positions[2].Y);
		worldTransform.TransformVertexInPlace(positions[3].X, positions[3].Y);
	}

	// TL-TR-BL
	batch.AddVertex(positions[0].X, positions[0].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[1].X, positions[1].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);

	// BL-TR-BR
	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[1].X, positions[1].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[3].X, positions[3].Y, color.R, color.G, color.B, color.A);
}

// ********************************************************************
void Renderer2D::DrawRectangleOutline(
	const Rectangle2& rectangle, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(4, Renderer2DPrimitiveType::Lines, 0, 0, blendMode, z, nonTexturedShader);
	
	Vector2 positions[] = // TL, TR, BR, BL
	{
		Vector2(rectangle.Position.X, rectangle.Position.Y),
		Vector2(rectangle.Position.X + rectangle.Size.X, rectangle.Position.Y),
		Vector2(rectangle.Position.X + rectangle.Size.X, rectangle.Position.Y + rectangle.Size.Y),
		Vector2(rectangle.Position.X, rectangle.Position.Y + rectangle.Size.Y)
		
	};
	
	// Do we have a custom transform?
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(positions[0].X, positions[0].Y);
		worldTransform.TransformVertexInPlace(positions[1].X, positions[1].Y);
		worldTransform.TransformVertexInPlace(positions[2].X, positions[2].Y);
		worldTransform.TransformVertexInPlace(positions[3].X, positions[3].Y);
	}

	// Add the 4 Segments
	batch.AddVertex(positions[0].X, positions[0].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[1].X, positions[1].Y, color.R, color.G, color.B, color.A);

	batch.AddVertex(positions[1].X, positions[1].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);

	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[3].X, positions[3].Y, color.R, color.G, color.B, color.A);

	batch.AddVertex(positions[3].X, positions[3].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[0].X, positions[0].Y, color.R, color.G, color.B, color.A);
}

// ********************************************************************
void Renderer2D::DrawDiamond(
	const Rectangle2& rectangle, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(2, Renderer2DPrimitiveType::Triangles, 0, 0, blendMode, z, nonTexturedShader);
	
	float halfWidth = rectangle.Size.X * 0.5f;
	float halfHeight = rectangle.Size.Y * 0.5f;

	Vector2 positions[] =  // L-T-R-B
	{
		Vector2(rectangle.Position.X, rectangle.Position.Y + halfHeight),
		Vector2(rectangle.Position.X + halfWidth, rectangle.Position.Y),
		Vector2(rectangle.Position.X + rectangle.Size.X, rectangle.Position.Y + halfHeight),
		Vector2(rectangle.Position.X + halfWidth, rectangle.Position.Y + rectangle.Size.Y)
	};
	
	// Do we have a custom transform?
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(positions[0].X, positions[0].Y);
		worldTransform.TransformVertexInPlace(positions[1].X, positions[1].Y);
		worldTransform.TransformVertexInPlace(positions[2].X, positions[2].Y);
		worldTransform.TransformVertexInPlace(positions[3].X, positions[3].Y);
	}

	// L-T-R
	batch.AddVertex(positions[0].X, positions[0].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[1].X, positions[1].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);

	// L-R-B
	batch.AddVertex(positions[0].X, positions[0].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[3].X, positions[3].Y, color.R, color.G, color.B, color.A);
}

// ********************************************************************
void Renderer2D::DrawDiamondOutline(
	const Rectangle2& rectangle, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(4, Renderer2DPrimitiveType::Lines, 0, 0, blendMode, z, nonTexturedShader);
	
	float halfWidth = rectangle.Size.X * 0.5f;
	float halfHeight = rectangle.Size.Y * 0.5f;

	Vector2 positions[] =  // L-T-R-B
	{
		Vector2(rectangle.Position.X, rectangle.Position.Y + halfHeight),
		Vector2(rectangle.Position.X + halfWidth, rectangle.Position.Y),
		Vector2(rectangle.Position.X + rectangle.Size.X, rectangle.Position.Y + halfHeight),
		Vector2(rectangle.Position.X + halfWidth, rectangle.Position.Y + rectangle.Size.Y)
	};
	
	// Do we have a custom transform?
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(positions[0].X, positions[0].Y);
		worldTransform.TransformVertexInPlace(positions[1].X, positions[1].Y);
		worldTransform.TransformVertexInPlace(positions[2].X, positions[2].Y);
		worldTransform.TransformVertexInPlace(positions[3].X, positions[3].Y);
	}

	// Add the 4 segments
	batch.AddVertex(positions[0].X, positions[0].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[1].X, positions[1].Y, color.R, color.G, color.B, color.A);

	batch.AddVertex(positions[1].X, positions[1].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);

	batch.AddVertex(positions[2].X, positions[2].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[3].X, positions[3].Y, color.R, color.G, color.B, color.A);

	batch.AddVertex(positions[3].X, positions[3].Y, color.R, color.G, color.B, color.A);
	batch.AddVertex(positions[0].X, positions[0].Y, color.R, color.G, color.B, color.A);
}

// ********************************************************************
void Renderer2D::DrawCircle(
	const Circle2& circle, int segments, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(segments, Renderer2DPrimitiveType::Triangles, 0, 0, blendMode, z, nonTexturedShader);

	// Calculate the amount of theta per segment
	float thetaPerSegment = Math::TWO_PI / (float)segments;

	// Start the first version at theta = 0
	float startX = circle.Center.X + circle.Radius;
	float startY = circle.Center.Y;
	float endX = 0.0f;
	float endY = 0.0f;

	// If we have a custom transform, get the xformd center & start vertices
	Vector2 center = circle.Center;
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(center.X, center.Y);
		worldTransform.TransformVertexInPlace(startX, startY);
	}
	
	// Loop through the segments
	for(int i = 0; i < segments; i++)
	{
		// Calculate the position at the end of the segment
		float theta = thetaPerSegment * (i+1);
		endX = Math::Cos(theta) * circle.Radius + circle.Center.X;
		endY = Math::Sin(theta) * circle.Radius + circle.Center.Y;

		// Transform the end vertex (if we have a transform)
		if(applyWorldTransform)
			worldTransform.TransformVertexInPlace(endX, endY);
		
		// Add the Center vertex
		batch.AddVertex(center.X, center.Y, color.R, color.G, color.B, color.A);

		// Add the segment Start & End vertices
		batch.AddVertex(endX, endY, color.R, color.G, color.B, color.A);
		batch.AddVertex(startX, startY, color.R, color.G, color.B, color.A);

		// This end vertex, is the position of the next segment's start
		startX = endX;
		startY = endY;
	}
}

// ********************************************************************
void Renderer2D::DrawCircleOutline(
	const Circle2& circle, int segments, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(segments, Renderer2DPrimitiveType::Lines, 0, 0, blendMode, z, nonTexturedShader);

	// Calculate the amount of theta per segment
	float thetaPerSegment = Math::TWO_PI / (float)segments;

	// Start the first version at theta = 0
	float startX = circle.Center.X + circle.Radius;
	float startY = circle.Center.Y;
	float endX = 0.0f;
	float endY = 0.0f;
	
	// If we have a custom transform, get the xformd start vertex
	if(applyWorldTransform)
		worldTransform.TransformVertexInPlace(startX, startY);

	// Loop through the segments
	for(int i = 0; i < segments; i++)
	{
		// Calculate the position at the end of the segment
		float theta = thetaPerSegment * (i+1);
		endX = Math::Cos(theta) * circle.Radius + circle.Center.X;
		endY = Math::Sin(theta) * circle.Radius + circle.Center.Y;

		// If we have a custom transform, get the xformd end vertex
		if(applyWorldTransform)
			worldTransform.TransformVertexInPlace(endX, endY);

		// Add the segment Start & End vertices
		batch.AddVertex(endX, endY, color.R, color.G, color.B, color.A);
		batch.AddVertex(startX, startY, color.R, color.G, color.B, color.A);

		// This end vertex, is the position of the next segment's start
		startX = endX;
		startY = endY;
	}
}

// ********************************************************************
void Renderer2D::DrawTexturedCircle(
	GLuint glTextureId,
	const Circle2& circle, int segments, 
	const Vector2& topLeftTC, 
	const Vector2& bottomRightTC,
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	Renderer2DBatchP2T2C4 batch = GetBatchP2T2C4(segments, Renderer2DPrimitiveType::Triangles, glTextureId, 0, blendMode, z, texturedShader);

	// Calculate the amount of theta per segment
	float thetaPerSegment = Math::TWO_PI / (float)segments;

	// Start the first version at theta = 0
	float startX = circle.Center.X + circle.Radius;
	float startY = circle.Center.Y;
	float endX = 0.0f;
	float endY = 0.0f;

	Vector2 centerTC = (topLeftTC + bottomRightTC) * 0.5f;
	float tcRadius = Math::Abs(centerTC.X - bottomRightTC.X);
	float startTX = bottomRightTC.X;
	float startTY = centerTC.Y;
	float endTX = 0.0f;
	float endTY = 0.0f;

	// If we have a custom transform, get the xformd center & start vertices
	Vector2 center = circle.Center;
	if(applyWorldTransform)
	{
		worldTransform.TransformVertexInPlace(center.X, center.Y);
		worldTransform.TransformVertexInPlace(startX, startY);
	}
	
	// Loop through the segments
	for(int i = 0; i < segments; i++)
	{
		// Calculate the position at the end of the segment
		float theta = thetaPerSegment * (i+1);
		endX = Math::Cos(theta) * circle.Radius + circle.Center.X;
		endY = Math::Sin(theta) * circle.Radius + circle.Center.Y;
		endTX = Math::Cos(theta) * tcRadius + centerTC.X;
		endTY = Math::Sin(theta) * tcRadius + centerTC.Y;

		// Transform the end vertex (if we have a transform)
		if(applyWorldTransform)
			worldTransform.TransformVertexInPlace(endX, endY);
		
		// Add the Center vertex
		batch.AddVertex(center.X, center.Y, centerTC.X, centerTC.Y, color.R, color.G, color.B, color.A);

		// Add the segment Start & End vertices
		batch.AddVertex(endX, endY, endTX, endTY, color.R, color.G, color.B, color.A);
		batch.AddVertex(startX, startY, startTX, startTY, color.R, color.G, color.B, color.A);

		// This end vertex, is the position of the next segment's start
		startX = endX;
		startY = endY;
		startTX = endTX;
		startTY = endTY;
	}
}

// ********************************************************************
void Renderer2D::DrawPolygon(
	Vector2* vertices, int vertexCount, 
	const Color& color, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	if(vertexCount < 3)
		return;

	// Get the batch for this render data
	Renderer2DBatchP2C4 batch = GetBatchP2C4(vertexCount - 2, Renderer2DPrimitiveType::Triangles, 0, 0, blendMode, z, nonTexturedShader);

	// Loop through the vertices (starting at index 2)
	for(int index = 2; index < vertexCount; index++)
	{
		// Add the last 3 vertices as a triangle
		batch.AddVertex(vertices[0].X, vertices[0].Y, color.R, color.G, color.B, color.A);
		batch.AddVertex(vertices[index-1].X, vertices[index-1].Y, color.R, color.G, color.B, color.A);
		batch.AddVertex(vertices[index].X, vertices[index].Y, color.R, color.G, color.B, color.A);
	}
}

// ********************************************************************
void Renderer2D::DrawPolyline(
	Vector2* vertices, int vertexCount, 
	const Color& color, 
	bool closed, 
	BlendMode::Enum blendMode, 
	float z
	)
{
	// Get the batch for this render data
	UInt32 numLines = closed ? vertexCount : vertexCount - 1;
	Renderer2DBatchP2C4 batch = GetBatchP2C4(numLines, Renderer2DPrimitiveType::Lines, 0, 0, blendMode, z, nonTexturedShader);

	// Loop through the vertices (starting at index 1)
	for(int index = 1; index < vertexCount; index++)
	{
		// Add the last 2 vertices as a line
		batch.AddVertex(vertices[index-1].X, vertices[index-1].Y, color.R, color.G, color.B, color.A);
		batch.AddVertex(vertices[index].X, vertices[index].Y, color.R, color.G, color.B, color.A);
	}

	// If the polyline is closed, add 1 more line
	if(closed)
	{
		batch.AddVertex(vertices[0].X, vertices[0].Y, color.R, color.G, color.B, color.A);
		batch.AddVertex(vertices[vertexCount-1].X, vertices[vertexCount-1].Y, color.R, color.G, color.B, color.A);
	}	
}