/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"


namespace Gdk
{
	// ============================================================================

	class HighResTimer
	{
	public:
		// Publics
		// -----------------------------

		// Gets a high-resolution time stamp
		static double GetSeconds();

	private:
		// Hidden CTor
		HighResTimer();
	};

} // namespace Gdk
