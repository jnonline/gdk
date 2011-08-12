/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "LineSegment2.h"

using namespace Gdk;

// ===================================================================================
// Constructors
// ===================================================================================

// ***********************************************************************
LineSegment2::LineSegment2()
{
}

// ***********************************************************************
LineSegment2::LineSegment2(const Vector2& start, const Vector2& end)
{
	Start = start;
	End = end;
}

// ***********************************************************************
LineSegment2::LineSegment2(const LineSegment2& input)
{
	Start = input.Start;
	End = input.End;
}

// ===================================================================================
// Utility Methods
// ===================================================================================

// ***********************************************************************
Vector2 LineSegment2::GetParametricPoint(float t)
{
	return Start + t * (End - Start);
}

