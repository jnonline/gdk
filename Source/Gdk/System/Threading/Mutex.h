/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



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
