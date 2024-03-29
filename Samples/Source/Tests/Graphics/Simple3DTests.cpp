/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "Simple3DTests.h"

Vector3 camOffset = Vector3::ZERO;

// ***********************************************************************
Simple3DTests::Simple3DTests()
{
	// Setup the camera properties
    viewLongitudeAngle = Math::PI * 0.35f;
    viewLatitudeAngle	= Math::PI * 0.2f;
    viewDistance = 10.0f;
    
    // Bind to input events
    TouchInput::TouchMoved.AddHandlerMethod(this, &Simple3DTests::OnTouchMoved);
    Mouse::MouseMove.AddHandlerMethod(this, &Simple3DTests::OnMouseMove);
	Mouse::MouseWheelScroll.AddHandlerMethod(this, &Simple3DTests::OnMouseWheelScroll);
    
	// Load a test model
	skeletalModel = NULL;
	//skeletalModel = ModelManager::LoadModel("Models/GHModels/GunHack3");
    //resourcePool.Add(skeletalModel);

}

// ***********************************************************************
Simple3DTests::~Simple3DTests()
{
	// Release the resources
	resourcePool.Release();

    // UnBind to input events
    TouchInput::TouchMoved.RemoveHandlerMethod(this, &Simple3DTests::OnTouchMoved);
    Mouse::MouseMove.RemoveHandlerMethod(this, &Simple3DTests::OnMouseMove);
	Mouse::MouseWheelScroll.RemoveHandlerMethod(this, &Simple3DTests::OnMouseWheelScroll);
}

// ***********************************************************************
void Simple3DTests::OnUpdate(float elapsedSeconds)
{
	Vector2 move = Vector2::ZERO;
	float speed = viewDistance * 0.5f;
	if(Keyboard::IsKeyDown(Keys::LeftShift) || Keyboard::IsKeyDown(Keys::RightShift))
		speed *= 4.0f;
	if(Keyboard::IsKeyDown(Keys::W))
		move += Vector2::FromAngle(viewLongitudeAngle + Math::PI) * elapsedSeconds * speed;
	if(Keyboard::IsKeyDown(Keys::S))
		move += Vector2::FromAngle(viewLongitudeAngle) * elapsedSeconds * speed;
	if(Keyboard::IsKeyDown(Keys::D))
		move += Vector2::FromAngle(viewLongitudeAngle + Math::PI * 1.5f) * elapsedSeconds * speed;
	if(Keyboard::IsKeyDown(Keys::A))
		move += Vector2::FromAngle(viewLongitudeAngle + Math::PI * 0.5f) * elapsedSeconds * speed;

	camOffset.X += move.X;
	camOffset.Z += move.Y;

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
	projection = Matrix3D::CreatePerspectiveFOV(
        Math::PI * 0.4f,
        Application::GetWidth() / (float)Application::GetHeight(),
        1.0f, 100.0f + viewDistance * 4.0f
        );
    
	// Calculate the view matrix
	view = Matrix3D::CreateLookAt(cameraPosition + camOffset, cameraLookAt + camOffset, Vector3::UNIT_Y);
}

// ***********************************************************************
void Simple3DTests::OnDraw()
{
	// Clear the background
    Graphics::Clear(
		true, true, false, 
		Color::FromNormalized(0.2f, 0.2f, 0.2f, 1.0f),
		1.0f
		);
	
	// Setup the view & projection matrices
	// ----------------------------------------------------

    Graphics::GlobalUniforms.View->SetMatrix4(view);
	Graphics::GlobalUniforms.Projection->SetMatrix4(projection);
    
	// Setup the camera parameters
	Graphics::GlobalUniforms.WorldUp->SetFloat3(Vector3(0,1,0));
	Graphics::GlobalUniforms.CameraPosition->SetFloat3(cameraPosition + camOffset);

	// Setup lighting
	// ----------------------------------------------------

    // Ambient Light
	Graphics::GlobalUniforms.AmbientLight->SetFloat3(0.5f, 0.5f, 0.5f);
    
	// Set the number of active lights
	Graphics::GlobalUniforms.NumActiveLights->SetInt(1);
    
	// Setup the Main Light 
	Graphics::GlobalUniforms.LightPositionsAndFalloffs->SetFloat4(
        cameraPosition.X + camOffset.X, // Light position
		cameraPosition.Y + camOffset.Y, 
		cameraPosition.Z + camOffset.Z,     
        viewDistance * 2.0f,                  // Light falloff distance
        0);                     // Light[] Index = 0
    Graphics::GlobalUniforms.LightColors->SetFloat3(
        1.0f, 1.0f, 1.0f
        );
    
    // Basic Model Drawing
	// ----------------------------------------------------

    // Draw the TestAxis model at the origin
	SharedResources::Models.TestAxis->World = Matrix3D::IDENTITY;
    SharedResources::Models.TestAxis->Draw();

	// Draw the Skeletal model slightly to the right
	if(skeletalModel != NULL)
	{
		skeletalModel->World = Matrix3D::CreateTranslation(2.0f, 0.0f, 2.0f);
		skeletalModel->Draw();
	}
    
    // Draw several models with cumulative transforms
    // TODO(P1) more tests
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
void Simple3DTests::OnMouseMove(MouseMoveArgs* args)
{
    // Is the mouse down?
    if(Mouse::IsButtonDown(MouseButton::Left))
    {
        // Get the mouse movement (in pixels)
        Vector2 mouseDelta((float)args->NewX - args->PreviousX, (float)args->NewY - args->PreviousY);
        
        // Convert the distance from pixel coordinates to screen symmetric coordinates
        mouseDelta.X /= Graphics::GetScreenWidth();
        mouseDelta.Y /= Graphics::GetScreenHeight();
        
        // Update the camera position
        viewLongitudeAngle += mouseDelta.X * 3.0f;
        viewLatitudeAngle = Math::Clamp(viewLatitudeAngle + mouseDelta.Y * 3.0f, -Math::PI * 0.49f, Math::PI * 0.49f);
    }
}

// ***********************************************************************
void Simple3DTests::OnMouseWheelScroll(float deltaX, float deltaY)
{
	// zoom/unzoom the camera
	this->viewDistance -= deltaY * 0.5f;
	this->viewDistance = Math::Clamp(this->viewDistance, 2.0f, 100.0f);
}

