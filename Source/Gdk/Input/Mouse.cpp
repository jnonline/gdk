/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Mouse.h"

using namespace Gdk;

// Static instantiations
int Mouse::mouseX = 0;
int Mouse::mouseY = 0;
bool Mouse::buttonDown[MouseButton::MAX_BUTTONS];
bool Mouse::buttonStateChanged[MouseButton::MAX_BUTTONS];
bool Mouse::mouseIsOverApp = false;

// Events
Mouse::MouseMoveEventHandler	Mouse::MouseMove;
Mouse::MouseButtonEventHandler	Mouse::MouseButtonDown;
Mouse::MouseButtonEventHandler	Mouse::MouseButtonUp;
Mouse::MouseWheelEventHandler	Mouse::MouseWheelScroll;
Mouse::MouseEventHandler		Mouse::MouseEnter;
Mouse::MouseEventHandler		Mouse::MouseLeave;

// *****************************************************************
/// @brief
///     Initializes the static Mouse class internal data
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Init()
{
	// Set the mouse buttons to all up
	for(int i=0; i<MouseButton::MAX_BUTTONS; i++)
    {
        Mouse::buttonStateChanged[i] = false;
		Mouse::buttonDown[i] = false;
    }
}

// *****************************************************************
/// @brief
///     Tells the GDK to update any internal mouse states
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Update(float elapsedSeconds)
{

}

// *****************************************************************
/// @brief
///     Tells the GDK to update any post-game-update internal mouse states
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::PostUpdate(float elapsedSeconds)
{
    // Loop through the mouse buttons
    for(int i=0; i<MouseButton::MAX_BUTTONS; i++)
    {
        // Unset the state-changed flag
        buttonStateChanged[i] = false;
	}
}

// *****************************************************************
/// @brief
///     Tells the GDK that the mouse has been moved
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Platform_ProcessMouseMove(int x, int y)
{
    MouseMoveArgs args(x, y, mouseX, mouseY);

    // Set the new mouse position
    mouseX = x;
	mouseY = y;

	// Call the event
    MouseMove.Invoke(&args);
}

// *****************************************************************
/// @brief
///     Tells the GDK that a mouse button has been pressed
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Platform_ProcessMouseButtonDown(MouseButton::Enum button)
{
    // Did the state change?
    if(buttonDown[(int)button] == false)
        buttonStateChanged[(int)button] = true;
	buttonDown[(int)button] = true;

	// Call the event
	MouseButtonDown.Invoke(button);
}

// *****************************************************************
/// @brief
///     Tells the GDK that a mouse button has been released
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Platform_ProcessMouseButtonUp(MouseButton::Enum button)
{
    // Did the state change?
    if(buttonDown[(int)button] == true)
        buttonStateChanged[(int)button] = true;
	buttonDown[(int)button] = false;

	// Call the event
	MouseButtonUp.Invoke(button);
}

// *****************************************************************
/// @brief
///     Tells the GDK that the mouse wheel has been scrolled
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Platform_ProcessMouseWheelScroll(float deltaX, float deltaY)
{
	// Call the event
	MouseWheelScroll.Invoke(deltaX, deltaY);
}

// *****************************************************************
/// @brief
///     Tells the GDK that the mouse has entered the application window
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Platform_ProcessMouseEnterApp()
{
    // Are we not currently in the app?
    if(mouseIsOverApp == false)
    {
        mouseIsOverApp = true;

        // Call the event
        MouseEnter.Invoke();
    }
}

// *****************************************************************
/// @brief
///     Tells the GDK that the mouse has left the application window
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Mouse::Platform_ProcessMouseLeaveApp()
{
    // Are we currently in the app?
    if(mouseIsOverApp == true)
    {
        mouseIsOverApp = false;

        // Call the event
        MouseLeave.Invoke();
    }
}

// *****************************************************************
/// @brief
///     Gets the current position of the mouse
/// @remarks
///     Returns the X,Y position of the mouse, within the application window.
// *****************************************************************
Vector2 Mouse::GetPosition()
{
	return Vector2((float)mouseX, (float)mouseY);
}

// *****************************************************************
/// @brief
///     Gets the current X position of the mouse
// *****************************************************************
int Mouse::GetX()
{
	return mouseX;
}

// *****************************************************************
/// @brief
///     Gets the current Y position of the mouse
// *****************************************************************
int Mouse::GetY()
{
	return mouseY;
}

// *****************************************************************
/// @brief
///     Returns true if the given mouse button is currently pressed
/// @param button
///     Mouse button to check the state of
// *****************************************************************
bool Mouse::IsButtonDown(MouseButton::Enum button)
{
	return buttonDown[(int)button];
}

// *****************************************************************
/// @brief
///     Returns true if the mouse is currently over the application window
// *****************************************************************
bool Mouse::IsMouseOverApp()
{
	return mouseIsOverApp;
}

// *****************************************************************
/// @brief
///     Returns true if the specified mouse button was just pressed
// *****************************************************************
bool Mouse::IsButtonJustPressed(MouseButton::Enum button)
{
    return buttonDown[(int)button] == true && 
           buttonStateChanged[(int)button] == true;
}

// *****************************************************************
/// @brief
///     Returns true if the specified mouse button was just released
// *****************************************************************
bool Mouse::IsButtonJustReleased(MouseButton::Enum button)
{
    return buttonDown[(int)button] == false && 
           buttonStateChanged[(int)button] == true;
}

// *****************************************************************
/// @brief
///     Moves the mouse to the given position within the application window
/// @param x
///     New X position of the mouse within the application window
/// @param y
///     New Y position of the mouse within the application window
// *****************************************************************
void Mouse::SetPosition(int x, int y)
{
	_Gdk_Platform_Mouse_SetPosition(x,y);
}

// *****************************************************************
/// @brief
///     Shows or Hides the mouse cursor (when it is over the application window)
/// @param show
///     Specify true to show the cursor, false to hide the cursor
// *****************************************************************
void Mouse::ShowCursor(bool show)
{
	_Gdk_Platform_Mouse_ShowCursor(show);
}

