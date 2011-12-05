/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "../Logging.h"
#include "Thread.h"


using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************

Thread::Thread()
{
    memset(&this->internalThreadId, 0, sizeof(pthread_t));
}

// *****************************************************************
/// @brief
///     Copy constructor
// *****************************************************************
Thread::Thread(const Thread& original)
{
	this->internalThreadId = original.internalThreadId;
}

// *****************************************************************
/// @brief
///     Assignment operator
// *****************************************************************
Thread& Thread::operator= (const Thread& input)
{
	this->internalThreadId = input.internalThreadId;
	return *this;
}

// ===================================================================================
struct ThreadLaunchData
{
	Thread::Delegate* ThreadDelegate;
	void* ThreadArgument;
};

// ===================================================================================
void *Gdk_Thread_ThreadFunction(void* arg)
{
	// Get the launch data
	ThreadLaunchData* launchData = (ThreadLaunchData*) arg;
	Thread::Delegate* threadDelegate = launchData->ThreadDelegate;
	void* threadArgument = launchData->ThreadArgument;

	// Destroy the launch data struct
	GdkDelete(launchData);

	// Call the delegate
	return threadDelegate->Invoke(threadArgument);
}

// *****************************************************************
/// @brief
///     Creates a new thread.
/// @remarks
///     The caller is responsible for cleaning the delegate* when the thread finishes
/// @param threadDelegate
///     A delegate that will be run in the thread
/// @param argument
///     A void* of data that will be passed to the threadDelegate
/// @param createDetached
///     If true, the thread will be detached, meaning it will clean itself up when it finishes executing  (Default = false)
// *****************************************************************
Thread Thread::Create(Thread::Delegate* threadDelegate, void* argument, bool createDetached)
{
	
	// Setup the thread attributes
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_attr_setdetachstate(&attributes, createDetached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
	
	// Create a ThreadLaunchData
	ThreadLaunchData* launchData = GdkNew ThreadLaunchData;
	launchData->ThreadDelegate = threadDelegate;
	launchData->ThreadArgument = argument;

	// Create the thread
	Thread thread;
	pthread_create(&thread.internalThreadId, &attributes, Gdk_Thread_ThreadFunction, (void*)launchData);
	
	// Release the thread attributes
	pthread_attr_destroy(&attributes);

	return thread;
}


// *****************************************************************
/// @brief
///     Creates a new thread.
/// @param threadFunction
///     A function or static class method that will be executed in the thread.
/// @param argument
///     A void* of data that will be passed to the threadFunction
/// @param createDetached
///     If true, the thread will be detached, meaning it will clean itself up when it finishes executing  (Default = false)
// *****************************************************************
Thread Thread::Create(void* threadFunction(void*), void* argument, bool createDetached)
{
	// Setup the thread attributes
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_attr_setdetachstate(&attributes, createDetached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
	
	// Create the thread
	Thread thread;
	pthread_create(&thread.internalThreadId, &attributes, threadFunction, argument);
	
	// Release the thread attributes
	pthread_attr_destroy(&attributes);

	return thread;
}

// *****************************************************************
/// @brief
///     Waits for the thread to finish execution
/// @remarks
///     The calling thread will block and wait for the this thread to finish.
// *****************************************************************
void* Thread::Join()
{
	// Join the thread
	void* returnValue;
	pthread_join(this->internalThreadId, &returnValue);

	return returnValue;
}

// *****************************************************************
/// @brief
///     Detaches a thread from tracking
/// @remarks
///     Detached threads are no longer trackable or managed from the threading system.  They will clean themselves up.
// *****************************************************************
void Thread::Detach()
{
	// Detach the thread
	pthread_detach(this->internalThreadId);
}

// *****************************************************************
/// @brief
///     Gets a unique identifier for the thread
/// @remarks
///     The calling thread will block and wait for the given thread to finish
// *****************************************************************
UInt32 Thread::GetId()
{
	// Get the thread ID as a UInt32
#ifdef GDKPLATFORM_WINDOWS
    return (UInt32) internalThreadId.p;
#else
    return (UInt32) internalThreadId->__sig;
#endif
}

// *****************************************************************
/// @brief
///     Gets the unique ID of the current executing thread
// *****************************************************************
UInt32 Thread::GetCurrentThreadId()
{
	// Get the current thread
	pthread_t currentThreadId = pthread_self();
	
	// Return the thread ID as a UInt32
#ifdef GDKPLATFORM_WINDOWS
    return (UInt32) currentThreadId.p;
#else
    return (UInt32) currentThreadId->__sig;
#endif
}

// *****************************************************************
/// @brief
///     Ends the currently executing thread
/// @param returnValue
///     A void* of data that gets returned to any threads waiting on a Join() call for this thread.
// *****************************************************************
void Thread::Exit(void* returnValue)
{
	// Exit the current thread
	pthread_exit(returnValue);
}

// *****************************************************************
/// @brief
///     Puts the currently executing thread to sleep for a number of milliseconds 
// *****************************************************************
void Thread::Sleep(UInt32 milliSeconds)
{
	#ifdef GDKPLATFORM_WINDOWS
		::Sleep(milliSeconds);
	#else // Apple & *nix platforms
		useconds_t microSeconds = milliSeconds * 1000;
		usleep(microSeconds);
	#endif
}