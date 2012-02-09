/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "VertexFormats.h"

using namespace Gdk;

// ***********************************************************************
size_t VertexFormat::GetStride(VertexFormat::Enum format)
{
    size_t size = 0;
    
    // Position
    if(format & VertexFormat::Position2)
        size += sizeof(float[2]);
    if(format & VertexFormat::Position3)
        size += sizeof(float[3]);
    
    // Normal
    if(format & VertexFormat::Normal2)
        size += sizeof(float[2]);
    if(format & VertexFormat::Normal3)
        size += sizeof(float[3]);
    
    // Tex Coord
    if(format & VertexFormat::TexCoords2)
        size += sizeof(float[2]);
    if(format & VertexFormat::TexCoords2B)
        size += sizeof(float[2]);
     
    // Color
    if(format & VertexFormat::Color4)
        size += sizeof(UInt8[4]);
    
    return size;
}
