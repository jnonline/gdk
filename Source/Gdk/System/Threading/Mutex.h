#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ============================================================================

	class Mutex
	{
	public: 

		// Instance Methods
		// --------------------------------------------

		// DTor
		~Mutex();

		// Locks the mutex
		void Lock();

		// Attempts to lock the mutex.  If the mutex is already locked the method returns false immediately.
		bool TryLock();

		// Releases the mutex lock
		void Unlock();

		// Static Methods
		// --------------------------------------------

		static Mutex* Create();

	private:

		// Internals
		// --------------------------------------------

		// CTor
		Mutex();

		// Properties
		pthread_mutex_t internalMutex;
	};

	// TODO(P2):  Timeout based locking : pthread_mutex_timedlock()

} // namespace Gdk
