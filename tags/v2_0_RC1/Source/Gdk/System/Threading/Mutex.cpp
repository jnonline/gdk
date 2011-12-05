/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Mutex.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Mutex::Mutex()
{
}

// *****************************************************************
/// @brief
///     Default destructor
// *****************************************************************
Mutex::~Mutex()
{
	pthread_mutex_destroy(&this->internalMutex);
}

// *****************************************************************
/// @brief
///     Creates a new mutex
// *****************************************************************
Mutex* Mutex::Create()
{
	// Create the mutex
	Mutex* mutex = GdkNew Mutex();

	// Create the pthread mutex
	pthread_mutex_init(&mutex->internalMutex, NULL);

	return mutex;
}

// *****************************************************************
/// @brief
///     Asks for a lock on this mutex, blocking until the lock is received
// *****************************************************************
void Mutex::Lock()
{
	// Lock the mutex
	pthread_mutex_lock(&this->internalMutex);
}

// *****************************************************************
/// @brief
///     Tries to lock a mutex, returning immediately.
/// @return
//      Returns true if the calling thread got the mutex lock
// *****************************************************************
bool Mutex::TryLock()
{
	// Lock the mutex
	int result = pthread_mutex_trylock(&this->internalMutex);
	if(result == EBUSY)
		return false;
	return true;
}

// *****************************************************************
/// @brief
///     Unlocks the mutex, allowing another waiting thread take ownerhip
// *****************************************************************
void Mutex::Unlock()
{
	// Lock the mutex
	pthread_mutex_unlock(&this->internalMutex);
}