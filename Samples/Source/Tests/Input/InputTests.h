/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "../../Module.h"

// *******************************************************************
struct InputLogLine
{
    Gdk::Color Color;
    string Message;
    
    InputLogLine(const char* msg, Gdk::Color color) : Message(msg), Color(color) {}
};

// *******************************************************************
class InputTestsModule : public Module
{
public:
	// CTor / DTor
    InputTestsModule();
	~InputTestsModule();

	// Module overrides
	virtual void OnUpdate(float elapsedSeconds);
	virtual void OnDraw();

protected:
    
    // Input log
    list<InputLogLine> inputLog;
    void WriteLog(const char* msg, Gdk::Color color = Color(255,224,160));
    
    // Keyboard Utilities
    void DrawKeyStatusBox(Keys::Enum key, Rectangle2 rect, const char* name);
	
    // Mouse Utilities
    void DrawMouseButtonStatusBox(MouseButton::Enum button, Rectangle2 rect, const char* name);
	void OnWheelScroll(float deltaX, float deltaY);
    
    // Touch Utilities
    void OnTouchBegan(Touch* touch);
    void OnTouchEnded(Touch* touch);
    
    // Motion Utilities
    float shakingTimer;
    void OnShakingBegan();
    void OnShakingEnded();
    
    // Device input Utilities
    void OnMediaControlAction(MediaControlActions::Enum action);
    
    // Projection & Coordinate Conversion Utilities
    Matrix3D proj;
    Matrix3D inverseProj;
    Vector2 ScreenToWorld(Vector2 screenCoordinate);
};
