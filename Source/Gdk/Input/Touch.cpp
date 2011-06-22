/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Touch.h"

using namespace Gdk;

// Static instantiations
vector<Touch> TouchInput::touches;
vector<TouchInput::TouchUpdate> TouchInput::touchUpdates;

// Events
TouchInput::TouchEventHandler	TouchInput::TouchBegan;
TouchInput::TouchEventHandler	TouchInput::TouchMoved;
TouchInput::TouchEventHandler	TouchInput::TouchEnded;

// ***********************************************************************
Touch::Touch(int internalId, Vector2 position)
{
	this->internalId = internalId;
	this->position = position;
	this->previousPosition = position;
    this->startingPosition = position;
	this->state = TouchState::Began;
	this->owner = NULL;
    this->timeStamp = Gdk::HighResTimer::GetSeconds();
}

// ***********************************************************************
Touch::Touch(const Touch& input)
{
	this->position = input.position;
	this->previousPosition = input.previousPosition;
    this->startingPosition = input.startingPosition;
	this->state = input.state;
	this->owner = input.owner;
	this->internalId = input.internalId;
    this->timeStamp = input.timeStamp;
}

// ***********************************************************************
TouchInput::TouchInput()
{
}

// ***********************************************************************
void TouchInput::Init()
{
	touches.reserve(20);
	touchUpdates.reserve(100);
}

// ***********************************************************************
void TouchInput::Update(float elapsedSeconds)
{
	// Do pre-processing on the current set of touches
	// -----------------------------------------

	vector<Touch>::iterator touchIter;
	touchIter = touches.begin();
	while(touchIter != touches.end() )
	{
		Touch& touch = (*touchIter);

		// Is this touch ended?
		if(touch.state == TouchState::Ended)
		{
			// Remove this touch
			touchIter = touches.erase(touchIter);
		}
		else
		{
			// Copy the position to the previous position
			touch.previousPosition = touch.position;

			// Set the touch state to stationary  (was began, moved, or stationary)
			touch.state = TouchState::Stationary;

			// Advance the touch iterator
			touchIter++;
		}
	}

	// Process the touch update queue
	// -----------------------------------------
    
    // This thing seems overly complex..  but it's necessary, as the OS may have multiple Began, Moved, Ended, Began.. updates within the same
    // update cycle, using the same Touch ID.  This system ignores touches once we've recieved an Ended event (and cleans them next cycle)

    // Loop through the touch updates
	for(vector<TouchInput::TouchUpdate>::iterator updateIter = touchUpdates.begin(); updateIter != touchUpdates.end(); updateIter++)
	{
		TouchInput::TouchUpdate& touchUpdate = (*updateIter);
        Touch* touch = NULL;

		// Is this a new touch?
		if(touchUpdate.State == TouchState::Began)
		{
			// Add a new touch to the touches vector
			touches.push_back(Touch(touchUpdate.InternalId, touchUpdate.Position));
            touch = &(touches.back());
            
            // Fire a TouchBegan event for this new touch
            TouchBegan.Invoke(touch);
		}
		else
		{
			// Get the touch that relates to this touch update  (Ignore touches that are already flagged as ended)
            touch = GetTouchById(touchUpdate.InternalId, true);
			if(touch != NULL)
			{
                
				// Update the position of the touch
				touch->position = touchUpdate.Position;
				
				// Update the touch's state (except for the case where the touch is new & this is a move update)
                if(touch->state == TouchState::Began && touchUpdate.State == TouchState::Moved)
                {
                    // No nothing.  This only happens if we get a Began & Move in the same update cycle. So we leave the touch
                    // in the Began state.
                }
                else
                {
                    touch->state = touchUpdate.State;
                }
                
                // Fire an event for this touch if it moved or ended
                if(touch->state == TouchState::Moved)
                {
                    TouchMoved.Invoke(touch);
                }
                else if(touch->state == TouchState::Ended)
                {
                    TouchEnded.Invoke(touch);
                }
			}
		}
	}

	// Clear the touch update queue
	touchUpdates.clear();

}

// ***********************************************************************
Touch* TouchInput::GetTouchById(int internalId, bool ignoreEndedTouches)
{
	// Loop through the touches
	for(vector<Touch>::iterator iter = touches.begin(); iter != touches.end(); iter++)
	{
		Touch& touch = (*iter);
		if(touch.internalId != internalId)
           continue;
        
        if(ignoreEndedTouches == true && touch.GetState() == TouchState::Ended)
            continue;
        
        return &touch;
	}

	// No touch found with the corresponding Id
	return NULL;
}

// ***********************************************************************
void TouchInput::Platform_ProcessTouchUpdate(int internalId, Vector2 position, TouchState::Enum state)
{
	// Create a touch update & add it to the processing queue
	touchUpdates.push_back(TouchUpdate(internalId, position, state));
}

// ***********************************************************************
void TouchInput::GetAllTouches(TouchSet& touchSet)
{
	// Add all the touches to the given set
	touchSet.reserve(touches.size());
	for(vector<Touch>::iterator iter = touches.begin(); iter != touches.end(); iter++)
		touchSet.push_back(&(*iter));
}

// ***********************************************************************
void TouchInput::GetTouchesByOwner(void* owner, TouchSet& touchSet)
{
	// Add all the touches with the given owner to the given set
	touchSet.reserve(touches.size());
	for(vector<Touch>::iterator iter = touches.begin(); iter != touches.end(); iter++)
		if(iter->owner == owner)
			touchSet.push_back(&(*iter));
}

// ***********************************************************************
void TouchInput::SetTouchOwner(Touch& touch, void* owner)
{
	// Find the touch
	Touch* realTouch = GetTouchById(touch.internalId);

	// Assign the owner to the given touch
	if(realTouch != NULL)
		realTouch->owner = owner;
}


