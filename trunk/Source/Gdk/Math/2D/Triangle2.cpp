/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Triangle2.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Triangle2::Triangle2()
{
}

// *****************************************************************
/// @brief
///     Constructs a triangle from the given vertices
/// @param v1
///     Vertex 1 of the triangle
/// @param v2
///     Vertex 2 of the triangle
/// @param v3
///     Vertex 3 of the triangle
// *****************************************************************
Triangle2::Triangle2(const Vector2& v1, const Vector2& v2, const Vector2& v3)
{
	this->Vertices[0] = v1;
    this->Vertices[1] = v2;
    this->Vertices[2] = v3;
}

// *****************************************************************
/// @brief
///     Constructs a triangle from an array of vertices
/// @param vertices
///     An array of 3 vertices for the new triangle
// *****************************************************************
Triangle2::Triangle2(const Vector2* vertices)
{
	this->Vertices[0] = vertices[0];
    this->Vertices[1] = vertices[1];
    this->Vertices[2] = vertices[2];
}


// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Triangle2::Triangle2(const Triangle2& input)
{
	this->Vertices[0] = input.Vertices[0];
    this->Vertices[1] = input.Vertices[1];
    this->Vertices[2] = input.Vertices[2];
}
