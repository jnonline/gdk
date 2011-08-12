/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	class LineSegment2
	{
	public:
		// Components
		Vector2 Start;
		Vector2 End;

		// Constructors
		LineSegment2();
		LineSegment2(const Vector2& start, const Vector2& end);
		LineSegment2(const LineSegment2& input);

		// Asssignment Operator
		inline LineSegment2& operator= (const LineSegment2& input);

		// Comparison Operators
		inline bool operator== (const LineSegment2& input) const;
		inline bool operator!= (const LineSegment2& input) const;

		// Utility Methods
		Vector2 GetParametricPoint(float t);
	};

	// ===================================================================================
	// Asssignment Operator
	// ===================================================================================

	// ***********************************************************************
	inline LineSegment2& LineSegment2::operator= (const LineSegment2& input)
	{
		Start = input.Start;
		End = input.End;
		return *this;
	}

	// ===================================================================================
	// Comparison Operators
	// ===================================================================================

	// ***********************************************************************
	inline bool LineSegment2::operator== (const LineSegment2& input) const
	{
		return 
			Start == input.Start && End == input.End;
	}

	// ***********************************************************************
	inline bool LineSegment2::operator!= (const LineSegment2& input) const
	{
		return 
			Start != input.Start || End != input.End;
	}

} // namespace
