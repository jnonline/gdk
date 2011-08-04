/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Circle2.h"

using namespace Gdk;

// instantiations of static constants
const Circle2 Circle2::ZERO(0.0f, 0.0f, 0.0f);

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Circle2::Circle2()
{
	Center = Vector2::ZERO;
	Radius = 0.0f;
}

// ***********************************************************************
Circle2::Circle2(float x, float y, float radius)
{
	Center.X = x;
	Center.Y = y;
	Radius = radius;
}

// ***********************************************************************
Circle2::Circle2(const Vector2& position, float radius)
{
	Center = position;
	Radius = radius;
}

// ***********************************************************************
Circle2::Circle2(const Circle2& input)
{
	Center = input.Center;
	Radius = input.Radius;
}

// ===================================================================================
// Utility Methods
// ===================================================================================

// ***********************************************************************
Vector2 Circle2::GetParametricPointInArea(float tAngle, float tRadius)
{
	// Return a point in the circle using the given parametric coordinates within circle space
	float theta = tAngle * Math::TWO_PI;
	float sinTheta = Math::Sin(theta);
	float cosTheta = Math::Cos(theta);
	float radius = tRadius * this->Radius;

	return Vector2(
		this->Center.X + cosTheta * radius, 
		this->Center.Y + sinTheta * radius
		);
}

// ***********************************************************************
Vector2 Circle2::GetParametricPointOnPerimeter(float tAngle)
{
	// Return a point on the perimeter of the circle using a parametric coordinate
	float theta = tAngle * Math::TWO_PI;
	float sinTheta = Math::Sin(theta);
	float cosTheta = Math::Cos(theta);

	return Vector2(
		this->Center.X + cosTheta * this->Radius, 
		this->Center.Y + sinTheta * this->Radius
		);
}


// ===================================================================================
// Encapsulation Methods
// ===================================================================================

// ***********************************************************************
Circle2 Circle2::FromPoints(size_t numPoints, const Vector2* points)
{
    if(numPoints == 0)
        return Circle2::ZERO;
    
    // Start with a 0 radius circle, centered on the first point.
    Circle2 result(points[0], 0.0f);
    
    // Loop through the rest of the points
    for(int pointIndex=1; pointIndex < numPoints; pointIndex++)
    {
        const Vector2& point = points[pointIndex];
        
        // Get the vector from the center of the circle, to this point
        Vector2 pointToCenter = result.Center - point;
        
        // If the point is already in the circle, move on to the next point
        float lenSquared = pointToCenter.LengthSquared();
        if(lenSquared < (result.Radius * result.Radius))
            continue;
        
        // Calculate the edge of the circle if we extended it to encapsulate the point
        Vector2 circleEdge = pointToCenter;
        circleEdge.Normalize();
        circleEdge *= result.Radius;
        circleEdge += result.Center;
        
        // Calculate a new center for the circle, halfway between the edge of the circle & this point
        result.Center = (point + circleEdge) * 0.5f;
        result.Radius = (point - circleEdge).Length() * 0.5f;
    }
    
    return result;
}

// ***********************************************************************
Circle2 Circle2::Merge(const Circle2 &c1, const Circle2 &c2)
{
	// Get the difference vector between the centers
	Vector2 diff = c2.Center - c1.Center;
	float lenSquared = diff.LengthSquared();
	float radiusDiff = c2.Radius - c1.Radius;
	float radiusDiffSqr = radiusDiff * radiusDiff;
    
	// Does one circle already encapsulate the other?
	if(radiusDiffSqr >= lenSquared)
	{
		// Return the bigger (Encapsulating) circle
		if(radiusDiff >= 0)
			return c2;
		else
			return c1;
	}
    
	// Get the actual length of the difference of circle centers
	float len = Math::Sqrt(lenSquared);
	Circle2 result;
    
	// Are the circles on different centers?
	if(len > Math::ZERO_TOLERANCE)
	{
		// Calculate the new center of the encapsulating circle
		float coeff = (len + radiusDiff) / (2.0f * len);
		result.Center = c1.Center + coeff * diff;
	}
	else
	{
		result.Center = c1.Center;
	}
    
	// Set the new radius
	result.Radius = 0.5f * (len + c1.Radius + c2.Radius);
	
	return result;
}

// ***********************************************************************
Circle2 Circle2::CircumscribeTriangle(const Triangle2& triangle)
{
    return CircumscribeTriangle(triangle.Vertices[0], triangle.Vertices[1], triangle.Vertices[2]);
}

// ***********************************************************************
Circle2 Circle2::CircumscribeTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3)
{
    // Get the edge vectors
    Vector2 edge1 = p2 - p1;
    Vector2 edge2 = p3 - p1;
    
    // Build a 2x2 basis matrix 
    float basis[2][2] =
    {
        {edge1.X, edge1.Y},
        {edge2.X, edge2.Y}
    };
    
    // Calculate the determinate of the basis matrix
    float det = basis[0][0] * basis[1][1] - basis[0][1] * basis[1][0];
    if (Math::Abs(det) < Math::ZERO_TOLERANCE)
    {
        // The determinate is 0, this is an invalid basis (all points are co-linear).  
        //   So we fallback to just calculating a closest point fit
        Vector2 points[] = {p1, p2, p3};
        return Circle2::FromPoints(3, points);
    }
    
    // Build a 2x1 edge length vector
    float edgeLens[2] =
    {
        0.5f * edge1.LengthSquared(),
        0.5f * edge2.LengthSquared()
    };
    
    // Multiply the edge length vector through the basis matrix
    float invDet = 1.0f / det;
    Vector2 solution(
        (basis[1][1] * edgeLens[0] - basis[0][1] * edgeLens[1]) * invDet,
        (basis[0][0] * edgeLens[1] - basis[1][0] * edgeLens[0]) * invDet
        );
    
    // The linear solution is the circle radius & center offset (from p1)
    return Circle2(p1 + solution, solution.Length());
}

// ***********************************************************************
Circle2 Circle2::InscribeTriangle(const Triangle2& triangle)
{
    return InscribeTriangle(triangle.Vertices[0], triangle.Vertices[1], triangle.Vertices[2]);
}

// ***********************************************************************
Circle2 Circle2::InscribeTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3)
{
    // Get the 3 edge vectors
    Vector2 edge1 = p2 - p1;
    Vector2 edge2 = p3 - p1;
    Vector2 edge3 = p3 - p2;
    
    // Get the lengths of the edges
    float len1 = edge1.Length();
    float len2 = edge2.Length();
    float len3 = edge3.Length();
    
    // Calculate the total perimeter of the triangle
    float perimeter = len1 + len2 + len3;
    
    // If the triangle perimeter is ~0, then the 3 points are basically co-located. 
    //   So we return a radius 0 cicle centered on the points
    if(perimeter <= Math::ZERO_TOLERANCE)
        return Circle2(p1, 0.0f);
    
    // Get the edge lengths as fractions of the total perimeter
    float invPerimeter = 1.0f / perimeter;
    len1 *= invPerimeter;
    len2 *= invPerimeter;
    len3 *= invPerimeter;
    
    // Calculate the center & radius of the inscribed circle
    return Circle2(
        len3 * p1 + len2 * p2 + len1 * p3,
        invPerimeter * Math::Abs(edge1.Dot(edge2.GetPerpendicular()))
        );
}

// ***********************************************************************
Circle2 Circle2::CircumscribeRectangle(const Rectangle2& rect)
{
    return Circle2(rect.GetCenter(), rect.GetExtents().Length());
}



