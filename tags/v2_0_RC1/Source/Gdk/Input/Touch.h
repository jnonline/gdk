/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


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
	/// @addtogroup Input
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Enumeration of states for a touch
    // =================================================================================
	namespace TouchState
	{
		enum Enum
		{
            /// The touch is new and didnt exist in the last frame
			Began,
            
            /// The touch moved since the last frame
			Moved,
            
            /// The touch has been stationary since the last frame (didnt move)
			Stationary,
            
            /// The touch has ended or been released, and will not exist in the next frame.
			Ended,
		};
	}

	// =================================================================================
    ///	@brief
    ///		Contains state data for a single touch
    /// @remarks
    ///     An instance of this class contains state, position, and other data for a 
    ///     single touch action.   Use the TouchInput class to access or manage the
    ///     touch instances.
    /// @see 
    ///     TouchInput
    // =================================================================================
	class Touch
	{
	public:
        
		// Public Methods
		// =====================================================
		
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Touch(const Touch& touch);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
        Touch& operator= (const Touch& input);
        bool operator== (const Touch& input) const;
        bool operator!= (const Touch& input) const;

        /// @}
        // ---------------------------------
        /// @name Methods
        /// @{
        
		Vector2 GetPosition() const;
        Vector2 GetPreviousPosition() const;
        Vector2 GetStartingPosition() const;
        TouchState::Enum GetState() const;
        void* GetOwner() const;
    
        /// @}
        
    private:
        
		// Internal Properties
		// ================================
        
        Vector2 startingPosition;
		Vector2 position;
		Vector2 previousPosition;
		TouchState::Enum state;
		void* owner;
		double timeStamp;
        int internalId;
        
        // Internal Methods
		// ================================
        
		Touch(int internalId, Vector2 position);
        
        friend class TouchInput;

	};

    // =================================================================================
    ///	@brief
    ///		A vector of Touch* instances
    // =================================================================================
	typedef vector<Touch*> TouchSet;
	
	// =================================================================================
    ///	@brief
    ///		Provides access to Touch based input.
    ///	@remarks 
    ///		This static-only class contains properties, methods, and events for 
    ///     interacting with multi-touch inputs.
    // =================================================================================
	class TouchInput
	{
    public:
		// Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        static vector<Touch>& GetAllTouches();
		static void GetAllTouches(TouchSet& touchSet);
		static void GetTouchesByOwner(void* owner, TouchSet& touchSet);
		static void SetTouchOwner(Touch& touch, void* owner);
        
        /// @}
        
        // Public Types
		// =====================================================
        
        // ---------------------------------
        /// @name Delegate Types
        /// @{
        
        /// Multicast delegate type for touch events
		typedef Gdk::MulticastDelegate1<void, Touch*>	TouchEventHandler;
        
        /// @}
        
        // Public Events
		// =====================================================
        
		/// @name Events
        /// @{
        
		/// @brief
		///     This event is raised when a new touch is detected.
        static TouchEventHandler	TouchBegan;
        
        /// @brief
		///     This event is raised when an existing touch is moved
		static TouchEventHandler	TouchMoved;
        
        /// @brief
		///     This event is raised when a touch is released / ended.
		static TouchEventHandler	TouchEnded;

        /// @}
        
    public:
		
        // INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
        
		// Input processing
		static void Platform_ProcessTouchUpdate(int internalId, Vector2 position, TouchState::Enum state);
        
	private:
        
        // Internal Types
		// ================================
        
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

        
        // Internal Properties
		// ================================
        
		static vector<Touch> touches;
		static vector<TouchUpdate> touchUpdates;
        
        // Internal Methods
		// ================================
        
		static void Init();
		static void Update(float elapsedSeconds);
		static Touch* GetTouchById(int internalId, bool ingoreEndedTouches = false);

        /// @}
        
        /// @endcond
		
		friend class Application;		
    };
    
    /// @} // Input

} // namespace
