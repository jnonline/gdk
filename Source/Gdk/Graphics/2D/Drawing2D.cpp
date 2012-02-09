/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Drawing2D.h"

using namespace Gdk;

// ********************************************************************************

// Static Instantiations
Drawing2DBatch* Drawing2D::batch = NULL;
Shader* Drawing2D::texturedShader = NULL;
Shader* Drawing2D::nonTexturedShader = NULL;

Matrix2D Drawing2D::WorldTransform = Matrix2D::IDENTITY;

// *****************************************************************
/// @brief
///     Initializes the static Drawing2D class
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Drawing2D::Init()
{
    // Create the drawing batch
    batch = GdkNew Drawing2DBatch();
    batch->TextureID1 = 0;
    batch->TextureID2 = 0;
    batch->BlendMode = BlendMode::Normal;
    batch->VertexFormat = VertexP2C4::Format;
    batch->PrimitiveType = PrimitiveTypes::Triangles;
    
    // Setup the batch's geometry buffer
    batch->Geometry = GdkNew GeometryBuffer(128000, 64000);
    batch->Geometry->Setup(batch->VertexFormat, batch->PrimitiveType);
    
    // Setup the shaders
    texturedShader = SharedResources::Shaders.Renderer2D.Textured;
    nonTexturedShader = SharedResources::Shaders.Renderer2D.NonTextured;
}	

// *****************************************************************
/// @brief
///     Shuts down the static Drawing2D class
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Drawing2D::Shutdown()
{
	// Free the batch
	if(batch != NULL)
    {
        if(batch->Geometry != NULL)
            GdkDelete(batch->Geometry);
		GdkDelete(batch);
    }
	batch = NULL;
}

// *****************************************************************
/// @brief
///     Sets the shader that is used when drawing textured primitives
/// @param shader
///     The shader to be used.  Passing NULL will reset the shader to 
///     SharedResources::Shaders.Drawing2D.Textured
/// @remarks
///     Shaders are batched, so if you modify shader parameters between draw calls, 
///     make sure to call Flush() first
// *****************************************************************
void Drawing2D::SetTexturedShader(Shader* shader)
{
	// If the user passed in NULL, we default to the global shared shader
	if(shader == NULL)
		texturedShader = SharedResources::Shaders.Renderer2D.Textured;
	else
		texturedShader = shader;
}

// *****************************************************************
/// @brief
///     Gets the shader that is used when drawing textured primitives
// *****************************************************************
Shader* Drawing2D::GetTexturedShader()
{
    return texturedShader;
}

// *****************************************************************
/// @brief
///     Sets the shader that is used when drawing Non-textured primitives
/// @param shader
///     The shader to be used.  Passing NULL will reset the shader to 
///     SharedResources::Shaders.Drawing2D.NonTextured
/// @remarks
///     Shaders are batched, so if you modify shader parameters between draw calls, 
///     make sure to call Flush() first
// *****************************************************************
void Drawing2D::SetNonTexturedShader(Shader* shader)
{
	// If the user passed in NULL, we default to the global shared shader
	if(shader == NULL)
		nonTexturedShader = SharedResources::Shaders.Renderer2D.NonTextured;
	else
		nonTexturedShader = shader;
}


// *****************************************************************
/// @brief
///     Gets the shader that is used when drawing Non-textured primitives
// *****************************************************************
Shader* Drawing2D::GetNonTexturedShader()
{
    return nonTexturedShader;
}

// *****************************************************************
/// @brief
///     Flushes any remaining drawing data in the render batch.
/// @remarks
///      Make sure to call this method when you are finished doing any Draw calls
// *****************************************************************
Drawing2DBatch*  Drawing2D::GetBatch(
    GLuint textureID1,
    GLuint textureID2,
    BlendMode::Enum blendMode,
    VertexFormat::Enum vertexFormat,
    PrimitiveTypes::Enum primitiveType
    )
{
    // Is the current batch using these render states?
    if(textureID1 == batch->TextureID1 && 
       textureID2 == batch->TextureID2 &&
       blendMode == batch->BlendMode &&
       vertexFormat == batch->VertexFormat &&
       primitiveType == batch->PrimitiveType)
    {
        // The current batch will work!
        return batch;
    }
    
    // Flush the current batch
    Flush();
    
    // Setup the batch for the new render state
    batch->TextureID1 = textureID1;
    batch->TextureID2 = textureID2;
    batch->BlendMode = blendMode;
    batch->VertexFormat = vertexFormat;
    batch->PrimitiveType = primitiveType;
    
    // Reset the batch's geometry buffer
    batch->Geometry->Setup(vertexFormat, primitiveType);
    
    return batch;
}

// *****************************************************************
/// @brief
///     Flushes any geometry & render data in the current batch.
/// @remarks
///     Make sure to call this method when you are finished doing any Draw calls
// *****************************************************************
void Drawing2D::Flush()
{
    // Bail early if there is no geometry to render
    if(batch->Geometry->GetVertexCount() == 0)
        return;
    
    // Disable depth write/testing & culling
	Graphics::EnableDepthTest(false);
	Graphics::EnableDepthWrite(false);
	Graphics::SetCullingMode(CullingMode::Disabled);
    
    // Setup blending
	Graphics::EnableBlending(true);
    Graphics::SetBlendMode(batch->BlendMode);
    
    // Bind textures (if necessary)
    if(batch->TextureID1 != 0)
        Graphics::BindTexture(batch->TextureID1, TextureUnit::Unit_0);
    if(batch->TextureID2 != 0)
        Graphics::BindTexture(batch->TextureID2, TextureUnit::Unit_1);
    
    // Determine which shader to use
    Shader* shader = nonTexturedShader;
    if(batch->VertexFormat & VertexFormat::TexCoords2)
        shader = texturedShader;
    
    // Apply the shader
    shader->Apply();
    
    // Draw the geometry
    batch->Geometry->Bind();
    batch->Geometry->Draw();
    batch->Geometry->Clear();
}

// #######################################################################################################################
//
// 2D Primitive Rendering
//
// #######################################################################################################################


// *****************************************************************
/// @brief
///     Draws a single point with a given color.
// *****************************************************************
void Drawing2D::DrawPoint(
                          const Vector2& point,
                          const Color& color,
                          BlendMode::Enum blendMode
                          )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::Points);
    
    // Transform the point by the world transform
    float x = point.X, y = point.Y;
    WorldTransform.TransformPointInPlace(x, y);
    
    // Add a vertex to the batch's Geometry
    batch->Geometry->AddVertex(VertexP2C4(x, y, color.R, color.G, color.B, color.A));
}

// ********************************************************************
void Drawing2D::DrawLine(
                         const Vector2& v0, const Vector2& v1,
                         const Color& color0, const Color& color1,
                         BlendMode::Enum blendMode
                         )
{
    // Lines are secretly drawn as triangles :D
    
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedLines);
    
    // Transform the points by the world transform
    float x0 = v0.X, y0 = v0.Y;
    float x1 = v1.X, y1 = v1.Y;
    WorldTransform.TransformPointInPlace(x0, y0);
    WorldTransform.TransformPointInPlace(x1, y1);
    
    // Add the vertices to the batch's Geometry
    UInt16 index = 
    batch->Geometry->AddVertex(VertexP2C4(x0, y0, color0.R, color0.G, color0.B, color0.A));
    batch->Geometry->AddVertex(VertexP2C4(x1, y1, color1.R, color1.G, color1.B, color1.A));
    
    // Add indices for the triangle
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+1);
}

// ********************************************************************
void Drawing2D::DrawTexturedLine(
                                 GLuint glTextureId,
                                 const Vector2& v0, const Vector2& v1,
                                 const Vector2& tc0, const Vector2& tc1,
                                 const Color& color0, const Color& color1,
                                 BlendMode::Enum blendMode
                                 )
{
    // Lines are secretly drawn as triangles :D
    
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(glTextureId, 0, blendMode, VertexP2T2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Transform the points by the world transform
    float x0 = v0.X, y0 = v0.Y;
    float x1 = v1.X, y1 = v1.Y;
    WorldTransform.TransformPointInPlace(x0, y0);
    WorldTransform.TransformPointInPlace(x1, y1);
    
    // Add a vertex to the batch's Geometry
    UInt16 index = 
    batch->Geometry->AddVertex(VertexP2T2C4(x0, y0, tc0.X, tc0.Y, color0.R, color0.G, color0.B, color0.A));
    batch->Geometry->AddVertex(VertexP2T2C4(x1, y1, tc1.X, tc1.Y, color1.R, color1.G, color1.B, color1.A));
    
    // Add indices for the triangle
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+1);
}


// ********************************************************************
void Drawing2D::DrawTriangle(
    const Vector2& v0, const Vector2& v1, const Vector2& v2,
    const Color& color0, const Color& color1, const Color& color2,
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Transform the points by the world transform
    float x0 = v0.X, y0 = v0.Y;
    float x1 = v1.X, y1 = v1.Y;
    float x2 = v2.X, y2 = v2.Y;
    WorldTransform.TransformPointInPlace(x0, y0);
    WorldTransform.TransformPointInPlace(x1, y1);
    WorldTransform.TransformPointInPlace(x2, y2);
    
    // Add a vertex to the batch's Geometry
    UInt16 index = 
    batch->Geometry->AddVertex(VertexP2C4(x0, y0, color0.R, color0.G, color0.B, color0.A));
    batch->Geometry->AddVertex(VertexP2C4(x1, y1, color1.R, color1.G, color1.B, color1.A));
    batch->Geometry->AddVertex(VertexP2C4(x2, y2, color2.R, color2.G, color2.B, color2.A));
    
    // Add indices for the triangle
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+1);
    batch->Geometry->AddIndex(index+2);
}

// ********************************************************************
void Drawing2D::DrawTexturedTriangle(
    GLuint glTextureId,
    const Vector2& v0, const Vector2& v1, const Vector2& v2,
    const Vector2& tc0, const Vector2& tc1, const Vector2& tc2,
    const Color& color0, const Color& color1, const Color& color2,
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(glTextureId, 0, blendMode, VertexP2T2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Transform the points by the world transform
    float x0 = v0.X, y0 = v0.Y;
    float x1 = v1.X, y1 = v1.Y;
    float x2 = v2.X, y2 = v2.Y;
    WorldTransform.TransformPointInPlace(x0, y0);
    WorldTransform.TransformPointInPlace(x1, y1);
    WorldTransform.TransformPointInPlace(x2, y2);
    
    // Add a vertex to the batch's Geometry
    UInt16 index = 
    batch->Geometry->AddVertex(VertexP2T2C4(x0, y0, tc0.X, tc0.Y, color0.R, color0.G, color0.B, color0.A));
    batch->Geometry->AddVertex(VertexP2T2C4(x1, y1, tc1.X, tc1.Y, color1.R, color1.G, color1.B, color1.A));
    batch->Geometry->AddVertex(VertexP2T2C4(x2, y2, tc2.X, tc2.Y, color2.R, color2.G, color2.B, color2.A));
    
    // Add indices for the triangle
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+1);
    batch->Geometry->AddIndex(index+2);
}

// ********************************************************************
void Drawing2D::DrawTexturedQuad(
    GLuint glTextureId, 
    float destX0, float destY0, float destX1, float destY1,
    float destX2, float destY2, float destX3, float destY3,
    float tcLeft, float tcTop, float tcRight, float tcBottom,
    const Color& color, 
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(glTextureId, 0, blendMode, VertexP2T2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Transform the points by the world transform
    WorldTransform.TransformPointInPlace(destX0, destY0);
    WorldTransform.TransformPointInPlace(destX1, destY1);
    WorldTransform.TransformPointInPlace(destX2, destY2);
    WorldTransform.TransformPointInPlace(destX3, destY3);
    
    // Create the 4 vertices of the quad
    VertexP2T2C4 verts[4] = 
    {
        VertexP2T2C4(destX0, destY0, tcLeft, tcTop, color.R, color.G, color.B, color.A),
        VertexP2T2C4(destX1, destY1, tcRight, tcTop, color.R, color.G, color.B, color.A),
        VertexP2T2C4(destX2, destY2, tcLeft, tcBottom, color.R, color.G, color.B, color.A),
        VertexP2T2C4(destX3, destY3, tcRight, tcBottom, color.R, color.G, color.B, color.A)
    };
    
    // Add the quad to the geometry buffer
    batch->Geometry->AddQuad(verts);
}

// ********************************************************************
void Drawing2D::DrawRectangle(
    const Rectangle2& rectangle, 
    const Color& color, 
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Get the TL, TR, BL, BR positions of the rectangle
    float x0 = rectangle.Position.X, y0 = rectangle.Position.Y;
    float x1 = rectangle.Position.X + rectangle.Size.X, y1 = rectangle.Position.Y;
    float x2 = rectangle.Position.X, y2 = rectangle.Position.Y + rectangle.Size.Y;
    float x3 = rectangle.Position.X + rectangle.Size.X, y3 = rectangle.Position.Y + rectangle.Size.Y;
	
    // Transform the points by the world transform
    WorldTransform.TransformPointInPlace(x0, y0);
    WorldTransform.TransformPointInPlace(x1, y1);
    WorldTransform.TransformPointInPlace(x2, y2);
    WorldTransform.TransformPointInPlace(x3, y3);
    
    // Add the 4 vertices of the geometry
    UInt16 index =
    batch->Geometry->AddVertex(VertexP2C4(x0, y0, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2C4(x1, y1, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2C4(x2, y2, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2C4(x3, y3, color.R, color.G, color.B, color.A));
    
    // Add indices for triangle:  TL-TR-BL
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+1);
    batch->Geometry->AddIndex(index+2);

    // Add indices for triangle:  BL-TR-BR
    batch->Geometry->AddIndex(index+2);
    batch->Geometry->AddIndex(index+1);
    batch->Geometry->AddIndex(index+3);
}

// ********************************************************************
void Drawing2D::DrawRectangleOutline(
    const Rectangle2& rectangle, 
    const Color& color, 
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedLines);
    
    // Get the TL, TR, BL, BR positions of the rectangle
    float x0 = rectangle.Position.X, y0 = rectangle.Position.Y;
    float x1 = rectangle.Position.X + rectangle.Size.X, y1 = rectangle.Position.Y;
    float x2 = rectangle.Position.X, y2 = rectangle.Position.Y + rectangle.Size.Y;
    float x3 = rectangle.Position.X + rectangle.Size.X, y3 = rectangle.Position.Y + rectangle.Size.Y;
	
    // Transform the points by the world transform
    WorldTransform.TransformPointInPlace(x0, y0);
    WorldTransform.TransformPointInPlace(x1, y1);
    WorldTransform.TransformPointInPlace(x2, y2);
    WorldTransform.TransformPointInPlace(x3, y3);
    
    // Add the 4 vertices of the geometry
    UInt16 index =
    batch->Geometry->AddVertex(VertexP2C4(x0, y0, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2C4(x1, y1, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2C4(x2, y2, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2C4(x3, y3, color.R, color.G, color.B, color.A));
    
    // Add indices for side: Left
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+2);
    
    // Add indices for side: Right
    batch->Geometry->AddIndex(index+1);
    batch->Geometry->AddIndex(index+3);
    
    // Add indices for side: Top
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+1);
    
    // Add indices for side: Bottom
    batch->Geometry->AddIndex(index+2);
    batch->Geometry->AddIndex(index+3);
}


// ********************************************************************
void Drawing2D::DrawTexturedRectangle(
    GLuint glTextureId,
    const Rectangle2& rectangle, 
    const Vector2& topLeftTC, 
    const Vector2& bottomRightTC,
    const Color& color, 
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(glTextureId, 0, blendMode, VertexP2T2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Get the TL, TR, BL, BR positions of the rectangle
    float x0 = rectangle.Position.X, y0 = rectangle.Position.Y;
    float x1 = rectangle.Position.X + rectangle.Size.X, y1 = rectangle.Position.Y;
    float x2 = rectangle.Position.X, y2 = rectangle.Position.Y + rectangle.Size.Y;
    float x3 = rectangle.Position.X + rectangle.Size.X, y3 = rectangle.Position.Y + rectangle.Size.Y;
	
    // Transform the points by the world transform
    WorldTransform.TransformPointInPlace(x0, y0);
    WorldTransform.TransformPointInPlace(x1, y1);
    WorldTransform.TransformPointInPlace(x2, y2);
    WorldTransform.TransformPointInPlace(x3, y3);
    
    // Add the 4 vertices of the geometry
    UInt16 index =
    batch->Geometry->AddVertex(VertexP2T2C4(x0, y0, topLeftTC.X, topLeftTC.Y, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2T2C4(x1, y1, bottomRightTC.X, topLeftTC.Y, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2T2C4(x2, y2, topLeftTC.X, bottomRightTC.Y, color.R, color.G, color.B, color.A));
    batch->Geometry->AddVertex(VertexP2T2C4(x3, y3, bottomRightTC.X, bottomRightTC.Y, color.R, color.G, color.B, color.A));
    
    // Add indices for triangle:  TL-TR-BL
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index+1);
    batch->Geometry->AddIndex(index+2);
    
    // Add indices for triangle:  BL-TR-BR
    batch->Geometry->AddIndex(index+2);
    batch->Geometry->AddIndex(index+1);
    batch->Geometry->AddIndex(index+3);
}

// ********************************************************************
void Drawing2D::DrawCircle(
    const Circle2& circle, 
    int segments, 
    const Color& color, 
    BlendMode::Enum blendMode
    )
{
	// Calculate the amount of theta per segment
	float thetaPerSegment = Math::TWO_PI / (float)segments;
    
	// Get the transformed center
	Vector2 center = circle.Center;
	WorldTransform.TransformPointInPlace(center.X, center.Y);
        
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Add the center vertex
    UInt16 index = 
    batch->Geometry->AddVertex(VertexP2C4(center.X, center.Y, color.R, color.G, color.B, color.A));
	
	// Loop through the points on the circle
	for(int i = 0; i < segments; i++)
	{
		// Calculate the position at the end of the segment
		float theta = thetaPerSegment * i;
		float x = Math::Cos(theta) * circle.Radius + circle.Center.X;
		float y = Math::Sin(theta) * circle.Radius + circle.Center.Y;
        
		// Transform the vertex using the world transform
        WorldTransform.TransformPointInPlace(x, y);
		
        // Add this vertex
        batch->Geometry->AddVertex(VertexP2C4(x, y, color.R, color.G, color.B, color.A));
    }
    
    // Loop through all the segments except the last
    for(int i = 1; i < segments; i++)
    {
        // Add a triangle for this segment
        batch->Geometry->AddIndex(index);
        batch->Geometry->AddIndex(index + i);
        batch->Geometry->AddIndex(index + i + 1);
    }
    
    // Add a triangle for the last segment
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index + segments);
    batch->Geometry->AddIndex(index + 1);
}

// ********************************************************************
void Drawing2D::DrawCircleOutline(
    const Circle2& circle, 
    int segments, 
    const Color& color, 
    BlendMode::Enum blendMode
    )
{
	// Calculate the amount of theta per segment
	float thetaPerSegment = Math::TWO_PI / (float)segments;
    
	// Get the transformed center
	Vector2 center = circle.Center;
	WorldTransform.TransformPointInPlace(center.X, center.Y);
    
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedLines);
    
    // The starting index is the first vertex on the segments
    UInt16 index = batch->Geometry->GetVertexCount();
    
	// Loop through the points on the circle
	for(int i = 0; i < segments; i++)
	{
		// Calculate the position at the end of the segment
		float theta = thetaPerSegment * i;
		float x = Math::Cos(theta) * circle.Radius + circle.Center.X;
		float y = Math::Sin(theta) * circle.Radius + circle.Center.Y;
        
		// Transform the vertex using the world transform
        WorldTransform.TransformPointInPlace(x, y);
		
        // Add this vertex
        batch->Geometry->AddVertex(VertexP2C4(x, y, color.R, color.G, color.B, color.A));
    }
    
    // Loop through all the segments except the last
    for(int i = 0; i < segments-1; i++)
    {
        // Add this segment line
        batch->Geometry->AddIndex(index + i);
        batch->Geometry->AddIndex(index + i + 1);
    }
    
    // Add the last segment line
    batch->Geometry->AddIndex(index + segments - 1);
    batch->Geometry->AddIndex(index);
}

// ********************************************************************
void Drawing2D::DrawTexturedCircle(
    GLuint glTextureId,
    const Circle2& circle, 
    int segments, 
    const Vector2& topLeftTC, 
    const Vector2& bottomRightTC,
    const Color& color,
    BlendMode::Enum blendMode
    )
{
	// Calculate the amount of theta per segment
	float thetaPerSegment = Math::TWO_PI / (float)segments;
    
    // Get the center texture coordinate & the x/y radii of the texture rectangle
    Vector2 centerTC = (topLeftTC + bottomRightTC) * 0.5f;
    float tcRadiusX = Math::Abs(centerTC.X - bottomRightTC.X);
    float tcRadiusY = Math::Abs(centerTC.Y - bottomRightTC.Y);
    
	// Get the transformed center
	Vector2 center = circle.Center;
	WorldTransform.TransformPointInPlace(center.X, center.Y);
    
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(glTextureId, 0, blendMode, VertexP2T2C4::Format, PrimitiveTypes::IndexedTriangles);
    UInt16 index;
    index = batch->Geometry->AddVertex(VertexP2T2C4(center.X, center.Y, centerTC.X, centerTC.Y, color.R, color.G, color.B, color.A));
    
	// Loop through the points on the circle
	for(int i = 0; i < segments; i++)
	{
		// Calculate the position at the end of the segment
		float theta = thetaPerSegment * i;
        float cosTheta = Math::Cos(theta);
        float sinTheta = Math::Sin(theta);
		float x = cosTheta * circle.Radius + circle.Center.X;
		float y = sinTheta * circle.Radius + circle.Center.Y;
        float tx = cosTheta * tcRadiusX + centerTC.X;
        float ty = sinTheta * tcRadiusY + centerTC.Y;
        
		// Transform the vertex using the world transform
        WorldTransform.TransformPointInPlace(x, y);
		
        // Add this vertex
        batch->Geometry->AddVertex(VertexP2T2C4(x, y, tx, ty, color.R, color.G, color.B, color.A));
    }
    
    // Loop through all the segments except the last
    for(int i = 1; i < segments; i++)
    {
        // Add a triangle for this segment
        batch->Geometry->AddIndex(index);
        batch->Geometry->AddIndex(index + i);
        batch->Geometry->AddIndex(index + i + 1);
    }
    
    // Add a triangle for the last segment
    batch->Geometry->AddIndex(index);
    batch->Geometry->AddIndex(index + segments);
    batch->Geometry->AddIndex(index + 1);
}

// ********************************************************************
void Drawing2D::DrawPolygon(
    Vector2* vertices,
    int numVertices,
    const Color& color,
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedTriangles);
        
    // Add the first vertex & get it's index
    UInt16 index = 
    batch->Geometry->AddVertex(VertexP2C4(vertices[0].X, vertices[0].Y, color.R, color.G, color.B, color.A));
    
    // Add all the other vertices
    for(int i=1; i<numVertices; i++)
    {
        batch->Geometry->AddVertex(VertexP2C4(vertices[i].X, vertices[i].Y, color.R, color.G, color.B, color.A));
	}
    
    // Add all the triangles for the polygon
    for(int i=1; i<numVertices-1; i++)
    {
        batch->Geometry->AddIndex(index);
        batch->Geometry->AddIndex(index+i);
        batch->Geometry->AddIndex(index+i+1);
    }
}

// ********************************************************************
void Drawing2D::DrawPolyline(
    Vector2* vertices,
    int numVertices,
    const Color& color,
    bool closed,
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing
    Drawing2DBatch* batch = GetBatch(0, 0, blendMode, VertexP2C4::Format, PrimitiveTypes::IndexedLines);
    
    // Add the first vertex & get it's index
    UInt16 index = batch->Geometry->AddVertex(VertexP2C4(vertices[0].X, vertices[0].Y, color.R, color.G, color.B, color.A));
    
    // Add all the other vertices
    for(int i=1; i<numVertices; i++)
    {
        batch->Geometry->AddVertex(VertexP2C4(vertices[i].X, vertices[i].Y, color.R, color.G, color.B, color.A));
	}
    
    // Add lines for each pair of vertices
    for(int i=0; i<numVertices-1; i++)
    {
        batch->Geometry->AddIndex(index+i);
        batch->Geometry->AddIndex(index+i+1);
    }
    
    // Add a closing line segment?
    if(closed)
    {
        batch->Geometry->AddIndex(index + numVertices-1);
        batch->Geometry->AddIndex(index);
    }
}


// ********************************************************************************
void Drawing2D::DrawAtlasImage(
    AtlasImage *image, 
    const Vector2& position, 
    const Color& color,
    float rotation, float scaleX, float scaleY, 
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing this image
    Drawing2DBatch* batch = GetBatch(image->Sheet->Texture->GLTextureId, 0, blendMode, 
                                     VertexP2T2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Get the quad for rendering this atlas image
    VertexP2T2C4 quadVertices[4];
    image->GetQuad(quadVertices, color);
    
	    
	// Apply scaling and/or rotation?
	if(rotation != 0.0f || scaleX != 1.0f || scaleY != 1.0f)
	{
		// Do a full scale/rotate/translate matrix
		Matrix2D matrix;
		matrix.ScaleRotateTranslate(scaleX, scaleY, position.X, position.Y, rotation);
		matrix.TransformPointInPlace(quadVertices[0].X, quadVertices[0].Y);
		matrix.TransformPointInPlace(quadVertices[1].X, quadVertices[1].Y);
		matrix.TransformPointInPlace(quadVertices[2].X, quadVertices[2].Y);
		matrix.TransformPointInPlace(quadVertices[3].X, quadVertices[3].Y);
	}
	else
	{
		// Just apply translation
		quadVertices[0].X += position.X; quadVertices[0].Y += position.Y;
        quadVertices[1].X += position.X; quadVertices[1].Y += position.Y;
        quadVertices[2].X += position.X; quadVertices[2].Y += position.Y;
        quadVertices[3].X += position.X; quadVertices[3].Y += position.Y;
	}
    
    // Transform the vertices through the world transform
    WorldTransform.TransformPointInPlace(quadVertices[0].X, quadVertices[0].Y);
    WorldTransform.TransformPointInPlace(quadVertices[1].X, quadVertices[1].Y);
    WorldTransform.TransformPointInPlace(quadVertices[2].X, quadVertices[2].Y);
    WorldTransform.TransformPointInPlace(quadVertices[3].X, quadVertices[3].Y);

    // Add the 4 vertices to the geometry buffer
    batch->Geometry->AddQuad(quadVertices);
}


// ********************************************************************************
void Drawing2D::DrawAtlasImage(
    AtlasImage *image, 
    const Rectangle2& destination,
    const Color& color, 
    BlendMode::Enum blendMode
    )
{
    // Get the batch for drawing this image
    Drawing2DBatch* batch = GetBatch(image->Sheet->Texture->GLTextureId, 0, blendMode, 
                                     VertexP2T2C4::Format, PrimitiveTypes::IndexedTriangles);
    
    // Get the quad for rendering this atlas image into the destination rectangle
    VertexP2T2C4 quadVertices[4];
    image->GetFittedQuad(quadVertices, destination, color);
    
    // Transform the vertices through the world transform
    WorldTransform.TransformPointInPlace(quadVertices[0].X, quadVertices[0].Y);
    WorldTransform.TransformPointInPlace(quadVertices[1].X, quadVertices[1].Y);
    WorldTransform.TransformPointInPlace(quadVertices[2].X, quadVertices[2].Y);
    WorldTransform.TransformPointInPlace(quadVertices[3].X, quadVertices[3].Y);
    
    // Add the 4 vertices to the geometry buffer
    batch->Geometry->AddQuad(quadVertices);
}

// ********************************************************************************
void Drawing2D::DrawSprite(Sprite* sprite)
{
	// Is the sprite visible?
	if(sprite->Visible)
	{
		// Get the current frame image
		AtlasImage *image = sprite->GetCurrentFrameImage();

		// Draw the atlas image with the sprite properties
		DrawAtlasImage(
			image,
			sprite->Position,
			sprite->Color,
			sprite->Rotation,
			sprite->Scale.X,
			sprite->Scale.Y,
			sprite->BlendMode
			);
	}
}

// ********************************************************************************
Vector2 Drawing2D::DrawText(
	BMFont *font, 
	const wchar_t *text, 
	const Vector2& position, 
	const Color& color, 
	const Vector2& scale, 
	BlendMode::Enum blendMode
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
                LOG_WARN(L"The character [%lc][%u] doesn't exist in the character map for the font [%hs]", ch, (UInt32)ch, font->GetName().c_str()); 
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
                DrawTexturedQuad(
                    texture->GLTextureId, 
                    dx1, dy1, dx2, dy1, dx1, dy2, dx2, dy2,
                    tx1, ty1, tx2, ty2, 
                    color, 
                    blendMode
                    );

                // Move the position to after the character
                drawPos.X += fontChar.XAdvance * scale.X;
            }
		}

		// next character
		current++;
	}

	return drawPos;
}


