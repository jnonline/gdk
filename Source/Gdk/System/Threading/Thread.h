/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "../Delegates.h"

// -------------------------------------------------------------------
// Threading Notes:
//
// Every thread must be either Detach()'d or Join()'d in order to properly cleanup the thread
// ------------------------------------------------------------------- 

namespace Gdk
{
	// ============================================================================

	class Thread
	{
	public:

		// Types
		// --------------------------------------------

		// The type for valid thread Worker() functions/methods
		typedef		Delegate1<void*, void*>		Delegate;

		// Instance Methods
		// --------------------------------------------

		
		// CTors
		Thread();
		Thread(const Thread& input);
		
		// Assignment operator
		Thread& operator= (const Thread& input);

		// Halts execution of the calling thread until the thread being joined is complete.  (The result from the thread delegate is returned from this method once the thread finishes execution)
		void* Join();

		// Detachs the thread, making it no longer accessible & controllable via the threading system.
		void Detach();

		// Returns the ID of the thread
		UInt32 GetId();
		
		// Conversions operator
		operator UInt32 () { return GetId(); }
        
		// Static Methods
		// --------------------------------------------

		// Creates a thread using the given delegate and argument.  The caller retains ownership of the delegate.  Optionally can create the thread in a detached state
		static Thread Create(Thread::Delegate* threadDelegate, void* argument = 0, bool createDetached = false);

		// Creates a thread using the given function pointer.  Optionally starts the thread in a detached state
		static Thread Create(void* threadFunction(void*), void* argument = 0, bool createDetached = false);

		// Gets the ID of the currently executing thread
		static UInt32 GetCurrentThreadId();

		// Exits the current thread
		static void Exit(void* returnValue);

		// Sleeps for the given number of milliseconds
		static void Sleep(UInt32 milliseconds);

	private:

		// Internals
		// --------------------------------------------

		// Properties
		pthread_t internalThreadId;
	};

} // namespace Gdk
