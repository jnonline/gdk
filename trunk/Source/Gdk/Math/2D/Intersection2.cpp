/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Intersection2.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
Intersection2::Intersection2()
{
}


// ===================================================================================
// Circle -> Circle
// ===================================================================================

// *************************************************************************************
bool Intersection2::Test(const Circle2& circle1, const Circle2& circle2)
{
	// Get the distance (squared) between the two circle centers 
    float distanceSquared = (circle1.Center - circle2.Center).LengthSquared();
    
    // Get the total radii of the circles
    float totalRadii = circle1.Radius + circle2.Radius;
    
    // If the distance between the centers is less than the total radii, then the circles intersect
    return distanceSquared <= totalRadii * totalRadii;
}

// ===================================================================================
// Rectangle -> Rectangle
// ===================================================================================

// *************************************************************************************
bool Intersection2::Test(const Rectangle2& rect1, const Rectangle2& rect2)
{
    // Test if the rectangles intersect
    return rect1.Position.X <= (rect2.Position.X + rect2.Size.X)
        && rect1.Position.X + rect1.Size.X >= rect2.Position.X
        && rect1.Position.Y <= (rect2.Position.Y + rect2.Size.Y)
        && rect1.Position.Y + rect1.Size.Y >= rect2.Position.Y
        ;
}

// ===================================================================================
// Box -> Box
// ===================================================================================

// *************************************************************************************
bool Intersection2::Test(const Box2& box1, const Box2& box2)
{
    // Get the difference vector between the box centers
    Vector2 diff = box1.Center - box2.Center;
    
    // Axis C0+t*A0
    float absA0dotB0 = Math::Abs(box2.Axis[0].Dot(box1.Axis[0]));
    float absA0dotB1 = Math::Abs(box2.Axis[0].Dot(box1.Axis[1]));
    float absA0dotDiff = Math::Abs(box2.Axis[0].Dot(diff));
    float sum = box2.Extent[0] + box1.Extent[0]*absA0dotB0 + box1.Extent[1]*absA0dotB1;
    if (absA0dotDiff > sum)
    {
        return false;
    }
    
    // Axis C0+t*A1
    float absA1dotB0 = Math::Abs(box2.Axis[1].Dot(box1.Axis[0]));
    float absA1dotB1 = Math::Abs(box2.Axis[1].Dot(box1.Axis[1]));
    float absA1dotDiff = Math::Abs(box2.Axis[1].Dot(diff));
    sum = box2.Extent[1] + box1.Extent[0]*absA1dotB0 + box1.Extent[1]*absA1dotB1;
    if (absA1dotDiff > sum)
    {
        return false;
    }
    
    // Axis C0+t*B0
    float absB0dotDiff = Math::Abs(box1.Axis[0].Dot(diff));
    sum = box1.Extent[0] + box2.Extent[0]*absA0dotB0 + box2.Extent[1]*absA1dotB0;
    if (absB0dotDiff > sum)
    {
        return false;
    }
    
    // Axis C0+t*B1
    float absB1dotDiff = Math::Abs(box1.Axis[1].Dot(diff));
    sum = box1.Extent[1] + box2.Extent[0]*absA0dotB1 + box2.Extent[1]*absA1dotB1;
    if (absB1dotDiff > sum)
    {
        return false;
    }
    
    return true;
}
