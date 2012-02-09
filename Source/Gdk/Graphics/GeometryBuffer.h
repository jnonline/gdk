/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


namespace Gdk
{
    /// @addtogroup Graphics
    /// @{    
    
    // ====================================================================
    ///	@brief
    ///		Types of primitives that can be rendered using the GeometryBuffer class
    // =================================================================================
	namespace PrimitiveTypes
	{
		enum Enum
		{
			Points,
			Lines,
			Triangles,
            IndexedLines,
            IndexedTriangles,
		};
        
		const char* ToString(PrimitiveTypes::Enum primitiveType);
	}
    
    // =================================================================================
    ///	@brief
    ///		Provides an auto-grow buffer for batching dynamic geometry data.
    /// @remarks
    ///     This class batches low-level rendering primitives such as 
    ///     points, lines, triangles, indexed lines, and indexed triangles.
    // =================================================================================
    class GeometryBuffer
	{
    public:
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Constructor / Destructor
        /// @{
        
        GeometryBuffer(int initialVertexBufferSize = 64000, int initialIndexBufferSize = 8000);
        ~GeometryBuffer();
        
        /// @}
        
        // ---------------------------------
        /// @name Control Methods
        /// @{
        
        VertexFormat::Enum GetVertexFormat() const;
        PrimitiveTypes::Enum GetPrimitiveType() const;
        int GetVertexCount() const;
        int GetIndexCount() const;
        int GetPrimitiveCount() const;
        
        void Setup(VertexFormat::Enum vertexFormat, PrimitiveTypes::Enum primitiveType);
        void Clear();
        
        void Draw();
        void Bind();
        
        /// @}
        
        // ---------------------------------
        /// @name Vertex & Index Creation Methods
        /// @{
        
        // *****************************************************************
        /// @brief
        ///     Adds a vertex to the geometry buffer
        /// @param TVertex
        ///     The type of the vertex that is to be added.  
        ///     Must match the vertex format passed in the most recent call to Setup()
        /// @param v
        ///     The vertex to add to the buffer
        /// @return
        ///     Returns the index of the vertex within the buffer.
        /// @remarks
        ///     This method appends a vertex to the geometry buffer.
        // *****************************************************************
        template <typename TVertex>
        inline UInt16 AddVertex(const TVertex& v)
        {
            // DEBUG CHECK:  make sure the vertex type matches the current VertexFormat
            ASSERT(currentVertexFormat == TVertex::Format, 
                   L"AddVertex called with an unexpected vertex format. [Current Format: %d][Submitted Format: %d]", 
                   currentVertexFormat, 
                   TVertex::Format
                   );
            // DEBUG CHECK:  make sure we dont draw too many vertices
            ASSERT(numVertices < 65535,
                   L"AddVertex called when the GeometryBuffer is full."
                   );
            
            // Is there not enough room for 1 more vertex?
            TVertex* temp = (TVertex*)vertexBufferCurrent;
            if((temp + 1) >= vertexBufferEnd)
            {
                // Grow the buffer
                ExpandVertexBuffer();
                temp = (TVertex*)vertexBufferCurrent;
            }
            
            // Append the vertex data to the current position in the buffer
            memcpy(temp, &v, sizeof(TVertex));
            
            // Increment the number of vertices
            numVertices++;
            
            // Move the buffer position forward
            vertexBufferCurrent = temp+1;
            
            // Return the index of the vertex we added
            return numVertices - 1;
        }
        
        // *****************************************************************
        /// @brief
        ///     Adds an Index to the geometry buffer
        /// @param index
        ///     The index value to add to the index buffer
        // *****************************************************************
        inline void AddIndex(UInt16 index)
        {
            // Is there not enough room for 1 more index?
            if((indexBufferCurrent + 1) >= indexBufferEnd)
            {
                // Grow the index buffer
                ExpandIndexBuffer();
            }
            
            // Assign the index at the current position in the index buffer
            *indexBufferCurrent = index;
            indexBufferCurrent++;
            
            // Increment the number of indices
            numIndices++;
        }

        // *****************************************************************
        /// @brief
        ///     Adds 4 vertices & 6 indices to the geometry buffer, which make up a quad
        /// @param TVertex
        ///     The type of the vertex that is to be added.  
        ///     Must match the vertex format passed in the most recent call to Setup()
        /// @param vertices
        ///     Pointer to 4 vertices that will be added to the buffer
        // *****************************************************************
        template <typename TVertex>
        inline void AddQuad(const TVertex* vertices)
        {
            // DEBUG CHECK:  make sure the primitive type is IndexedTriangles
            ASSERT(currentPrimitiveType == PrimitiveTypes::IndexedTriangles, 
                   L"AddQuad called when the primitive type is NOT IndexedTriangles. [Current Type: %d]", 
                   currentPrimitiveType
                   );
            // DEBUG CHECK:  make sure the vertex type matches the current VertexFormat
            ASSERT(currentVertexFormat == TVertex::Format, 
                   L"AddQuad called with an unexpected vertex format. [Current Format: %d][Submitted Format: %d]", 
                   currentVertexFormat, 
                   TVertex::Format
                   );
            // DEBUG CHECK:  make sure we dont draw too many vertices
            ASSERT(numVertices < 65532,
                   L"AddQuad called when the GeometryBuffer is full."
                   );
            
            // Is there not enough room for 4 more vertices?
            TVertex* temp = (TVertex*)vertexBufferCurrent;
            if((temp + 4) >= vertexBufferEnd)
            {
                // Grow the buffer
                ExpandVertexBuffer();
                temp = (TVertex*)vertexBufferCurrent;
            }
            
            // Is there not enough room for 6 more indices?
            if((indexBufferCurrent + 6) >= indexBufferEnd)
            {
                // Grow the index buffer
                ExpandIndexBuffer();
            }
            
            // Append the vertex data to the current position in the vertex buffer
            memcpy(temp, vertices, sizeof(TVertex[4]));
            
            // Assign the indices at the current position in the index buffer
            indexBufferCurrent[0] = numVertices;
            indexBufferCurrent[1] = numVertices+1;
            indexBufferCurrent[2] = numVertices+2;
            indexBufferCurrent[3] = numVertices+2;
            indexBufferCurrent[4] = numVertices+1;
            indexBufferCurrent[5] = numVertices+3;
            
            // Move the buffer positions forward
            vertexBufferCurrent = temp + 4;
            indexBufferCurrent += 6;
            
            // Increment the number of vertices & indices
            numVertices += 4;
            numIndices += 6;
        }
        
        /// @}
        
    private:
        
        // Private Properties
		// =====================================================
        
        // Vertex Data
        void* vertexBuffer;
		void* vertexBufferEnd;
        void* vertexBufferCurrent;
        UInt32 vertexBufferSize;
        UInt32 numVertices;
        GLuint vertexBufferObject;
        
        // Index Data
        UInt16* indexBuffer;
        UInt16* indexBufferEnd;
        UInt16* indexBufferCurrent;
        UInt32 indexBufferSize;
        UInt32 numIndices;
        GLuint indexBufferObject;
		
        // Current geometric data types
        PrimitiveTypes::Enum currentPrimitiveType;
        VertexFormat::Enum currentVertexFormat;
        
        // Intermediate state variables
        bool bufferDataIsUpToDate;
        
        // Private Methods
		// =====================================================
      
        void ExpandVertexBuffer();
        void ExpandIndexBuffer();
        
    };

    /// @}
    
} // namespace 
