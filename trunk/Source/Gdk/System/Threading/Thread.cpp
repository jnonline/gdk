
#include "BasePCH.h"
#include "../Logging.h"
#include "Thread.h"


using namespace Gdk;

// ===================================================================================
Thread::Thread()
{
    memset(&this->internalThreadId, 0, sizeof(pthread_t));
}

// ===================================================================================
Thread::Thread(const Thread& original)
{
	this->internalThreadId = original.internalThreadId;
}

// ===================================================================================
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

// ===================================================================================
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

// ===================================================================================
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

// ===================================================================================
void* Thread::Join()
{
	// Join the thread
	void* returnValue;
	pthread_join(this->internalThreadId, &returnValue);

	return returnValue;
}

// ===================================================================================
void Thread::Detach()
{
	// Detach the thread
	pthread_detach(this->internalThreadId);
}

// ===================================================================================
UInt32 Thread::GetId()
{
	// Get the thread ID as a UInt32
#ifdef GDKPLATFORM_WINDOWS
    return (UInt32) internalThreadId.p;
#else
    return (UInt32) internalThreadId->__sig;
#endif
}

// ===================================================================================
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

// ===================================================================================
void Thread::Exit(void* returnValue)
{
	// Exit the current thread
	pthread_exit(returnValue);
}

// ===================================================================================
void Thread::Sleep(UInt32 milliSeconds)
{
	#ifdef GDKPLATFORM_WINDOWS
		::Sleep(milliSeconds);
	#else // Apple & *nix platforms
		useconds_t microSeconds = milliSeconds * 1000;
		usleep(microSeconds);
	#endif
}