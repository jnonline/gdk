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
bool Mouse::buttonsDown[MouseButton::MAX_BUTTONS];
bool Mouse::mouseIsOverApp = false;

// Events
Mouse::MouseMoveEventHandler	Mouse::MouseMove;
Mouse::MouseButtonEventHandler	Mouse::MouseButtonDown;
Mouse::MouseButtonEventHandler	Mouse::MouseButtonUp;
Mouse::MouseWheelEventHandler	Mouse::MouseWheelScroll;
Mouse::MouseEventHandler		Mouse::MouseEnter;
Mouse::MouseEventHandler		Mouse::MouseLeave;

// ***********************************************************************
void Mouse::Init()
{
	// Set the mouse buttons to all up
	for(int i=0; i<MouseButton::MAX_BUTTONS; i++)
		Mouse::buttonsDown[i] = false;
}

// ***********************************************************************
void Mouse::Update(float elapsedSeconds)
{
}

// ***********************************************************************
void Mouse::Platform_ProcessMouseMove(int x, int y)
{
    MouseMoveArgs args(x, y, mouseX, mouseY);

    // Set the new mouse position
    mouseX = x;
	mouseY = y;

	// Call the event
    MouseMove.Invoke(&args);
}

// ***********************************************************************
void Mouse::Platform_ProcessMouseButtonDown(MouseButton::Enum button)
{
	buttonsDown[(int)button] = true;

	// Call the event
	MouseButtonDown.Invoke(button);
}

// ***********************************************************************
void Mouse::Platform_ProcessMouseButtonUp(MouseButton::Enum button)
{
	buttonsDown[(int)button] = false;

	// Call the event
	MouseButtonUp.Invoke(button);
}

// ***********************************************************************
void Mouse::Platform_ProcessMouseWheelScroll(float deltaX, float deltaY)
{
	// Call the event
	MouseWheelScroll.Invoke(deltaX, deltaY);
}

// ***********************************************************************
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

// ***********************************************************************
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



// ***********************************************************************
Vector2 Mouse::GetPosition()
{
	return Vector2((float)mouseX, (float)mouseY);
}

// ***********************************************************************
int Mouse::GetX()
{
	return mouseX;
}

// ***********************************************************************
int Mouse::GetY()
{
	return mouseY;
}

// ***********************************************************************
bool Mouse::IsButtonDown(MouseButton::Enum button)
{
	return buttonsDown[(int)button];
}

// ***********************************************************************
bool Mouse::IsMouseOverApp()
{
	return mouseIsOverApp;
}

// **********************************************************************
void Mouse::SetPosition(int x, int y)
{
	_Gdk_Platform_Mouse_SetPosition(x,y);
}

// ***********************************************************************
void Mouse::ShowCursor(bool show)
{
	_Gdk_Platform_Mouse_ShowCursor(show);
}

