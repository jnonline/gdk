/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Math/2D/Vector2.h"
#include "../System/Delegates.h"

namespace Gdk
{
	/// @addtogroup Input
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Enumeration of buttons available on a Mouse
    // =================================================================================
	namespace MouseButton
	{
		enum Enum
		{
			Left = 0,
			Right = 1,
			Middle = 2,
            
			Button4 = 3,
			Button5,
            Button6,
            Button7,
            Button8,

			MAX_BUTTONS
		};
	}

    // =================================================================================
    ///	@brief
    ///		Struct containing the new and previous positions of a mouse movement
    /// @see 
    ///     Mouse::MouseMove
    // =================================================================================
    struct MouseMoveArgs
    {
        /// New X position of the mouse
        int NewX;
        
        /// New Y position of the mouse
        int NewY;
        
        /// Previous X position of the mouse
        int PreviousX;
        
        /// Previous Y position of the mouse
        int PreviousY;
        
        // *****************************************************************
        /// @brief
        ///     Constructor
        // *****************************************************************
        MouseMoveArgs(int newX, int newY, int previousX, int previousY) 
            : NewX(newX), NewY(newY), PreviousX(previousX), PreviousY(previousY)
        {}
    };
    
    // =================================================================================
    ///	@brief
    ///		Provides access to Mouse based input.
    ///	@remarks 
    ///		This static-only class contains properties, methods, and events for interacting
    ///     with the Mouse
    // =================================================================================
	class Mouse
	{
    public:
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
		static Vector2 GetPosition();
		static int GetX();
		static int GetY();
		static bool IsButtonDown(MouseButton::Enum button);
		static bool IsMouseOverApp();
        
		static void SetPosition(int x, int y);
		static void ShowCursor(bool show);		
        
        /// @} 
        
        // Public Types
		// =====================================================
        
        // ---------------------------------
        /// @name Delegate Types
        /// @{
        
        /// Multicast delegate type for general mouse events
		typedef Gdk::MulticastDelegate0<void>						MouseEventHandler;
        
        /// Multicast delegate type for mouse movement events
		typedef Gdk::MulticastDelegate1<void, MouseMoveArgs*>		MouseMoveEventHandler;
        
        /// Multicast delegate type for mouse button events
		typedef Gdk::MulticastDelegate1<void, MouseButton::Enum>	MouseButtonEventHandler;
        
        /// Multicase delegate type for mouse wheel events
		typedef Gdk::MulticastDelegate2<void, float, float>			MouseWheelEventHandler;
        
        /// @}
        
        // Public Events
		// =====================================================
        
        /// @name Events
        /// @{
        
		/// @brief
		///     This event is raised when the mouse is moved.
        /// @remarks
        ///     This event will only be raised if the mouse is within the application window bounds.
        static MouseMoveEventHandler	MouseMove;
        
        /// @brief
		///     This event is raised when a mouse button is pressed
		static MouseButtonEventHandler	MouseButtonDown;
        
        /// @brief
		///     This event is raised when a mouse button is released
		static MouseButtonEventHandler	MouseButtonUp;
        
        /// @brief
		///     This event is raised when the mouse scroll wheel is moved
        /// @remarks
        ///     The first parameter is the delta X value of the mouse wheel movement
        ///     The second parameter is the delta Y value of the mouse wheel movement
		static MouseWheelEventHandler	MouseWheelScroll;
        
        /// @brief
		///     This event is raised when a mouse enters the application window bounds
		static MouseEventHandler		MouseEnter;
        
        /// @brief
		///     This event is raised when a mouse leaves the application window bounds
		static MouseEventHandler		MouseLeave;
        
        /// @}
        
	public:

		// INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
		
		static void Platform_ProcessMouseMove(int x, int y);
		static void Platform_ProcessMouseButtonDown(MouseButton::Enum button);
		static void Platform_ProcessMouseButtonUp(MouseButton::Enum button);
		static void Platform_ProcessMouseWheelScroll(float deltaX, float deltaY);
		static void Platform_ProcessMouseEnterApp();
		static void Platform_ProcessMouseLeaveApp();

        /// @}
        
        /// @endcond
        
    private:
        
        // Internal Properties
		// ================================
        
        // Current state
		static int mouseX, mouseY;
		static bool buttonsDown[MouseButton::MAX_BUTTONS];
		static bool mouseIsOverApp;
        
        // Internal Methods
		// ================================
        
		static void Init();
		static void Update(float elapsedSeconds);
	    
		friend class Application;

	};
    
    /// @}

} // namespace Gdk

