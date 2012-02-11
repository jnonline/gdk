/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


namespace Gdk
{
    /// @addtogroup Graphics
    /// @{ 
    /// @addtogroup _2D
    /// @{ 
    
    // =================================================================================
    ///	@brief
    ///		Contains a batch of geometry to be rendered with specific render state settings.
    // =================================================================================
    class Drawing2DBatch
    {
    public:
        
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Properties
        /// @{
        
        /// ID of the GL texture bound to texture unit 1
        GLuint TextureID1;
        
        /// ID of the GL texture bound to texture unit 1
        GLuint TextureID2;
        
        /// Blend mode used by this batch
        BlendMode::Enum BlendMode;

        /// Format of the vertex data in this batch
        VertexFormat::Enum VertexFormat;

        /// Type of the primitives in this batch
        PrimitiveTypes::Enum PrimitiveType;
        
        /// Geometry buffer containing the vertex/index/primitive data for this batch
        GeometryBuffer* Geometry;
        
        /// @}
    };
    
    // ====================================================================
    ///	@brief
    ///		Provides static methods for doing simple 2D drawing.
    /// @remarks
    ///     This class provides static methods for drawing high-level 2d primitives
    ///     such as lines, triangles, circles, rectangles, polygons, textured quads
    ///     images, text, etc...
    // =================================================================================
	class Drawing2D
	{

	public:

        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Control Methods
        /// @{
        
        static Matrix2D WorldTransform;
                                        
        static void SetTexturedShader(Shader* shader);
        static Shader* GetTexturedShader();
        
        static void SetNonTexturedShader(Shader* shader);
        static Shader* GetNonTexturedShader();
        
        static Drawing2DBatch* GetBatch(
                                        GLuint textureID1,
                                        GLuint textureID2,
                                        BlendMode::Enum blendMode,
                                        VertexFormat::Enum vertexFormat,
                                        PrimitiveTypes::Enum primitiveType
                                        );
        
        static void Flush();
        
        /// @}
        // ---------------------------------
        /// @name Basic 2D Primitives
        /// @{
        
        // Points
        // ------------
        
        static void DrawPoint(
            const Vector2& point,
            const Color& color = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
		// Lines
        // ------------
        
        static void DrawLine(
            const Vector2& v0, const Vector2& v1,
            const Color& color0 = Color::WHITE, const Color& color1 = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        static void DrawTexturedLine(
            GLuint glTextureId,
            const Vector2& v0, const Vector2& v1,
            const Vector2& tc0, const Vector2& tc1,
            const Color& color0 = Color::WHITE, const Color& color1 = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );

        // Triangles
        // ------------
        
        static void DrawTriangle(
            const Vector2& v0, const Vector2& v1, const Vector2& v2,
            const Color& color0 = Color::WHITE, const Color& color1 = Color::WHITE, const Color& color2 = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );        
        static void DrawTexturedTriangle(
            GLuint glTextureId,
            const Vector2& v0, const Vector2& v1, const Vector2& v2,
            const Vector2& tc0, const Vector2& tc1, const Vector2& tc2,
            const Color& color0 = Color::WHITE, const Color& color1 = Color::WHITE, const Color& color2 = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        // Quads
        // ------------
        
        static void DrawTexturedQuad(
            GLuint glTextureId, 
            float destX0, float destY0, float destX1, float destY1,
            float destX2, float destY2, float destX3, float destY3,
            float tcLeft, float tcTop, float tcRight, float tcBottom,
            const Color& color = Color::WHITE, 
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        /// @}
        // ---------------------------------
        /// @name Compound 2D Primitives
        /// @{
        
		// Rectangles
        // ------------
        
        static void DrawRectangle(
            const Rectangle2& rectangle, 
            const Color& color = Color::WHITE, 
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        static void DrawRectangleOutline(
            const Rectangle2& rectangle, 
            const Color& color = Color::WHITE, 
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        static void DrawTexturedRectangle(
            GLuint glTextureId,
            const Rectangle2& rectangle, 
            const Vector2& topLeftTC = Vector2::ZERO, 
            const Vector2& bottomRightTC = Vector2::ONE,
            const Color& color = Color::WHITE, 
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        // Circles
        // ------------
        
        static void DrawCircle(
            const Circle2& circle, 
            int segments = 16, 
            const Color& color = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        static void DrawCircleOutline(
            const Circle2& circle, 
            int segments = 16, 
            const Color& color = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
		static void DrawTexturedCircle(
            GLuint glTextureId,
            const Circle2& circle, 
            int segments = 16, 
            const Vector2& topLeftTC = Vector2::ZERO, 
            const Vector2& bottomRightTC = Vector2::ONE,
            const Color& color = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        // Polygons
        // ------------
        
        static void DrawPolygon(
            Vector2* vertices,
            int numVertices,
            const Color& color = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
            );
        
        static void DrawPolyline(
            Vector2* vertices,
            int numVertices,
            const Color& color = Color::WHITE,
            bool closed = true,
            BlendMode::Enum blendMode = BlendMode::Normal
            );

        
        /// @}
        // ---------------------------------
        /// @name AtlasImage Drawing Methods
        /// @{
                
        
        // -----
		static void DrawAtlasImage(
            AtlasImage *image, 
            const Vector2& position, 
            const Color& color = Color::WHITE,
            float rotation = 0.0f, 
            float scaleX = 1.0f, 
            float scaleY = 1.0f, 
            BlendMode::Enum blendMode = BlendMode::Normal
			);

		static void DrawAtlasImage(
			AtlasImage *image, 
			const Rectangle2& destination, 
			const Color& color = Color::WHITE,
            BlendMode::Enum blendMode = BlendMode::Normal
			);

		static void DrawSprite(Sprite *sprite);

		/// @}
        // ---------------------------------
        /// @name Text Drawing Methods
        /// @{

		static Vector2 DrawText(
			BMFont *font, 
			const char *text, 
			const Vector2& position, 
			const Color& color = Color::WHITE,
			const Vector2& scale = Vector2::ONE,
            BlendMode::Enum blendMode = BlendMode::Normal
			);
        
        /// @}

    private:
        
		// Private Properties
        // =======================================        
        
		static Drawing2DBatch* batch;
        static Shader* texturedShader;
        static Shader* nonTexturedShader;
		
		// Hidden CTor
		Drawing2D() {}
        
		// Init & Shutdown from the Graphics class
		friend class Graphics;
		static void Init();
		static void Shutdown();

	};
    
    /// @} // _2D
    /// @} // Graphics

} // namespace