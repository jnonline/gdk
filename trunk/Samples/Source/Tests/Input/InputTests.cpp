
// Includes
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
    proj.MakeIdentity();
	proj.OrthoOffCenter(
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
        (ClearBuffers::Enum) (ClearBuffers::Color), 
        Color::FromNormalized(0.1f, 0.5f, 0.4f, 1.0f),
        1.0f
        );
	
	// Apply our projection matrix
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

    // Keyboard Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Keyboard:" header
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Keyboard:", Vector2(-9.8f, 9.7f), Color(196,255,240), Vector2(0.03f, -0.03f));

    // Draw a box for various keys, showing the status of each key
    DrawKeyStatusBox(Keys::LeftShift,    Rectangle2(-9.8f, 7.7f, 2.5f, 1.0f), L"LShift");
    DrawKeyStatusBox(Keys::LeftAlt,      Rectangle2(-9.8f, 6.5f, 2.5f, 1.0f), L"LAlt");
    DrawKeyStatusBox(Keys::LeftControl,  Rectangle2(-9.8f, 5.3f, 2.5f, 1.0f), L"LCtrl");
    DrawKeyStatusBox(Keys::LeftGUI,      Rectangle2(-9.8f, 4.1f, 2.5f, 1.0f), L"LGui");

    DrawKeyStatusBox(Keys::RightShift,   Rectangle2(-7.1f, 7.7f, 2.5f, 1.0f), L"RShift");
    DrawKeyStatusBox(Keys::RightAlt,     Rectangle2(-7.1f, 6.5f, 2.5f, 1.0f), L"RAlt");
    DrawKeyStatusBox(Keys::RightControl, Rectangle2(-7.1f, 5.3f, 2.5f, 1.0f), L"RCtrl");
    DrawKeyStatusBox(Keys::RightGUI,     Rectangle2(-7.1f, 4.1f, 2.5f, 1.0f), L"RGui");

    DrawKeyStatusBox(Keys::Left,         Rectangle2(-4.4f, 7.7f, 2.5f, 1.0f), L"Left");
    DrawKeyStatusBox(Keys::Right,        Rectangle2(-4.4f, 6.5f, 2.5f, 1.0f), L"Right");
    DrawKeyStatusBox(Keys::Up,           Rectangle2(-4.4f, 5.3f, 2.5f, 1.0f), L"Up");
    DrawKeyStatusBox(Keys::Down,         Rectangle2(-4.4f, 4.1f, 2.5f, 1.0f), L"Down");

    // Mouse Input Tests
	// ----------------------------------------------------------------------------------------------

    // "Mouse:" header
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Mouse:", Vector2(0.0f, 9.7f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Draw the mouse coordinates
    wchar_t temp[256];
    swprintf(temp, 256, L"Position: %d, %d", Mouse::GetX(), Mouse::GetY());
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, temp, Vector2(0.0f, 8.8f), Color(255,224,160), Vector2(0.04f, -0.04f));

    // Draw the mouse over state
    if(Mouse::IsMouseOverApp())
        Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Mouse is Inside", Vector2(0.0f, 7.8f), Color(255,224,160), Vector2(0.04f, -0.04f));
    else
        Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Mouse is Outside", Vector2(0.0f, 7.8f), Color(255,224,160), Vector2(0.04f, -0.04f));
    
    // Draw a box for the various mouse buttons, showing the status of each one
    DrawMouseButtonStatusBox(MouseButton::Left,    Rectangle2(0.0f, 5.5f, 2.5f, 1.0f), L"Left");
    DrawMouseButtonStatusBox(MouseButton::Middle,  Rectangle2(2.7f, 5.5f, 2.5f, 1.0f), L"Middle");
    DrawMouseButtonStatusBox(MouseButton::Right,   Rectangle2(5.4f, 5.5f, 2.5f, 1.0f), L"Right");
    DrawMouseButtonStatusBox(MouseButton::Button4, Rectangle2(0.0f, 4.3f, 1.5f, 1.0f), L"B4");
    DrawMouseButtonStatusBox(MouseButton::Button5, Rectangle2(1.7f, 4.3f, 1.5f, 1.0f), L"B5");
    DrawMouseButtonStatusBox(MouseButton::Button6, Rectangle2(3.4f, 4.3f, 1.5f, 1.0f), L"B6");
    DrawMouseButtonStatusBox(MouseButton::Button7, Rectangle2(5.1f, 4.3f, 1.5f, 1.0f), L"B7");
    DrawMouseButtonStatusBox(MouseButton::Button8, Rectangle2(6.8f, 4.3f, 1.5f, 1.0f), L"B8");
    
    // Draw a circle at the mouse cursor
    Vector2 mouseWorldPosition = ScreenToWorld(Mouse::GetPosition());
    Renderer2D::DrawCircle(Circle2(mouseWorldPosition, 0.5f), 12, Color(0,128,255,128));
    
    // Touch Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Touch:" header
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Touch:", Vector2(0.0f, 3.6f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Get all the active touches
    vector<Touch>& touches = TouchInput::GetAllTouches();
    
    // Draw a string for the number of active touches
    swprintf(temp, 256, L"Active Touches: %d", touches.size());
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, temp, Vector2(0.0f, 2.8f), Color(255,224,160), Vector2(0.04f, -0.04f));
    
    // Draw a circle at each touch position
    for(vector<Touch>::iterator iter = touches.begin(); iter != touches.end(); iter++)
    {
        // Get a reference to this touch
        Touch& touch = *iter;
        Vector2 touchWorldPosition = ScreenToWorld(touch.GetPosition());
        
        // Draw a circle at this position
        Renderer2D::DrawCircle(Circle2(touchWorldPosition, 0.5f), 12, Color(0,255,128,128));
    }
    
    // Motion Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Motion:" header
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Motion:", Vector2(-9.8f, 3.3f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Get the accelerometer value
    Vector3 acceleration = MotionInput::GetAccelerometerAcceleration();
    
    // Display the accelerometer values
    swprintf(temp, 256, 
        L"Accelerometer X: %.4f\nAccelerometer Y: %.4f\nAccelerometer Z: %.4f", 
        acceleration.X, acceleration.Y, acceleration.Z
        );
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, temp, Vector2(-9.8f, 2.5f), Color(255,224,160), Vector2(0.04f, -0.04f));
    
    // Is the device shaking?
    Vector2 shakingTextPos(-9.8f, -0.2f);
    if(shakingTimer > 0.0f)
    {
        float shakeExtent = 0.06f;
        shakingTextPos += Vector2(Random::FromInterval(-shakeExtent, shakeExtent), Random::FromInterval(-shakeExtent, shakeExtent));
        Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"SHAKING", shakingTextPos, Color(255,128,128), Vector2(0.04f, -0.04f));
    }
    else
    {
        Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Not Shaking", shakingTextPos, Color(224,200,80), Vector2(0.04f, -0.04f));
    }
    
    // Device Input Tests
	// ----------------------------------------------------------------------------------------------
    
    // "Device:" header
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Device:", Vector2(-9.8f, -1.5f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Display the orientation
    swprintf(temp, 256, L"Orient: %hs", 
        DeviceOrientation::ToString(Device::GetOrientation())
        );
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, temp, Vector2(-9.8f, -2.4f), Color(255,224,160), Vector2(0.03f, -0.03f));
    
    // Display the battery state
    swprintf(temp, 256, L"Battery: %hs (%d%)", 
        DeviceBatteryState::ToString(Device::GetBatteryState()), 
        (int)(Device::GetBatteryLevel() * 100.0f)
        );
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, temp, Vector2(-9.8f, -3.3f), Color(255,224,160), Vector2(0.03f, -0.03f));
    
    
    // Input Log
	// ----------------------------------------------------------------------------------------------
    
    // "Motion:" header
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, L"Input Logs:", Vector2(0.0f, 1.4f), Color(196,255,240), Vector2(0.03f, -0.03f));
    
    // Loop through the input log lines
    float logY = 0.6f;
    for(list<InputLogLine>::iterator logIter = inputLog.begin(); logIter != inputLog.end(); logIter++)
    {
        // Write out this line
        InputLogLine &logLine = *logIter;
        Renderer2D::DrawText(
             SharedAssets::Fonts.Arial20, 
             logLine.Message.c_str(), 
             Vector2(0.0f, logY), 
             logLine.Color, 
             Vector2(0.03f, -0.03f)
             );
        logY -= 0.8f;
    }

    
	// Flush the 2D renderer
	Renderer2D::Flush();
}

// ***********************************************************************
void InputTestsModule::WriteLog(wchar_t* msg, Gdk::Color color)
{
    // Add a log entry for this touch event
    inputLog.push_back(InputLogLine(msg, color));
    
    // Keep the log at <= 8 entries
    if(inputLog.size() > 8)
        inputLog.pop_front();
}

// ***********************************************************************
void InputTestsModule::DrawKeyStatusBox(Keys::Enum key, Rectangle2 rect, wchar_t* name)
{
    // Was the key just now pressed?
    if(Keyboard::IsKeyJustPressed(key))
        Renderer2D::DrawRectangle(rect, Color(128,255,255,196));
    // Is the key toggled?
    else if(Keyboard::IsKeyToggled(key))
        Renderer2D::DrawRectangle(rect, Color(128,128,128,196));
    else
        Renderer2D::DrawRectangle(rect, Color(64,64,64,128));
    
    
    
    // Is the key held down?
    if(Keyboard::IsKeyDown(key))
        Renderer2D::DrawRectangleOutline(rect, Color(196,196,196,255));
    else
        Renderer2D::DrawRectangleOutline(rect, Color(0,0,0,255));
    
    
        
    // Draw the display name for this key
    Vector2 textPosition = rect.Position + Vector2(0.02f, rect.Size.Y - 0.02f);
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, name, textPosition, Color(255,224,160), Vector2(0.04f, -0.04f));
}

// ***********************************************************************
void InputTestsModule::DrawMouseButtonStatusBox(MouseButton::Enum button, Rectangle2 rect, wchar_t* name)
{
    // Is the button down
    if(Mouse::IsButtonDown(button))
        Renderer2D::DrawRectangle(rect, Color(128,128,128,196));
    else
        Renderer2D::DrawRectangle(rect, Color(64,64,64,128));
    
    Renderer2D::DrawRectangleOutline(rect, Color(0,0,0,255));
    
    // Draw the display name for this button
    Vector2 textPosition = rect.Position + Vector2(0.02f, rect.Size.Y - 0.02f);
    Renderer2D::DrawText(SharedAssets::Fonts.Arial20, name, textPosition, Color(255,224,160), Vector2(0.04f, -0.04f));
}

// ***********************************************************************
void InputTestsModule::OnWheelScroll(float deltaX, float deltaY)
{
	// Write a log entry for this event
	wchar_t temp[64];
	swprintf(temp, 64, L"Mouse Wheel Scroll: %.3f%", deltaY);
    WriteLog(temp, Color(80,224,224));
}

// ***********************************************************************
void InputTestsModule::OnTouchBegan(Gdk::Touch *touch)
{
    // Add a log entry for this touch event
    WriteLog(L"Touch Began", Color(224,200,80));
}

// ***********************************************************************
void InputTestsModule::OnTouchEnded(Gdk::Touch *touch)
{
    // Add a log entry for this touch event
    WriteLog(L"Touch Ended", Color(224,200,80));
}

// ***********************************************************************
void InputTestsModule::OnShakingBegan()
{
    // Add a log entry for this touch event
    WriteLog(L"Shaking Began", Color(224,120,80));
    shakingTimer = 0.21f;   // Continue displaying the 'shaking' effect for a bit of time
}

// ***********************************************************************
void InputTestsModule::OnShakingEnded()
{
    // Add a log entry for this touch event
    WriteLog(L"Shaking Ended", Color(224,120,80));
}

// ***********************************************************************
void InputTestsModule::OnMediaControlAction(MediaControlActions::Enum action)
{
    // Add a log entry for this event
    wchar_t temp[64];
    swprintf(temp, 64, L"%hs", MediaControlActions::ToString(action));
    WriteLog(temp, Color(200,224,80));
}

// ***********************************************************************
Vector2 InputTestsModule::ScreenToWorld(Vector2 screenCoordinate)
{
    // First:  Convert the screen position to symmetric coordinates (-1 to 1)
    Vector2 symmetricPosition = Graphics::ScreenToSymmetric(screenCoordinate);
    
    // Second:  Convert the symmetric position to world coordinates
    // by multiplying through the inverse of the projection matrix
    //    (Z is ignored because we're using an orthographic projection matrix)
    Vector3 worldPosition = inverseProj.TransformVertex(Vector3(symmetricPosition, 0.0f));
    
    // Return the XY world coordinate
    return worldPosition.GetXY();
}

