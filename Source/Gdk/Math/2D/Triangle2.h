/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	class Triangle2
	{
	public:
		// Components
		Vector2 Vertices[3];
		
		// Constructors
		Triangle2 ();
		Triangle2 (const Vector2& v1, const Vector2& v2, const Vector2& v3);
        Triangle2 (const Vector2* vertices);
		Triangle2 (const Triangle2& input);
	};

} // namespace Gdk
