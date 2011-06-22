/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "../Color.h"
#include "../Graphics.h"
#include "../../Math/2D/Vector2.h"
#include "../../Math/2D/Rectangle2.h"
#include "../../Math/2D/Circle2.h"
#include "Atlas.h"
#include "BMFont.h"
#include "Sprite.h"
#include "../VertexFormats.h"

namespace Gdk
{
	// ====================================================================
	namespace Renderer2DPrimitiveType
	{
		enum Enum
		{
			Points		= 0x01,		// # is also the number of vertices for the primitive type
			Lines		= 0x02,
			Triangles	= 0x03,
			Quads		= 0x04
		};

		const char* ToString(Renderer2DPrimitiveType::Enum primitiveType);
	}

	// ====================================================================
	namespace Renderer2DVertexType
	{
		enum Enum
		{
			P2C4		= 0x01,
			P2T2C4		= 0x02,
		};

		const char* ToString(Renderer2DVertexType::Enum vertexType);
	}

	// ====================================================================
	struct Renderer2DBatchHeader
	{
		// Batch Information
		UInt8							PrimitiveType;
		UInt8							VertexType;
		short							CallOrder;
		float 							Z;
		GLuint							TextureID1;
		GLuint							TextureID2;
		BlendMode::Enum					BlendMode;
		Shader*							ShaderPtr;

		// Counts
		int								NumPrimitives;
	};

	// Container Types
	typedef vector<Renderer2DBatchHeader*>				Renderer2DBatchSet;
	typedef vector<Renderer2DBatchHeader*>::iterator	Renderer2DBatchSetIterator;

	// ====================================================================
	struct Renderer2DBatchP2C4
	{
		// Properties
		Renderer2DBatchHeader* Header;
		VertexP2C4* VertexBuffer;

		// Adds a vertex
		inline void AddVertex(GLfloat x, GLfloat y, UInt8 r, UInt8 g, UInt8 b, UInt8 a);

		// CTor
		Renderer2DBatchP2C4(Renderer2DBatchHeader* header, VertexP2C4* vertexBuffer) { this->Header = header; this->VertexBuffer = vertexBuffer; }
	};

	// ====================================================================
	struct Renderer2DBatchP2T2C4
	{
		// Properties
		Renderer2DBatchHeader* Header;
		VertexP2T2C4* VertexBuffer;

		// Adds a vertex
		inline void AddVertex(GLfloat x, GLfloat y, GLfloat tx, GLfloat ty, UInt8 r, UInt8 g, UInt8 b, UInt8 a);

		// CTor
		Renderer2DBatchP2T2C4(Renderer2DBatchHeader* header, VertexP2T2C4* vertexBuffer) { this->Header = header; this->VertexBuffer = vertexBuffer; }
	};

	// ====================================================================
	namespace Renderer2DSortMode
	{
		enum Enum
		{
			RenderState,	// Batches are sorted by Render State  (optimal performance, but requires caller to handle Z ordering)
			ZOrder,			// Batches are sorted by Z order then call order
			CallOrder,		// Everything is rendered in the order of the Draw() calls
		};
	}

	// ============================================================================
	class Renderer2D
	{

	private:
		// Private Properties
		// ----------------------------

		static void* batchBuffer;
		static void* endOfTheBuffer;
		static UInt32 bufferSize;
		static Renderer2DBatchSet batches;

		static Shader* texturedShader;
		static Shader* nonTexturedShader;
		static Matrix2D worldTransform;
		static bool applyWorldTransform;

		friend struct Renderer2DBatch;

		// Hidden CTor
		Renderer2D() {}

		// Init & Shutdown from the Graphics class
		friend class Graphics;
		static void Init();
		static void Shutdown();

	public:

		// Renderer Settings Control
		// ====================================================

		// Sort mode used by the renderer
		static Renderer2DSortMode::Enum		SortMode;

		// Changes the shader being used for rendering.
		// NOTE:   Shaders are batched, so if you modify shader parameters across draw calls, make sure to flush the renderer
		static void SetTexturedShader(Shader* shader);
		static void SetNonTexturedShader(Shader* shader);

		static inline Shader* GetTexturedShader()				{ return texturedShader; }
		static inline Shader* GetNonTexturedShader()			{ return nonTexturedShader; }

		// Sets the world transform used for 2D Rendering
		//  NOTE: the world transform is applied immediately during a DrawXXX call, so you are free to modify it after doing drawing
		static void SetWorldTransform(const Matrix2D& world);

		static inline bool ShouldApplyWorldTransform()			{ return applyWorldTransform; }
		static inline const Matrix2D& GetWorldTransform()		{ return worldTransform; }

		// Batch Rendering methods
		// ====================================================

		// Flush all the batch data to the GPU
		static void Flush();

		// Gets a non-textured batch big enough for the given number of primitives
		static Renderer2DBatchP2C4 GetBatchP2C4(
			UInt32 numPrimitives,
			Renderer2DPrimitiveType::Enum primitiveType,
			GLuint textureId1,
			GLuint textureId2,
			BlendMode::Enum blendMode, 
			float z,
			Shader* shader
			);

		// Gets a batch of big enough for the given number of primitives
		static Renderer2DBatchP2T2C4 GetBatchP2T2C4(
			UInt32 numPrimitives,
			Renderer2DPrimitiveType::Enum primitiveType,
			GLuint textureId1,
			GLuint textureId2,
			BlendMode::Enum blendMode, 
			float z,
			Shader* shader
			);

		// Log utility to dump the full batch details
		static void LogBatches();

		// DrawTexturedQuad() - Textured Quad drawing
		// ====================================================

		// ------------------------------------
		// Draw Quad from Texture Rectangle
		//		src  = Source rectangle
		//		dest = Destination rectangle
		static void DrawTexturedQuad(
			GLuint glTextureId,
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX1, float destY1, float destX2, float destY2, 
			const Color& color = Color::WHITE,
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);

		// ------------------------------------
		// Draw 4-point polygon from Texture Rectangle
		//		src  = Source rectangle
		//		dest = 4-point destination shape
		static void DrawTexturedQuad(
			GLuint glTextureId, 
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX1, float destY1, float destX2, float destY2, 
			float destX3, float destY3, float destX4, float destY4, 
			const Color& color = Color::WHITE,
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);

		// ------------------------------------
		// Draw 4-point polygon from Texture Rectangle  - to a pre-fetched batch
		//		src  = Source rectangle
		//		dest = 4-point destination shape
		static void DrawTexturedQuad(
			Renderer2DBatchP2T2C4& batch,
			float srcX1, float srcY1, float srcX2, float srcY2,
			float destX1, float destY1, float destX2, float destY2, 
			float destX3, float destY3, float destX4, float destY4,
			const Color& color = Color::WHITE
			);

		// DrawImage() - Atlas Image drawing
		// ====================================================

		// ------------------------------------
		// Draw AtlasImage at given Position (with common transforms)
		static void DrawImage(
			AtlasImage *image,
			const Vector2& position, 
			const Color& color = Color::WHITE,
			float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f,
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);

		// ------------------------------------
		// Draw AtlasImage at given Position (with common transforms)   - into a pre-fetched batch
		static void DrawImage(
			Renderer2DBatchP2T2C4& batch,
			AtlasImage *image,
			const Vector2& position, 
			const Color& color = Color::WHITE,
			float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f
			);

		// ------------------------------------
		// Draw AtlasImage into given rectangle
		static void DrawImage(
			AtlasImage *image, 
			const Vector2& topLeft, const Vector2& bottomRight, 
			const Color& color = Color::WHITE,
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);

		// ------------------------------------
		// Draw part of atlas image scaled to fit destination rectangle
		static void DrawImage(
			AtlasImage *image, 
			const Rectangle2& sourceRect, const Rectangle2& destRect,
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);

		// DrawSprite() - Sprite Drawing!!
		// ==================================================

		static void DrawSprite(Sprite *sprite);

		// DrawText() - draws text to the screen using a given Font
		// ==================================================

		// Draw text at the given position
		static Vector2 DrawText(
			BMFont *font, 
			const wchar_t *text, 
			const Vector2& position, 
			const Color& color = Color::WHITE,
			const Vector2& scale = Vector2::ONE, 
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);

		// DrawTexturedPolyLine() - Draws textured line strips with width
		// ====================================================

		/*
		struct TexturedLineVertex
		{
			Vector2		Position;
			AtlasImage*	SegmentImage;		// This image will be applied between this point & the next point

			float startTextureOffset;			// Offset within the texture (0-1)
			float endTextureOffset;				// Offset within the texture (0-1)
		};

		// Draw Textured Line
		static void DrawTexturedPolyLine(
			TexturedLineVertex* vertices, 
			int numVertices, 
			float lineWidth,
			bool flipYOnInvertedX = true,
			const Color& color = Color::WHITE,
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);
		*/

		// 2D Primitive Rendering
		// ====================================================

		// Lines
		static void DrawLine(
			const Vector2& start, const Vector2& end, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);
		
		// Rectangles
		static void DrawRectangle(
			const Rectangle2& rectangle, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);
		static void DrawRectangleOutline(
			const Rectangle2& rectangle, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);
		
		// Diamonds
		static void DrawDiamond(
			const Rectangle2& rectangle, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);
		static void DrawDiamondOutline(
			const Rectangle2& rectangle, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);
		
		// Circles
		static void DrawCircle(
			const Circle2& circle, int segments = 16, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal,
			float z = 0
			);
		static void DrawCircleOutline(
			const Circle2& circle, int segments = 16, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);
		static void DrawTexturedCircle(
			GLuint glTextureId,
			const Circle2& circle, int segments = 16,
			const Vector2& topLeftTC = Vector2::ZERO, 
			const Vector2& bottomRightTC = Vector2::ONE,
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);

		// Polygons
		static void DrawPolygon(
			Vector2* vertices, int vertexCount, 
			const Color& color = Color::WHITE, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);
		static void DrawPolyline(
			Vector2* vertices, int vertexCount, 
			const Color& color = Color::WHITE, 
			bool closed = false, 
			BlendMode::Enum blendMode = BlendMode::Normal, 
			float z = 0
			);

	private:

		// Internal Methods
		// ----------------------------

		// Get a batch pointer for the given type of render data
		static Renderer2DBatchHeader* GetBatch(
			UInt32 requestedBatchSize,						// Size (in-bytes) of data that will be added to the batch
			UInt8 primitiveType, 
			UInt8 vertexType, 
			GLuint textureId1,
			GLuint textureId2,
			BlendMode::Enum blendMode, 
			float z,
			Shader* shader
			);

	};

	// ==========================================================================
	// Inline Implementations

	// *****************************************************************
	inline void Renderer2DBatchP2C4::AddVertex(GLfloat x, GLfloat y, UInt8 r, UInt8 g, UInt8 b, UInt8 a)
	{
		VertexBuffer->X = x;
		VertexBuffer->Y = y;
		VertexBuffer->R = r;
		VertexBuffer->G = g;
		VertexBuffer->B = b;
		VertexBuffer->A = a;
		VertexBuffer++;
	}

	// *****************************************************************
	inline void Renderer2DBatchP2T2C4::AddVertex(GLfloat x, GLfloat y, GLfloat tx, GLfloat ty, UInt8 r, UInt8 g, UInt8 b, UInt8 a)
	{
		VertexBuffer->X = x;
		VertexBuffer->Y = y;
		VertexBuffer->TX = tx;
		VertexBuffer->TY = ty;
		VertexBuffer->R = r;
		VertexBuffer->G = g;
		VertexBuffer->B = b;
		VertexBuffer->A = a;
		VertexBuffer++;
	}

} // namespace