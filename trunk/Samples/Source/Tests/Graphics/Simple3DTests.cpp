/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "Simple3DTests.h"

// ***********************************************************************
Simple3DTests::Simple3DTests()
{
	// Setup the camera properties
    viewLongitudeAngle = Math::PI * 0.35f;
    viewLatitudeAngle = Math::PI * 0.2;
    viewDistance = 10.0f;
    
    // Bind to input events
    TouchInput::TouchMoved.AddHandlerMethod(this, &Simple3DTests::OnTouchMoved);
    Mouse::MouseMove.AddHandlerMethod(this, &Simple3DTests::OnMouseMoved);
    
}

// ***********************************************************************
Simple3DTests::~Simple3DTests()
{
}

// ***********************************************************************
void Simple3DTests::OnUpdate(float elapsedSeconds)
{
	// Update the camera
	// ------------------------------------
    
    // Calculate the camera's position
    float cosLatitude = Math::Cos(viewLatitudeAngle);
    cameraPosition.X = Math::Cos(viewLongitudeAngle) * cosLatitude * viewDistance;
    cameraPosition.Y = Math::Sin(viewLatitudeAngle) * viewDistance;
    cameraPosition.Z = Math::Sin(viewLongitudeAngle) * cosLatitude * viewDistance;
    
    // Calculate the camera's target & direction
    cameraLookAt = Vector3::ZERO;
    cameraDirection = cameraLookAt - cameraPosition;
    cameraDirection.Normalize();
    
    // Calculate the projection matrix
	projection.MakeIdentity();
	projection.PerspectiveFOV(
        Math::PI * 0.4f,
        Application::GetWidth() / (float)Application::GetHeight(),
        1.0f, 100.0f
        );
    
	// Calculate the view matrix
	view.MakeIdentity();
	view.LookAt(cameraPosition, cameraLookAt, Vector3::UNIT_Y);
}

// ***********************************************************************
void Simple3DTests::OnDraw()
{
	// Clear the background
	Graphics::Clear(
		(ClearBuffers::Enum) (ClearBuffers::Color | ClearBuffers::Depth), 
		Color::FromNormalized(0.2f, 0.2f, 0.2f, 1.0f),
		1.0f
		);
	
	// Setup the view & projection matrices
	// ----------------------------------------------------

    Graphics::GlobalUniforms.View->SetMatrix4(view);
	Graphics::GlobalUniforms.Projection->SetMatrix4(projection);
    
	// Setup the camera parameters
	Graphics::GlobalUniforms.WorldUp->SetFloat3(Vector3(0,1,0));
	Graphics::GlobalUniforms.CameraPosition->SetFloat3(cameraPosition);

	// Setup lighting
	// ----------------------------------------------------

    // Ambient Light
	Graphics::GlobalUniforms.AmbientLight->SetFloat3(0.2f, 0.2f, 0.2f);
    
	// Set the number of active lights
	Graphics::GlobalUniforms.NumActiveLights->SetInt(1);
    
	// Setup the Main Light 
	Graphics::GlobalUniforms.LightPositionsAndFalloffs->SetFloat4(
        10.0f, 5.0f, 10.0f,     // Light position
        20.0f,                  // Light falloff distance
        0);                     // Light[] Index = 0
    Graphics::GlobalUniforms.LightColors->SetFloat3(
        1.0f, 1.0f, 1.0f
        );
    
    // Basic Model Drawing
	// ----------------------------------------------------

    // Draw the TestAxis model at the origin
	Matrix3D world;
	Graphics::GlobalUniforms.World->SetMatrix4(world);
    SharedAssets::Models.TestAxis->Draw();
}

// ***********************************************************************
void Simple3DTests::OnTouchMoved(Gdk::Touch *touch)
{
    // Get the touch movement (in pixels)
    Vector2 touchDelta = touch->GetPosition() - touch->GetPreviousPosition();
    
    // Convert the distance from pixel coordinates to screen symmetric coordinates
    touchDelta.X /= Graphics::GetScreenWidth();
    touchDelta.Y /= Graphics::GetScreenHeight();
    
    // Update the camera position
    viewLongitudeAngle += touchDelta.X * 3.0f;
    viewLatitudeAngle = Math::Clamp(viewLatitudeAngle + touchDelta.Y * 3.0f, -Math::PI * 0.49f, Math::PI * 0.49f);
}

// ***********************************************************************
void Simple3DTests::OnMouseMoved(MouseMoveArgs* args)
{
    // Is the mouse down?
    if(Mouse::IsButtonDown(MouseButton::Left))
    {
        // Get the mouse movement (in pixels)
        Vector2 mouseDelta(args->NewX - args->PreviousX, args->NewY - args->PreviousY);
        
        // Convert the distance from pixel coordinates to screen symmetric coordinates
        mouseDelta.X /= Graphics::GetScreenWidth();
        mouseDelta.Y /= Graphics::GetScreenHeight();
        
        // Update the camera position
        viewLongitudeAngle += mouseDelta.X * 3.0f;
        viewLatitudeAngle = Math::Clamp(viewLatitudeAngle + mouseDelta.Y * 3.0f, -Math::PI * 0.49f, Math::PI * 0.49f);
    }
}


