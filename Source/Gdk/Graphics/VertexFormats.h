/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Math/Maths.h"

namespace Gdk
{
    // ============================================================================
    namespace VertexFormat
    {
        enum Enum
        {
            /// 2D Position (x,y)
            Position2 = 0x0001,
            
            /// 3D Position (x,y,z)
            Position3 = 0x0002,
            
            /// 2D Normal (x,y)
            Normal2 = 0x0004,
            
            /// 3D Normal (x,y,z)
            Normal3 = 0x0008,
            
            /// 2D Texture Coordinates (u,v)   [Set A]
            TexCoords2 = 0x0010,
            
            /// 2D Texture Coordinates (u,v)   [Set B]
            TexCoords2B = 0x0020,
            
            // 0x0040
            // 0x0080
            
            /// 4-part RGBA Color
            Color4 = 0x0100,
        };
        
        size_t GetStride(VertexFormat::Enum format);
    }
    
	// ============================================================================
	struct VertexP2T2C4
	{
		GLfloat X, Y;
		GLfloat TX, TY;
		GLubyte R, G, B, A;

        // ******************************************************
        VertexP2T2C4() {}
        
        // ******************************************************
        VertexP2T2C4(float x, float y, float tx, float ty, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
            : X(x), Y(y), TX(tx), TY(ty), R(r), G(g), B(b), A(a)
        {
        }
        
		// ******************************************************
		inline static void AppendToBuffer(void*& buffer, GLfloat x, GLfloat y, GLfloat tx, GLfloat ty, UInt8 r, UInt8 g, UInt8 b, UInt8 a)
		{
			VertexP2T2C4* vertex = (VertexP2T2C4*) buffer;
			vertex->X = x;
			vertex->Y = y;
			vertex->TX = tx;
			vertex->TY = ty;
			vertex->R = r;
			vertex->G = g;
			vertex->B = b;
			vertex->A = a;
			buffer = (void*)(vertex+1);
		}
        
        // *******************************************************
        static const VertexFormat::Enum Format = (VertexFormat::Enum)
            (
            VertexFormat::Position2 | 
            VertexFormat::TexCoords2 | 
            VertexFormat::Color4
            );
	};

	// ============================================================================
	struct VertexP2C4
	{
		GLfloat X, Y;
		GLubyte R, G, B, A;

        // ******************************************************
        VertexP2C4() {}
        
        // ******************************************************
        VertexP2C4(float x, float y, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
            : X(x), Y(y), R(r), G(g), B(b), A(a)
        {
        }
        
		// ******************************************************
		inline static void AppendToBuffer(void*& buffer, GLfloat x, GLfloat y, UInt8 r, UInt8 g, UInt8 b, UInt8 a)
		{
			VertexP2C4* vertex = (VertexP2C4*) buffer;
			vertex->X = x;
			vertex->Y = y;
			vertex->R = r;
			vertex->G = g;
			vertex->B = b;
			vertex->A = a;
			buffer = (void*)(vertex+1);
		}
        
        // *******************************************************
        static const VertexFormat::Enum Format = (VertexFormat::Enum)
            (
            VertexFormat::Position2 | 
            VertexFormat::Color4
            );
	};

	/*
	// ============================================================================
	struct VertexP3N3T2
	{
		GLfloat X, Y, Z;
		GLfloat NX, NY, NZ;
		GLfloat TX, TY;
	};

	// ============================================================================
	struct VertexP3N3
	{
		GLfloat X, Y, Z;
		GLfloat NX, NY, NZ;
	};
	*/

} // namespace Gdk
