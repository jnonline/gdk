/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Event.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Default constructor
// *****************************************************************
Event::Event()
{
}

// *****************************************************************
/// @brief
///     Default destructor
// *****************************************************************
Event::~Event()
{
	pthread_mutex_destroy(&this->internalMutex);
	pthread_cond_destroy(&this->internalCV);

}

// *****************************************************************
/// @brief
///     Creates a new event
// *****************************************************************
Event* Event::Create()
{
	// Create the event
	Event* event = GdkNew Event();
	event->signaled = false;

	// Create the pthread mutex & conditional
	pthread_mutex_init(&event->internalMutex, NULL);
	pthread_cond_init(&event->internalCV, NULL);

	return event;
}

// *****************************************************************
/// @brief
///     Waits for the event to be signaled.
/// @param reset
///     If true (default), the event signal will be reset when the waiting thread is resumes.
// *****************************************************************
void Event::Wait(bool reset)
{
	// Lock the mutex
	pthread_mutex_lock(&this->internalMutex);

	// Go into a wait state until the signal fires
	while(this->signaled == false)
	{
		// Wait on the condition variable
		pthread_cond_wait(&this->internalCV, &this->internalMutex);
	}

	// Autoreset?
	if(reset)
		this->signaled = false;

	// Unlock the mutex
	pthread_mutex_unlock(&this->internalMutex);
}

// *****************************************************************
/// @brief
///     Signals the event
/// @remarks
///     All blocked threads waiting for the event will resume.
// *****************************************************************
void Event::Set()
{
	// Lock the mutex
	pthread_mutex_lock(&this->internalMutex);

	// Set the signal
	this->signaled = true;

	// Signal the condition
	pthread_cond_broadcast(&this->internalCV);

	// Unlock the mutex
	pthread_mutex_unlock(&this->internalMutex);
}

// *****************************************************************
/// @brief
///     Resets the event, (turn off the signal)
// *****************************************************************
void Event::Reset()
{
	// Lock the mutex
	pthread_mutex_lock(&this->internalMutex);

	// Reset the signal
	this->signaled = false;

	// Unlock the mutex
	pthread_mutex_unlock(&this->internalMutex);
}