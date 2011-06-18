
#pragma once

#include "GdkLIB.h"
#include "../Math/2D/Vector2.h"
#include "../System/Delegates.h"

// ************************************************************
//
// TouchInput - Proper Usage
//
//    For the easiest touch response & handling..  you should use the Touch*** events.. As these will guarantee touch update ordering.
//    Also, if you want to claim a touch during an event handler, simply call TouchInput::SetTouchOwner();
//  
//    You can also call GetAllTouches() or GetTouchesByOwner() if you like, but the code for handling touches can be a bit more complex..
//
//    You will always get a BEGAN touch for new touches (unless the touch is ended or canceled by the OS before GDK can handle it).
//    You are also ALWAYS guaranteed that if you got a Began touch, you will get an Ended even for the same touch.
//
//
// ************************************************************


namespace Gdk
{
	// ============================================================================

	namespace TouchState
	{
		enum Enum
		{
			Began,			// This is a new touch
			Moved,			// The touch moved since the last frame
			Stationary,		// The touch was stationary since the last frame
			Ended,			// The touch is ending this frame (and will be removed before the next frame)
		};
	}

	// =========================================================================================================

	class Touch
	{
	private:
		// Private Methods
		// ---------------------------

		friend class TouchInput;
		Touch(int internalId, Vector2 position);

		// Private Properties
		// ---------------------------

        Vector2 startingPosition;
		Vector2 position;
		Vector2 previousPosition;
		TouchState::Enum state;
		void* owner;
		double timeStamp;
        int internalId;
        

	public:
		// Public Methods
		// ---------------------------
		
		Touch(const Touch& touch);

		// Operators
		inline Touch& operator= (const Touch& input);
		inline bool operator== (const Touch& input) const;
		inline bool operator!= (const Touch& input) const;

		// Methods		
		inline Vector2 GetPosition()			{return this->position;}
		inline Vector2 GetPreviousPosition()	{return this->previousPosition;}
        inline Vector2 GetStartingPosition()    {return this->startingPosition;}
		inline TouchState::Enum GetState()		{return this->state;}
		inline void* GetOwner()					{return this->owner;}
	};

	// ***********************************************************************
	inline Touch& Touch::operator= (const Touch& input)
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

	// ***********************************************************************
	inline bool Touch::operator== (const Touch& input) const
	{
		return this->internalId == input.internalId;
	}

	// ***********************************************************************
	inline bool Touch::operator!= (const Touch& input) const
	{
		return this->internalId != input.internalId;
	}

	typedef vector<Touch*> TouchSet;
	
	// =========================================================================================================

	class TouchInput
	{
	private:
		TouchInput();
	    
		// Application Interface Methods
		// -----------------------------------

		friend class Application;

		// Init / Update
		static void Init();
		static void Update(float elapsedSeconds);

		static Touch* GetTouchById(int internalId, bool ingoreEndedTouches = false);

		// Internal Properties
		// -------------------------

		struct TouchUpdate
		{
			int InternalId;
			Vector2 Position;
			TouchState::Enum State;
			
			TouchUpdate()
				: InternalId(0), Position(Vector2::ZERO), State(TouchState::Began)
			{}

			TouchUpdate(int internalId, Vector2 position, TouchState::Enum state)
				: InternalId(internalId), Position(position), State(state)
			{}
			
			TouchUpdate(const TouchUpdate& touchUpdate)
				: InternalId(touchUpdate.InternalId), Position(touchUpdate.Position), State(touchUpdate.State)
			{}
		};

		// Current state
		static vector<Touch> touches;
		static vector<TouchUpdate> touchUpdates;

	public:
		// Public Platform Interfaces
		// ----------------------------------

		// Input processing
		static void Platform_ProcessTouchUpdate(int internalId, Vector2 position, TouchState::Enum state);

	public:
		// Public Game Interfaces
		// ------------------------------
	
        // Methods
        static vector<Touch>& GetAllTouches()   { return touches; }
		static void GetAllTouches(TouchSet& touchSet);
		static void GetTouchesByOwner(void* owner, TouchSet& touchSet);
		static void SetTouchOwner(Touch& touch, void* owner);
        
        // Delegates
		typedef Gdk::MulticastDelegate1<void, Touch*>	TouchEventHandler;
        
		// Events
		static TouchEventHandler	TouchBegan;
		static TouchEventHandler	TouchMoved;
		static TouchEventHandler	TouchEnded;
	};

} // namespace
