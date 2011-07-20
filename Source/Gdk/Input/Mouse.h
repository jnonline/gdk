/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Math/2D/Vector2.h"
#include "../System/Delegates.h"

namespace Gdk
{
	// ============================================================================

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

    // ============================================================================

    struct MouseMoveArgs
    {
        int NewX;
        int NewY;
        int PreviousX;
        int PreviousY;
        
        // CTor
        MouseMoveArgs(int newX, int newY, int previousX, int previousY) 
            : NewX(newX), NewY(newY), PreviousX(previousX), PreviousY(previousY)
        {}
    };
    
	// ============================================================================

	class Mouse
	{
	private:
		Mouse();
	    
		// Application Interface Methods
		// -----------------------------------

		friend class Application;

		// Init / Update
		static void Init();
		static void Update(float elapsedSeconds);

		// Internal Properties
		// -------------------------

		// Current state
		static int mouseX, mouseY;
		static bool buttonsDown[MouseButton::MAX_BUTTONS];
		static bool mouseIsOverApp;

	public:

		// Public Platform Interfaces
		// ----------------------------------

		// Input processing
		static void Platform_ProcessMouseMove(int x, int y);
		static void Platform_ProcessMouseButtonDown(MouseButton::Enum button);
		static void Platform_ProcessMouseButtonUp(MouseButton::Enum button);
		static void Platform_ProcessMouseWheelScroll(float deltaX, float deltaY);
		static void Platform_ProcessMouseEnterApp();
		static void Platform_ProcessMouseLeaveApp();

		// Public Interfaces for Games
		// -----------------------------------

		// Get - methods
		static Vector2 GetPosition();
		static int GetX();
		static int GetY();
		static bool IsButtonDown(MouseButton::Enum button);
		static bool IsMouseOverApp();

		// Set - methods
		static void SetPosition(int x, int y);
		static void ShowCursor(bool show);		

		// Delegates
		typedef Gdk::MulticastDelegate0<void>						MouseEventHandler;
		typedef Gdk::MulticastDelegate1<void, MouseMoveArgs*>		MouseMoveEventHandler;
		typedef Gdk::MulticastDelegate1<void, MouseButton::Enum>	MouseButtonEventHandler;
		typedef Gdk::MulticastDelegate2<void, float, float>			MouseWheelEventHandler;

		// Events
		static MouseMoveEventHandler	MouseMove;
		static MouseButtonEventHandler	MouseButtonDown;
		static MouseButtonEventHandler	MouseButtonUp;
		static MouseWheelEventHandler	MouseWheelScroll;
		static MouseEventHandler		MouseEnter;
		static MouseEventHandler		MouseLeave;

	};

} // namespace Gdk

