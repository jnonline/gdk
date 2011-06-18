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
