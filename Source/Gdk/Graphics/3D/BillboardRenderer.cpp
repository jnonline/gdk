/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "BillboardRenderer.h"

using namespace Gdk;

// ***********************************************************************
// Utility Macros

#define BILLBOARD_MAX_QUADS 1024

#define CHECK_BUFFER_SPACE(dataSize)  \
	unsigned int numBytes = (unsigned int)endOfTheBuffer - (unsigned int)batchBuffer; \
	if(numBytes + dataSize > (unsigned int)bufferSize) { Flush(); }


// #######################################################################################################

// Vertex Utilities

// ---------------------------------
inline void AddBillboardVertex(
		   void*& buffer, 
		   GLfloat x, GLfloat y, GLfloat z, GLfloat rotation,
		   GLfloat tx, GLfloat ty, GLfloat extentX, GLfloat extentY,
		   UInt8 r, UInt8 g, UInt8 b, UInt8 a,
		   GLfloat facingX, GLfloat facingY, GLfloat facingZ, GLfloat facingType
		   )
{
	BillboardVertex* vertex = (BillboardVertex*) buffer;
	vertex->X = x;
	vertex->Y = y;
	vertex->Z = z;
	vertex->Rotation = rotation;
	vertex->TX = tx;
	vertex->TY = ty;
	vertex->ExtentX = extentX;
	vertex->ExtentY = extentY;
	vertex->R = r;
	vertex->G = g;
	vertex->B = b;
	vertex->A = a;
	vertex->FacingX = facingX;
	vertex->FacingY = facingY;
	vertex->FacingZ = facingZ;
	vertex->FacingType = facingType;

	buffer = (void*)((unsigned int)buffer + sizeof(BillboardVertex));
}

// #######################################################################################################

// Static Instantiations
void* BillboardRenderer::batchBuffer = NULL;
void* BillboardRenderer::endOfTheBuffer = NULL;
int BillboardRenderer::bufferSize = 0;
BillboardRendererBatchSet BillboardRenderer::batches;

// ********************************************************************************
void BillboardRenderer::Init()
{
	// Setup internal buffers
	// ----------------------------------------

	// Allocate the batch buffer
	bufferSize = 512000;
	batchBuffer = GdkAlloc(bufferSize);
	endOfTheBuffer = batchBuffer;

	// Reserve the batch index vector
	batches.reserve(200);
}	

// ********************************************************************************
void BillboardRenderer::Shutdown()
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
void BillboardRenderer::Flush()
{
	// Render the batches
	// ----------------------

	// Unbind any vertex & index buffers
	Graphics::BindVertexBuffer(0);
	Graphics::BindIndexBuffer(0);

	// Enable blending
	Graphics::EnableBlending(true);

	// Disable depth write/testing & culling
	Graphics::EnableDepthTest(true);
	Graphics::EnableDepthWrite(false);
	Graphics::SetCullingMode(CullingMode::Disabled);

	// All vertices have 4 vertex attribute arrays
	Graphics::EnableVertexAttribArray(0, true);
	Graphics::EnableVertexAttribArray(1, true);
	Graphics::EnableVertexAttribArray(2, true);
	Graphics::EnableVertexAttribArray(3, true);

	// Turn on the shader
	SharedAssets::Shaders.Billboard.Textured->Apply();

	// Loop through the batches
	for(BillboardRendererBatchSetIterator batchIter = batches.begin(); batchIter != batches.end(); batchIter++)
	{
		// Get this batch
		BillboardRendererBatch* batch = *batchIter;

		// Bind the texture for the batch
		Graphics::BindTexture(batch->TextureID, TextureUnit::Unit_0);

		// Setup blending
		Graphics::SetBlendMode(batch->BlendMode);

		// Get a pointer to the vertex data at the end of the batch
		void* vertexData = &(batch[1]);

		// Set the vertex data pointers
		GLuint stride = sizeof(BillboardVertex);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, vertexData);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)((long)vertexData + sizeof(GLfloat[4])) );
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)((long)vertexData + sizeof(GLfloat[8])) );
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)((long)vertexData + sizeof(GLfloat[8]) + sizeof(GLubyte[4])) );

		// Use the index buffer to draw the quads
		glDrawElements(GL_TRIANGLES, batch->NumPrimitives * 6, GL_UNSIGNED_SHORT, SharedAssets::QuadIndexBuffer);
	}

	Graphics::EnableDepthWrite(true);

	// Clear the batches
	endOfTheBuffer = batchBuffer;
	batches.clear();
}

// ********************************************************************************
BillboardRendererBatch* BillboardRenderer::GetBatch(
		  GLuint textureId, 
		  BlendMode::Enum blendMode
		  )
{
	BillboardRendererBatch* batch;

	// Is there a current batch?
	if(batches.size() > 0)
	{
		// Get the current batch
		batch = batches.back();
		
		// Is the render data the same?
		if(	batch->TextureID == textureId &&
			batch->BlendMode == blendMode 
			)
		{
			// This batch is re-usable
			return batch;
		}
	}

	CHECK_BUFFER_SPACE( sizeof(BillboardRenderer) );

	// No suitable batch, so we need to create a new one at the end of the batch buffer
	batch = (BillboardRendererBatch*) endOfTheBuffer;
	batches.push_back(batch);
	batch->CallOrder = batches.size();

	// Move the end-of-buffer pointer to the end of the new batch 
	endOfTheBuffer = &(batch[1]);

	// Setup the batch
	batch->TextureID = textureId;
	batch->BlendMode = blendMode;
	batch->NumPrimitives = 0;
	batch->NumVertices = 0;

	return batch;
}

// ********************************************************************************
void BillboardRenderer::DrawQuad(
			GLuint glTextureId,
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX, float destY, float destZ,
			float extentLeft, float extentRight, float extentUp, float extentDown,
			const Color& color,
			float rotation,
			BillboardFacing::Enum facing,
			const Vector3& customFacing,
			BlendMode::Enum blendMode
			)
{
	CHECK_BUFFER_SPACE( sizeof(BillboardVertex) * 4 );

	// Get the batch for this render data
	BillboardRendererBatch* batch = GetBatch(glTextureId, blendMode);

	float dx1 = extentLeft, dy1 = extentUp;
	float dx2 = extentRight, dy2 = extentUp;
	float dx3 = extentLeft, dy3 = extentDown;
	float dx4 = extentRight, dy4 = extentDown;

	if(rotation != 0.0f)
	{
		Matrix2D mat;
		mat.Rotate(rotation);
		mat.TransformVertexInPlace(dx1, dy1);
		mat.TransformVertexInPlace(dx2, dy2);
		mat.TransformVertexInPlace(dx3, dy3);
		mat.TransformVertexInPlace(dx4, dy4);
	}

	// Add the vertex data
	AddBillboardVertex(endOfTheBuffer, destX, destY, destZ, rotation, srcX1, srcY1, dx1, dy1, color.R, color.G, color.B, color.A, customFacing.X, customFacing.Y, customFacing.Z, (float)facing);
	AddBillboardVertex(endOfTheBuffer, destX, destY, destZ, rotation, srcX2, srcY1, dx2, dy2, color.R, color.G, color.B, color.A, customFacing.X, customFacing.Y, customFacing.Z, (float)facing);
	AddBillboardVertex(endOfTheBuffer, destX, destY, destZ, rotation, srcX1, srcY2, dx3, dy3, color.R, color.G, color.B, color.A, customFacing.X, customFacing.Y, customFacing.Z, (float)facing);
	AddBillboardVertex(endOfTheBuffer, destX, destY, destZ, rotation, srcX2, srcY2, dx4, dy4, color.R, color.G, color.B, color.A, customFacing.X, customFacing.Y, customFacing.Z, (float)facing);

	batch->NumVertices += 4;
	batch->NumPrimitives += 1;
}

// ********************************************************************************
void BillboardRenderer::DrawImage(
			AtlasImage *image,
			const Vector3& position, 
			const Color& color,
			float rotation, float scaleX, float scaleY,
			BillboardFacing::Enum facing,
			const Vector3& customFacing,
			BlendMode::Enum blendMode
			)
{
	// Get the texture of the image's sheet
	Texture2D *texture = image->Sheet->Texture;

	// Calculate the extents of the destination quad, relative to the reference point
	float extentLeft = (image->CroppedImageX - image->Width * image->ReferencePoint.X);
	float extentRight = extentLeft + image->CroppedImageWidth;
	float extentTop = (image->CroppedImageY - image->Height * image->ReferencePoint.Y);
	float extentBottom = extentTop + image->CroppedImageHeight;

	// Scale the extents
	extentLeft *= scaleX;
	extentRight *= scaleX;
	extentTop *= scaleY;
	extentBottom *= scaleY;

	// Draw the quad
	DrawQuad(
		texture->GLTextureId,
		image->TopLeftTexCoord.X, image->TopLeftTexCoord.Y, image->BottomRightTexCoord.X, image->BottomRightTexCoord.Y,
		position.X, position.Y, position.Z, 
		extentLeft, extentRight, extentTop, extentBottom,
		color, rotation, facing, customFacing, blendMode
		);
}

// ********************************************************************************
void BillboardRenderer::DrawSprite(
		Sprite* sprite,
		BillboardFacing::Enum facing,
		const Vector3& customFacing
		)
{
	// Is the sprite visible?
	if(sprite->Visible)
	{
		// Get the current frame image
		AtlasImage *image = sprite->GetCurrentFrameImage();

		// Get the sprite 3D position
		Vector3 position(sprite->Position.X, sprite->Position.Y, sprite->Z);

		// Draw the atlas image with the sprite properties
		DrawImage(
			image,
			position,
			sprite->Color, 
			sprite->Rotation,
			sprite->Scale.X, 
			sprite->Scale.Y, 
			facing, 
			customFacing, 
			sprite->BlendMode
			);
	}
}
