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

// *****************************************************************
/// @brief
///     Constructor
/// @remarks
///     Instantiates a new touch in the Began state
/// @note
///     GDK Internal Use Only
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Copy Constructor
/// @param input
///     Source touch to copy
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Assignment-copy operator
/// @param input
///     Source touch to copy
// *****************************************************************
Touch& Touch::operator= (const Touch& input)
{
    this->position = input.position;
    this->previousPosition = input.previousPosition;
    this->startingPosition = input.startingPosition;
    this->state = input.state;
    this->owner = input.owner;
    this->internalId = input.internalId;
    this->timeStamp = input.timeStamp;
    return *this;
}

// *****************************************************************
/// @brief
///     Equality operator
/// @remarks
///     Returns true if the two touches are the same touch.
// *****************************************************************
bool Touch::operator== (const Touch& input) const
{
    return this->internalId == input.internalId;
}

// *****************************************************************
/// @brief
///     In-equality operator
/// @remarks
///     Returns true if the two touches are NOT the same touch.
// *****************************************************************
bool Touch::operator!= (const Touch& input) const
{
    return this->internalId != input.internalId;
}

// *****************************************************************
/// @brief
///     Gets the current position of the touch
// *****************************************************************
Vector2 Touch::GetPosition() const
{
    return this->position;
}

// *****************************************************************
/// @brief
///     Gets the position the touch was at in the last frame
// *****************************************************************
Vector2 Touch::GetPreviousPosition() const
{
    return this->previousPosition;
}

// *****************************************************************
/// @brief
///     Gets the position the touch was at when it first began
// *****************************************************************
Vector2 Touch::GetStartingPosition() const  
{
    return this->startingPosition;
}

// *****************************************************************
/// @brief
///     Gets the current inter-frame state of the touch
// *****************************************************************
TouchState::Enum Touch::GetState() const	
{
    return this->state;
}

// *****************************************************************
/// @brief
///     Gets the owner pointer of the touch.
// *****************************************************************
void* Touch::GetOwner()	const		
{
    return this->owner;
}

// *****************************************************************
/// @brief
///     Initializes the static TouchInput class internal data
/// @note
///     GDK Internal Use Only
// *****************************************************************
void TouchInput::Init()
{
	touches.reserve(20);
	touchUpdates.reserve(100);
}

// *****************************************************************
/// @brief
///     Processes any touch state changes or updates that have occured since the last frame
/// @note
///     GDK Internal Use Only
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Gets all the active touches
/// @return
///     Gets a reference to a vector of all the touches currently
///     recognized by the TouchInput system.
// *****************************************************************
vector<Touch>& TouchInput::GetAllTouches()
{ 
    return touches; 
}

// *****************************************************************
/// @brief
///     Gets all the active touches
/// @remarks
///     Pointers to all of the active touches are copied to the given TouchSet vector.
///     Use this method if you are caching a TouchSet to avoid inter-frame allocations.
// *****************************************************************
void TouchInput::GetAllTouches(TouchSet& touchSet)
{
	// Add all the touches to the given set
	touchSet.reserve(touches.size());
	for(vector<Touch>::iterator iter = touches.begin(); iter != touches.end(); iter++)
		touchSet.push_back(&(*iter));
}

// *****************************************************************
/// @brief
///     Gets all touches that have the given assigned owner
/// @remarks
///     Use touch ownership to properly share touches with other systems.  As a general 
///     rule of thumb, dont use touches that you dont own, and claim ownership of 
///     un-owned touches that you want to own.
/// @param owner
///     A general pointer that is assigned as the "Owner" of the touch.  Pass NULL
///     if you want to get all un-owned touches.
/// @param touchSet
///     A TouchSet vector to copy the touches into.  If you call this method over multiple 
///     frames,  consider caching & re-using a single TouchSet instance to avoid inter-frame
///     allocations
// *****************************************************************
void TouchInput::GetTouchesByOwner(void* owner, TouchSet& touchSet)
{
	// Add all the touches with the given owner to the given set
	touchSet.reserve(touches.size());
	for(vector<Touch>::iterator iter = touches.begin(); iter != touches.end(); iter++)
		if(iter->owner == owner)
			touchSet.push_back(&(*iter));
}

// *****************************************************************
/// @brief
///     Sets the owner of a touch
/// @remarks
///     Use touch ownership to properly share touches with other systems.  As a general 
///     rule of thumb, dont use touches that you dont own, and claim ownership of 
///     un-owned touches that you want to own.
/// @param touch
///     The touch to claim ownership of
/// @param owner
///     A general pointer that is assigned as the "Owner" of the touch.
// *****************************************************************
void TouchInput::SetTouchOwner(Touch& touch, void* owner)
{
	// Find the touch
	Touch* realTouch = GetTouchById(touch.internalId);
    
	// Assign the owner to the given touch
	if(realTouch != NULL)
		realTouch->owner = owner;
}

// *****************************************************************
/// @brief
///     Gets a specific touch by it's touch Id
/// @param internalId
///     Id of the touch to get
/// @param ignoreEndedTouches
///     Causes NULL to be returned if the given touch is Ended.  Generally you should pass @em false for this parameter
/// @return
///     Returns a pointer to the touch or NULL if the touch doesn't exist
/// @note
///     GDK Internal Use Only
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Tells the GDK about a touch update event
/// @note
///     GDK Internal Use Only
// *****************************************************************
void TouchInput::Platform_ProcessTouchUpdate(int internalId, Vector2 position, TouchState::Enum state)
{
	// Create a touch update & add it to the processing queue
	touchUpdates.push_back(TouchUpdate(internalId, position, state));
}



