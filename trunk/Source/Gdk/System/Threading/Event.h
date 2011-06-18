#pragma once

#include "GdkLIB.h"


namespace Gdk
{
	// ============================================================================

	class Event
	{
	public: 

		// Instance Methods
		// --------------------------------------------

		// DTor
		~Event();

		// Waits for the event to be signaled.  Optionally resets the event after it is signal'd
		void Wait(bool reset = true);

		// Signals the event
		void Set();

		// Resets the event signal
		void Reset();

		// Static Methods
		// --------------------------------------------

		static Event* Create();

	private:

		// Internals
		// --------------------------------------------

		// CTor
		Event();

		// Properties
		bool signaled;
		pthread_mutex_t internalMutex;
		pthread_cond_t internalCV;


	};


} // namespace Gdk
