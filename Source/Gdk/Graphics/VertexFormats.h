
#pragma once

#include "GdkLIB.h"
#include "../Math/Maths.h"

namespace Gdk
{
	// ============================================================================
	struct VertexP2T2C4
	{
		GLfloat X, Y;
		GLfloat TX, TY;
		GLubyte R, G, B, A;

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
	};

	// ============================================================================
	struct VertexP2C4
	{
		GLfloat X, Y;
		GLubyte R, G, B, A;

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
	};

	// ============================================================================
	struct VertexP3N3T2
	{
		GLfloat X, Y, Z;
		GLfloat NX, NY, NZ;
		GLfloat TX, TY;
	};

} // namespace Gdk
