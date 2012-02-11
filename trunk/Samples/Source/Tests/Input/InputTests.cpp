/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "InputTests.h"

// ***********************************************************************
InputTestsModule::InputTestsModule()
{
    shakingTimer = 0.0f;
    
	// Bind to mouse events
	Mouse::MouseWheelScroll.AddHandlerMethod(this, &InputTestsModule::OnWheelScroll);

    // Bind to touch events
    TouchInput::TouchBegan.AddHandlerMethod(this, &InputTestsModule::OnTouchBegan);
    TouchInput::TouchEnded.AddHandlerMethod(this, &InputTestsModule::OnTouchEnded);
    
    // Bind to motion events
    MotionInput::ShakingBegan.AddHandlerMethod(this, &InputTestsModule::OnShakingBegan);
    MotionInput::ShakingEnded.AddHandlerMethod(this, &InputTestsModule::OnShakingEnded);
    
    // Bind to events from the device input
    Device::MediaControlAction.AddHandlerMethod(this, &InputTestsModule::OnMediaControlAction);
}

// ***********************************************************************
InputTestsModule::~InputTestsModule()
{
	// Un-Bind mouse events
	Mouse::MouseWheelScroll.RemoveHandlerMethod(this, &InputTestsModule::OnWheelScroll);

    // Un-Bind touch events
    TouchInput::TouchBegan.RemoveHandlerMethod(this, &InputTestsModule::OnTouchBegan);
    TouchInput::TouchEnded.RemoveHandlerMethod(this, &InputTestsModule::OnTouchEnded);
    
    // Un-Bind motion events
    MotionInput::ShakingBegan.RemoveHandlerMethod(this, &InputTestsModule::OnShakingBegan);
    MotionInput::ShakingEnded.RemoveHandlerMethod(this, &InputTestsModule::OnShakingEnded);
    
    // Un-Bind from the device input events
    Device::MediaControlAction.RemoveHandlerMethod(this, &InputTestsModule::OnMediaControlAction);
}

// ***********************************************************************
void InputTestsModule::OnUpdate(float elapsedSeconds)
{
    // Calculate the projection matrix of the view
    proj = Matrix3D::CreateOrthoOffCenter(
        -10,10,				// left / right
        -10,10,				// bottom / top
        -1.0f, 1.0f				// far / near
        );

    // Get the inverse of the projection matrix
    inverseProj = proj.GetInverse();
    
    // Updating the 'shaking' timer
    if(shakingTimer > 0.0f && MotionInput::IsShaking() == false)
    {
        shakingTimer -= elapsedSeconds;
        if(shakingTimer < 0.0f)
            shakingTimer = 0.0f;
    }
}

// ***********************************************************************
void InputTestsModule::OnDraw()
{
	// Clear the background
	Graphics::Clear(
        true, false, false,
        Color::FromNormalized(0.1f, 0.5f, 0.4f, 1.0f),
        1.0f
        );
	
	// Apply our projection matrix
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

    // Keyboard Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Keyboard:" header
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Keyboard:", Vector2(-9.8f, 9.7f), Color(196,255,240), Vector2(0.03f, -0.03f));

    // Draw a box for various keys, showing the status of each key
    DrawKeyStatusBox(Keys::LeftShift,    Rectangle2(-9.8f, 7.7f, 2.5f, 1.0f), "LShift");
    DrawKeyStatusBox(Keys::LeftAlt,      Rectangle2(-9.8f, 6.5f, 2.5f, 1.0f), "LAlt");
    DrawKeyStatusBox(Keys::LeftControl,  Rectangle2(-9.8f, 5.3f, 2.5f, 1.0f), "LCtrl");
    DrawKeyStatusBox(Keys::LeftGUI,      Rectangle2(-9.8f, 4.1f, 2.5f, 1.0f), "LGui");

    DrawKeyStatusBox(Keys::RightShift,   Rectangle2(-7.1f, 7.7f, 2.5f, 1.0f), "RShift");
    DrawKeyStatusBox(Keys::RightAlt,     Rectangle2(-7.1f, 6.5f, 2.5f, 1.0f), "RAlt");
    DrawKeyStatusBox(Keys::RightControl, Rectangle2(-7.1f, 5.3f, 2.5f, 1.0f), "RCtrl");
    DrawKeyStatusBox(Keys::RightGUI,     Rectangle2(-7.1f, 4.1f, 2.5f, 1.0f), "RGui");

    DrawKeyStatusBox(Keys::Left,         Rectangle2(-4.4f, 7.7f, 2.5f, 1.0f), "Left");
    DrawKeyStatusBox(Keys::Right,        Rectangle2(-4.4f, 6.5f, 2.5f, 1.0f), "Right");
    DrawKeyStatusBox(Keys::Up,           Rectangle2(-4.4f, 5.3f, 2.5f, 1.0f), "Up");
    DrawKeyStatusBox(Keys::Down,         Rectangle2(-4.4f, 4.1f, 2.5f, 1.0f), "Down");

    // Mouse Input Tests
	// ----------------------------------------------------------------------------------------------

    // "Mouse:" header
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Mouse:", Vector2(0.0f, 9.7f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Draw the mouse coordinates
    char temp[256];
    GDK_SPRINTF(temp, 256, "Position: %d, %d", Mouse::GetX(), Mouse::GetY());
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, temp, Vector2(0.0f, 8.8f), Color(255,224,160), Vector2(0.04f, -0.04f));

    // Draw the mouse over state
    if(Mouse::IsMouseOverApp())
        Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Mouse is Inside", Vector2(0.0f, 7.8f), Color(255,224,160), Vector2(0.04f, -0.04f));
    else
        Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Mouse is Outside", Vector2(0.0f, 7.8f), Color(255,224,160), Vector2(0.04f, -0.04f));
    
    // Draw a box for the various mouse buttons, showing the status of each one
    DrawMouseButtonStatusBox(MouseButton::Left,    Rectangle2(0.0f, 5.5f, 2.5f, 1.0f), "Left");
    DrawMouseButtonStatusBox(MouseButton::Middle,  Rectangle2(2.7f, 5.5f, 2.5f, 1.0f), "Middle");
    DrawMouseButtonStatusBox(MouseButton::Right,   Rectangle2(5.4f, 5.5f, 2.5f, 1.0f), "Right");
    DrawMouseButtonStatusBox(MouseButton::Button4, Rectangle2(0.0f, 4.3f, 1.5f, 1.0f), "B4");
    DrawMouseButtonStatusBox(MouseButton::Button5, Rectangle2(1.7f, 4.3f, 1.5f, 1.0f), "B5");
    DrawMouseButtonStatusBox(MouseButton::Button6, Rectangle2(3.4f, 4.3f, 1.5f, 1.0f), "B6");
    DrawMouseButtonStatusBox(MouseButton::Button7, Rectangle2(5.1f, 4.3f, 1.5f, 1.0f), "B7");
    DrawMouseButtonStatusBox(MouseButton::Button8, Rectangle2(6.8f, 4.3f, 1.5f, 1.0f), "B8");
    
    // Draw a circle at the mouse cursor
    Vector2 mouseWorldPosition = ScreenToWorld(Mouse::GetPosition());
    Drawing2D::DrawCircle(Circle2(mouseWorldPosition, 0.5f), 12, Color(0,128,255,128));
    
    // Touch Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Touch:" header
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Touch:", Vector2(0.0f, 3.6f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Get all the active touches
    vector<Touch>& touches = TouchInput::GetAllTouches();
    
    // Display the number of active touches
    GDK_SPRINTF(temp, 256, "Active Touches: %lu", touches.size());
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, temp, Vector2(0.0f, 2.8f), Color(255,224,160), Vector2(0.04f, -0.04f));
    
    // Draw a circle at each touch position
    for(vector<Touch>::iterator iter = touches.begin(); iter != touches.end(); iter++)
    {
        // Get a reference to this touch
        Touch& touch = *iter;
        Vector2 touchWorldPosition = ScreenToWorld(touch.GetPosition());
        
        // Draw a circle at this position
        Drawing2D::DrawCircle(Circle2(touchWorldPosition, 0.5f), 12, Color(0,255,128,128));
    }
    
    // Motion Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Motion:" header
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Motion:", Vector2(-9.8f, 3.3f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Get the accelerometer value
    Vector3 acceleration = MotionInput::GetAccelerometerAcceleration();
    
    // Display the accelerometer values
    GDK_SPRINTF(temp, 256, 
        "Accelerometer X: %.4f\nAccelerometer Y: %.4f\nAccelerometer Z: %.4f", 
        acceleration.X, acceleration.Y, acceleration.Z
        );
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, temp, Vector2(-9.8f, 2.5f), Color(255,224,160), Vector2(0.04f, -0.04f));
    
    // Is the device shaking?
    Vector2 shakingTextPos(-9.8f, -0.2f);
    if(shakingTimer > 0.0f)
    {
        float shakeExtent = 0.06f;
        shakingTextPos += Vector2(Random::FromInterval(-shakeExtent, shakeExtent), Random::FromInterval(-shakeExtent, shakeExtent));
        Drawing2D::DrawText(SharedResources::Fonts.Arial20, "SHAKING", shakingTextPos, Color(255,128,128), Vector2(0.04f, -0.04f));
    }
    else
    {
        Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Not Shaking", shakingTextPos, Color(224,200,80), Vector2(0.04f, -0.04f));
    }
    
    // Device Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Device:" header
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Device:", Vector2(-9.8f, -1.5f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Display the orientation
    GDK_SPRINTF(temp, 256, "Orient: %s", 
        DeviceOrientation::ToString(Device::GetOrientation())
        );
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, temp, Vector2(-9.8f, -2.4f), Color(255,224,160), Vector2(0.03f, -0.03f));
    
    // Display the battery state
    GDK_SPRINTF(temp, 256, "Battery: %s (%d%%)", 
        DeviceBatteryState::ToString(Device::GetBatteryState()), 
        (int)(Device::GetBatteryLevel() * 100.0f)
        );
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, temp, Vector2(-9.8f, -3.3f), Color(255,224,160), Vector2(0.03f, -0.03f));
    
    
    // Input Log
	// ----------------------------------------------------------------------------------------------
    
    // "Motion:" header
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Input Logs:", Vector2(0.0f, 1.4f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Loop through the input log lines
    float logY = 0.6f;
    for(list<InputLogLine>::iterator logIter = inputLog.begin(); logIter != inputLog.end(); logIter++)
    {
        // Write out this line
        InputLogLine &logLine = *logIter;
        Drawing2D::DrawText(
             SharedResources::Fonts.Arial20, 
             logLine.Message.c_str(), 
             Vector2(0.0f, logY), 
             logLine.Color, 
             Vector2(0.03f, -0.03f)
             );
        logY -= 0.8f;
    }

    
	// Flush the 2D renderer
	Drawing2D::Flush();
}

// ***********************************************************************
void InputTestsModule::WriteLog(const char* msg, Gdk::Color color)
{
    // Add a log entry for this touch event
    inputLog.push_back(InputLogLine(msg, color));
    
    // Keep the log at <= 8 entries
    if(inputLog.size() > 8)
        inputLog.pop_front();
}

// ***********************************************************************
void InputTestsModule::DrawKeyStatusBox(Keys::Enum key, Rectangle2 rect, const char* name)
{
    // Was the key just now pressed?
    if(Keyboard::IsKeyJustPressed(key))
        Drawing2D::DrawRectangle(rect, Color(128,255,255,196));
    // Is the key toggled?
    else if(Keyboard::IsKeyToggled(key))
        Drawing2D::DrawRectangle(rect, Color(128,128,128,196));
    else
        Drawing2D::DrawRectangle(rect, Color(64,64,64,128));
    
    
    
    // Is the key held down?
    if(Keyboard::IsKeyDown(key))
        Drawing2D::DrawRectangleOutline(rect, Color(196,196,196,255));
    else
        Drawing2D::DrawRectangleOutline(rect, Color(0,0,0,255));
    
    
        
    // Draw the display name for this key
    Vector2 textPosition = rect.Position + Vector2(0.02f, rect.Size.Y - 0.02f);
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, name, textPosition, Color(255,224,160), Vector2(0.04f, -0.04f));
}

// ***********************************************************************
void InputTestsModule::DrawMouseButtonStatusBox(MouseButton::Enum button, Rectangle2 rect, const char* name)
{
    // Is the button down
    if(Mouse::IsButtonDown(button))
        Drawing2D::DrawRectangle(rect, Color(128,128,128,196));
    else
        Drawing2D::DrawRectangle(rect, Color(64,64,64,128));
    
    Drawing2D::DrawRectangleOutline(rect, Color(0,0,0,255));
    
    // Draw the display name for this button
    Vector2 textPosition = rect.Position + Vector2(0.02f, rect.Size.Y - 0.02f);
    Drawing2D::DrawText(SharedResources::Fonts.Arial20, name, textPosition, Color(255,224,160), Vector2(0.04f, -0.04f));
}

// ***********************************************************************
void InputTestsModule::OnWheelScroll(float deltaX, float deltaY)
{
	// Write a log entry for this event
	char temp[64];
	GDK_SPRINTF(temp, 64, "Mouse Wheel Scroll: %.3f", deltaY);
    WriteLog(temp, Color(80,224,224));
}

// ***********************************************************************
void InputTestsModule::OnTouchBegan(Gdk::Touch *touch)
{
    // Add a log entry for this touch event
    WriteLog("Touch Began", Color(224,200,80));
}

// ***********************************************************************
void InputTestsModule::OnTouchEnded(Gdk::Touch *touch)
{
    // Add a log entry for this touch event
    WriteLog("Touch Ended", Color(224,200,80));
}

// ***********************************************************************
void InputTestsModule::OnShakingBegan()
{
    // Add a log entry for this touch event
    WriteLog("Shaking Began", Color(224,120,80));
    shakingTimer = 0.21f;   // Continue displaying the 'shaking' effect for a bit of time
}

// ***********************************************************************
void InputTestsModule::OnShakingEnded()
{
    // Add a log entry for this touch event
    WriteLog("Shaking Ended", Color(224,120,80));
}

// ***********************************************************************
void InputTestsModule::OnMediaControlAction(MediaControlActions::Enum action)
{
    // Add a log entry for this event
    WriteLog(MediaControlActions::ToString(action), Color(200,224,80));
}

// ***********************************************************************
Vector2 InputTestsModule::ScreenToWorld(Vector2 screenCoordinate)
{
    // First:  Convert the screen position to symmetric coordinates (-1 to 1)
    Vector2 symmetricPosition = Graphics::ScreenToSymmetric(screenCoordinate);
    
    // Second:  Convert the symmetric position to world coordinates
    // by multiplying through the inverse of the projection matrix
    //    (Z is ignored because we're using an orthographic projection matrix)
    Vector3 worldPosition = inverseProj.TransformPoint(Vector3(symmetricPosition, 0.0f));
    
    // Return the XY world coordinate
    return worldPosition.GetXY();
}

