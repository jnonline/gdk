
#include "BasePCH.h"
#include "Mutex.h"

using namespace Gdk;

// ===================================================================================
Mutex::Mutex()
{
}

// ===================================================================================
Mutex::~Mutex()
{
	pthread_mutex_destroy(&this->internalMutex);
}

// ===================================================================================
Mutex* Mutex::Create()
{
	// Create the mutex
	Mutex* mutex = GdkNew Mutex();

	// Create the pthread mutex
	pthread_mutex_init(&mutex->internalMutex, NULL);

	return mutex;
}

// ===================================================================================
void Mutex::Lock()
{
	// Lock the mutex
	pthread_mutex_lock(&this->internalMutex);
}

// ===================================================================================
bool Mutex::TryLock()
{
	// Lock the mutex
	int result = pthread_mutex_trylock(&this->internalMutex);
	if(result == EBUSY)
		return false;
	return true;
}

// ===================================================================================
void Mutex::Unlock()
{
	// Lock the mutex
	pthread_mutex_unlock(&this->internalMutex);
}