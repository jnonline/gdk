/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Color.h"
#include "../Graphics.h"
#include "../../Math/2D/Vector2.h"
#include "../../Math/2D/Rectangle2.h"
#include "../../Math/2D/Circle2.h"
#include "../../Math/3D/Vector3.h"
#include "../2D/Atlas.h"
#include "../2D/BMFont.h"
#include "../2D/Sprite.h"
#include "../VertexFormats.h"

namespace Gdk
{
	// ====================================================================
	namespace BillboardFacing
	{
		enum Enum
		{
			Camera = 0,					// Billboard will face the camera
			CustomDirection = 1,		// Billboard will face a custom direction vector
		};
	}

	// ============================================================================
	struct BillboardVertex
	{
		GLfloat X, Y, Z;
		GLfloat Rotation;
		GLfloat TX, TY;
		GLfloat ExtentX, ExtentY;
		GLubyte R, G, B, A;
		GLfloat FacingX, FacingY, FacingZ;
		GLfloat FacingType;
	};

	// ====================================================================
	struct BillboardRendererBatch
	{
		// Batch Information
		short				CallOrder;
		GLuint				TextureID;
		BlendMode::Enum		BlendMode;

		// Counts
		int					NumPrimitives;
		int					NumVertices;
	};

	// Forward declares & typedefs
	typedef vector<BillboardRendererBatch*>				BillboardRendererBatchSet;
	typedef vector<BillboardRendererBatch*>::iterator	BillboardRendererBatchSetIterator;

	// ============================================================================
	class BillboardRenderer
	{

	private:
		// Private Properties
		// ----------------------------

		static void* batchBuffer;
		static void* endOfTheBuffer;
		static int bufferSize;
		static BillboardRendererBatchSet batches;

		// Hidden CTor
		BillboardRenderer() {}

		// Init & Shutdown from the Graphics class
		friend class Graphics;
		static void Init();
		static void Shutdown();
	

	public:

		// Public Methods
		// ----------------------------

		// Flush all the batch data to the GPU
		static void Flush();

		// Access the batch vector.  Useful for checking batching data
		//  NOTE: DO NOT MODIFY any of the data in the batch sets...
		static BillboardRendererBatchSet* GetRenderBatches() { return &batches; }

		// DrawQuad() - Basic Quad drawing
		// ====================================================

		// ------------------------------------
		// Draw Quad from Texture Rectangle
		//		x,y,z = position of the center of the billboarded quad
		//		extents = extent out from the center position
		static void DrawQuad(
			GLuint glTextureId,
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX, float destY, float destZ,
			float extentLeft, float extentRight, float extentUp, float extentDown,
			const Color& color = Color::WHITE,
			float rotation = 0.0f,
			BillboardFacing::Enum facing = BillboardFacing::Camera,
			const Vector3& customFacing = Vector3::UNIT_Z,
			BlendMode::Enum blendMode = BlendMode::Normal
			);

		
		// DrawImage() - Atlas Image drawing
		// ====================================================

		// ------------------------------------
		// Draw AtlasImage at given Position (with common transforms)
		static void DrawImage(
			AtlasImage *image,
			const Vector3& position, 
			const Color& color = Color::WHITE,
			float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f,
			BillboardFacing::Enum facing = BillboardFacing::Camera,
			const Vector3& customFacing = Vector3::UNIT_Z,
			BlendMode::Enum blendMode = BlendMode::Normal
			);

		// DrawSprite() - Sprite Drawing!!
		// ==================================================

		static void DrawSprite(
			Sprite *sprite,
			BillboardFacing::Enum facing = BillboardFacing::Camera,
			const Vector3& customFacing = Vector3::UNIT_Z
			);

	private:
		// Internal Methods
		// ------------------------------------

		// Get a batch point for the given type of render data
		static BillboardRendererBatch* GetBatch(
			GLuint textureId, 
			BlendMode::Enum blendMode
			);
	};


} // namespace