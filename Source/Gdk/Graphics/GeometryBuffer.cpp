/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "GeometryBuffer.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///    Gets a display-friendly text string for the given primitive type
// *****************************************************************
const char* PrimitiveTypes::ToString(PrimitiveTypes::Enum primitiveType)
{
	static const char* points = "Points";
	static const char* lines = "Lines";
	static const char* triangles = "Triangles";
    static const char* indexedLines = "Indexed Lines";
    static const char* indexedTriangles = "Indexed Triangles";

	switch(primitiveType)
	{
		case PrimitiveTypes::Points:            return points;
		case PrimitiveTypes::Lines:             return lines;
		case PrimitiveTypes::Triangles:         return triangles;
        case PrimitiveTypes::IndexedLines:      return indexedLines;
        case PrimitiveTypes::IndexedTriangles:	return indexedTriangles;
	};
	
	return StringUtilities::Unknown;
}


// *****************************************************************
/// @brief
///    Creates a new geometry buffer with the given sized vertex & index buffers
/// @param initialVertexBufferSize
///     Initial size of the geometry vertex buffer (in bytes)
/// @param initialIndexBufferSize
///     Initial size of the geometry index buffer (in bytes)
// *****************************************************************
GeometryBuffer::GeometryBuffer(int initialVertexBufferSize, int initialIndexBufferSize)
{
	// Setup internal buffers
	// ----------------------------------------

	// Allocate the vertex buffer
	vertexBufferSize = initialVertexBufferSize;
	vertexBuffer = GdkAlloc(vertexBufferSize);
    vertexBufferCurrent = vertexBuffer;
	vertexBufferEnd = (UInt8*) vertexBuffer + vertexBufferSize;
    numVertices = 0;
    
    // Allocate the index buffer
	indexBufferSize = initialIndexBufferSize;
	indexBuffer = (UInt16*) GdkAlloc(indexBufferSize);
    indexBufferCurrent = indexBuffer;
	indexBufferEnd = (UInt16*)((UInt8*) indexBuffer + indexBufferSize);
    numIndices = 0;
    
    // Create a video memory VBO & IBO
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &indexBufferObject);
    Graphics::BindVertexBuffer(vertexBufferObject);
    Graphics::BindIndexBuffer(indexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_DYNAMIC_DRAW);

	// Set the default render state values
    currentVertexFormat = (VertexFormat::Enum) 0;
    currentPrimitiveType = PrimitiveTypes::Points;
}	

// *****************************************************************
/// @brief
///     Destroys the GeometryBuffer, freeing any used memory.
// *****************************************************************
GeometryBuffer::~GeometryBuffer()
{
    // Release the VBO & IBO
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &indexBufferObject);
    
	// Free the vertex buffer
	if(vertexBuffer != NULL)
		GdkFree(vertexBuffer);
	vertexBuffer = NULL;
    vertexBufferCurrent = NULL;
	vertexBufferEnd = NULL;
    numVertices = 0;
    
    // Free the index buffer
    if(indexBuffer != NULL)
		GdkFree(indexBuffer);
    indexBuffer = NULL;
    indexBufferCurrent = NULL;
	indexBufferEnd = NULL;
    numIndices = 0;
}

// *****************************************************************
/// @brief
///     Expands the capacity of the vertex buffer
// *****************************************************************
void GeometryBuffer::ExpandVertexBuffer()
{
    // Create a new buffer bigger than the original
    size_t newSize = vertexBufferSize * 2;
    void* newVertexBuffer = (UInt8*) GdkAlloc(newSize);
    
    // Copy the old buffer to the new one
    memcpy(newVertexBuffer, vertexBuffer, vertexBufferSize);
    
    // Setup the new end & current pointers
    vertexBufferCurrent = ((UInt8*)vertexBufferCurrent - (UInt8*)vertexBuffer) + (UInt8*) newVertexBuffer;
    vertexBufferEnd = (UInt8*) newVertexBuffer + newSize;
    
    // Release the old buffer
    GdkFree(vertexBuffer);
    
    // Switch to the new buffer
    vertexBuffer = newVertexBuffer;
    vertexBufferSize = newSize;
    
    // Expand the GPU VBO
    Graphics::BindVertexBuffer(vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_DYNAMIC_DRAW);
}

// *****************************************************************
/// @brief
///     Expands the capacity of the index buffer
// *****************************************************************
void GeometryBuffer::ExpandIndexBuffer()
{
    // Create a new buffer bigger than the original
    size_t newSize = indexBufferSize * 2;
    UInt16* newIndexBuffer = (UInt16*) GdkAlloc(newSize);
    
    // Copy the old buffer to the new one
    memcpy(newIndexBuffer, indexBuffer, indexBufferSize);
    
    // Setup the new end & current pointers
    indexBufferCurrent = (UInt16*) (((UInt8*)indexBufferCurrent - (UInt8*)indexBuffer) + (UInt8*)newIndexBuffer);
    indexBufferEnd = (UInt16*)((UInt8*) newIndexBuffer + newSize);
    
    // Release the old buffer
    GdkFree(indexBuffer);
    
    // Switch to the new buffer
    indexBuffer = newIndexBuffer;
    indexBufferSize = newSize;
    
    // Expand the GPU VBO
    Graphics::BindIndexBuffer(indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_DYNAMIC_DRAW);
}

// *****************************************************************
/// @brief
///     Gets the vertex format of the geometry
// *****************************************************************
VertexFormat::Enum GeometryBuffer::GetVertexFormat() const
{
    return currentVertexFormat;
}

// *****************************************************************
/// @brief
///     Gets the primitive type of the geometry
// *****************************************************************
PrimitiveTypes::Enum GeometryBuffer::GetPrimitiveType() const
{
    return currentPrimitiveType;
}

// *****************************************************************
/// @brief
///     Gets the number of vertices in the geometry buffer
// *****************************************************************
int GeometryBuffer::GetVertexCount() const
{
    return numVertices;
}

// *****************************************************************
/// @brief
///     Gets the number of indices in the geometry buffer
// *****************************************************************
int GeometryBuffer::GetIndexCount() const
{
    return numIndices;
}

// *****************************************************************
/// @brief
///     Gets the number of primitives in the geometry buffer.
/// @remarks
///     This value is derived from the primitive type and number of vertices/indices in the buffer
// *****************************************************************
int GeometryBuffer::GetPrimitiveCount() const
{
    switch(currentPrimitiveType)
    {
        case PrimitiveTypes::Points:            return numVertices;
        case PrimitiveTypes::Lines:             return numVertices / 2;
        case PrimitiveTypes::Triangles:         return numVertices / 3;
        case PrimitiveTypes::IndexedLines:      return numIndices / 2;
        case PrimitiveTypes::IndexedTriangles:  return numIndices / 3;
    }
    return 0;
}

// *****************************************************************
/// @brief
///     Gets a pointer to the internal vertex data buffer 
// *****************************************************************
void* GeometryBuffer::GetVertexDataPtr() const
{
    return vertexBuffer;
}

// *****************************************************************
/// @brief
///     Gets a pointer to the internal vertex data buffer 
// *****************************************************************
UInt16* GeometryBuffer::GetIndexDataPtr() const
{
    return indexBuffer;
}


// *****************************************************************
/// @brief
///     Sets the vertex format & primitive type of the geometry data that will be stored in the buffer.
/// @param vertexFormat
///     The format of the vertices that will be stored in the buffer.  (Ex:  VertexP2T2C4::Format)
/// @param primitiveType
///     The type of primitives that are stored in the geometry buffer
/// @note
///     This will also Clear() any geometry currently in the buffer
// *****************************************************************
void GeometryBuffer::Setup(VertexFormat::Enum vertexFormat, PrimitiveTypes::Enum primitiveType)
{
    // Set the new vertex format & primitive type
    currentVertexFormat = vertexFormat;
    currentPrimitiveType = primitiveType;
    
    // Clear the buffer
    Clear();
}

// *****************************************************************
/// @brief
///     Clears all the geometry currently in the buffer
// *****************************************************************
void GeometryBuffer::Clear()
{
    // Reset the buffers & counters
    vertexBufferCurrent = vertexBuffer;
    indexBufferCurrent = indexBuffer;
    numVertices = 0;
    numIndices = 0;
    
    // A reset means the gpu buffers need to be re-updated
    bufferDataIsUpToDate = false;
}

// *****************************************************************
/// @brief
///     Binds the geometry buffer data to a VBO in gpu memory and 
///     configures the vertex attribute bindinsgs for the VBO
// *****************************************************************
void GeometryBuffer::Bind()
{
    // Setup GPU Buffers
    // ------------------------------
    
    size_t vertexStride = VertexFormat::GetStride(currentVertexFormat);
    
    // Bind the GPU buffers
    Graphics::BindVertexBuffer(vertexBufferObject);
    Graphics::BindIndexBuffer(indexBufferObject);
    
    // Do we need to update the GPU buffers?
    if(bufferDataIsUpToDate == false)
    {
        // Do we have any vertices?
        if(numVertices > 0)
        {
            // Copy the vertex data to the GPU VBO
            size_t vertexDataSize = numVertices * vertexStride;
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataSize, vertexBuffer);
        }
        
        // Do we have any indices?
        if(numIndices > 0)
        {
            // Copy the index data to the GPU IBO
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndices * 2, indexBuffer);
        }
        
        bufferDataIsUpToDate = true;
    }
    
    // Setup Vertex Attributes
    // ----------------------------
    
    // Setup the attribute arrays based on pre-defined vertex formats  (faster)
    if(currentVertexFormat == VertexP2C4::Format)
    {
        // Enable 2 attribute arrays
        Graphics::EnableVertexAttribArrays(2);
        
        // Setup the attribute array pointers
        GLuint stride = sizeof(VertexP2C4);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)(sizeof(GLfloat[2])) );
    }
    else if(currentVertexFormat == VertexP2T2C4::Format)
    {
        // Enable 3 attribute arrays
        Graphics::EnableVertexAttribArrays(3);
        
        // Setup the attribute array pointers
        GLuint stride = sizeof(VertexP2T2C4);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat[2])));
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)(sizeof(GLfloat[4])) );
    }	
    else
    {
        // This is a custom vertex format, so we use a dynamic setup
        
        // Calculate the stride
        UInt8* vb = 0;
        int attIndex = 0;
        
        // Setup the Position attributes
        if(currentVertexFormat & VertexFormat::Position2)
        {
            glVertexAttribPointer(attIndex, 2, GL_FLOAT, GL_FALSE, vertexStride, vb);
            vb += sizeof(float[2]);
            attIndex++;
        }
        if(currentVertexFormat & VertexFormat::Position3)
        {
            glVertexAttribPointer(attIndex, 3, GL_FLOAT, GL_FALSE, vertexStride, vb);
            vb += sizeof(float[3]);
            attIndex++;
        }
        
        // Setup the Normal attributes
        if(currentVertexFormat & VertexFormat::Normal2)
        {
            glVertexAttribPointer(attIndex, 2, GL_FLOAT, GL_FALSE, vertexStride, vb);
            vb += sizeof(float[2]);
            attIndex++;
        }
        else if(currentVertexFormat & VertexFormat::Normal3)
        {
            glVertexAttribPointer(attIndex, 3, GL_FLOAT, GL_FALSE, vertexStride, vb);
            vb += sizeof(float[3]);
            attIndex++;
        }
        
        // Setup the Texture Coordinates attributes
        if(currentVertexFormat & VertexFormat::TexCoords2)
        {
            glVertexAttribPointer(attIndex, 2, GL_FLOAT, GL_FALSE, vertexStride, vb);
            vb += sizeof(float[2]);
            attIndex++;
        }
        else if(currentVertexFormat & VertexFormat::TexCoords2B)
        {
            glVertexAttribPointer(attIndex, 2, GL_FLOAT, GL_FALSE, vertexStride, vb);
            vb += sizeof(float[2]);
            attIndex++;
        }
        
        // Setup the Color attributes
        if(currentVertexFormat & VertexFormat::Color4)
        {
            glVertexAttribPointer(attIndex, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, vb);
            vb += sizeof(UInt8[4]);
            attIndex++;
        }
        
        // Enable the number of vertex attribute arrays we used
        Graphics::EnableVertexAttribArrays(attIndex);
    }

}

    
// *****************************************************************
/// @brief
///     Submits the geometry in the buffer to be drawn.
// *****************************************************************
void GeometryBuffer::Draw()
{
    // Do the drawing based on the PrimitiveType
    switch(currentPrimitiveType)
    {
        case PrimitiveTypes::Points:
            // Draw the points
            glDrawArrays(GL_POINTS, 0, numVertices);
            break;
            
        case PrimitiveTypes::Lines:
            // Draw the lines
            glDrawArrays(GL_LINES, 0, numVertices);
            break;

        case PrimitiveTypes::Triangles:
            // Draw the triangles
            glDrawArrays(GL_TRIANGLES, 0, numVertices);
            break;
            
        case PrimitiveTypes::IndexedLines:
            // Use the index buffer to draw the quads
            glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_SHORT, 0);
            break;
            
        case PrimitiveTypes::IndexedTriangles:
            // Use the index buffer to draw the quads
            glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
            break;
        
    }
}

