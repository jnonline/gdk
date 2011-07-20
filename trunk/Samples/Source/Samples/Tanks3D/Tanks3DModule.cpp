/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "Tanks3DModule.h"

// ***********************************************************************
Tanks3DModule::Tanks3DModule ()
{
	// Do game initialization here!
	// -----------------------------------------------

	AssetManager* assetManager = AssetManager::GetSingleton();

	// Load our test models
	ground = assetManager->Load<Model>("Models/Ground", &assetsPool, NULL);
	tank = assetManager->Load<Model>("Models/Tank", &assetsPool, NULL);

	// Setup the game objects
	playerDirection = 0.0f;
	playerPosition = Vector2::ZERO;

	// Add some enemies
	for(int i=0; i<20; i++)
	{
		EnemyTank enemy;
		enemy.Position = Vector2(Random::FromInterval(-50.0f, 50.0f), Random::FromInterval(-50.0f, 50.0f));
		enemy.Direction = Random::FromInterval(0.0f, Math::TWO_PI);
		enemies.push_back(enemy);
	}

	// Create a sprite (using the Gdk's builtin test animation)
	chopper = GdkNew Sprite(SharedAssets::AtlasAnimations.Chopper);

    // Bind to input event handlers
    Mouse::MouseMove.AddHandlerMethod(this, &Tanks3DModule::OnMouseMove);
}

// ***********************************************************************
Tanks3DModule::~Tanks3DModule()
{
	// Do game shutdown here
	// -------------------------------

	// UnBind to input event handlers
    Mouse::MouseMove.RemoveHandlerMethod(this, &Tanks3DModule::OnMouseMove);

	// Delete the sprite
	GdkDelete(chopper);

	// Release any assets in our pool
	assetsPool.Release();
}

// ***********************************************************************
void Tanks3DModule::OnUpdate(float elapsedSeconds)
{
	// Update Loop
	//   - Process time-based gameplay movement and updates here

	// Handle player tank movement!
	// ---------------------------------

	float turnSpeed = 1.0f;
	float forwardSpeed = 3.0f;
	float backSpeed = 1.0f;

	// A & D = Left & Right
	if(Keyboard::IsKeyDown(Keys::A))
		playerDirection -= Math::PI * turnSpeed * elapsedSeconds;
	if(Keyboard::IsKeyDown(Keys::D))
		playerDirection += Math::PI * turnSpeed * elapsedSeconds;

	// W & S = Forward / Backward
	if(Keyboard::IsKeyDown(Keys::W))
		playerPosition += Vector2::FromAngle(playerDirection) * elapsedSeconds * forwardSpeed;
	if(Keyboard::IsKeyDown(Keys::S))
		playerPosition -= Vector2::FromAngle(playerDirection) * elapsedSeconds * backSpeed;

    // First active touch, moves the tank
    vector<Touch>& touches = TouchInput::GetAllTouches();
    if(touches.size() > 0)
    {
        Touch& touch = touches[0];
        
        Vector2 pos = touch.GetPosition();
        pos = Graphics::ScreenToUnit(pos);
        if(pos.X < pos.Y)
        {
            // Bottom Left diagonal
            if((1.0f - pos.X) < pos.Y)
            {
                // Bottom
                playerPosition -= Vector2::FromAngle(playerDirection) * elapsedSeconds * backSpeed;
            }
            else
            {
                // Left
                playerDirection -= Math::PI * turnSpeed * elapsedSeconds;
            }
        }
        else
        {
            // Top Right diagonal
            if((1.0f - pos.X) < pos.Y)
            {
                // Right
                playerDirection += Math::PI * turnSpeed * elapsedSeconds;    
                
            }
            else
            {
                // Top
                playerPosition += Vector2::FromAngle(playerDirection) * elapsedSeconds * forwardSpeed;
            }
        }
        
        // Position the chopper on the 2nd touch
        if(touches.size() > 1)
            chopper->Position = touches[1].GetPosition();
    }
    
	// Update the emeny tanks
	// -----------------------------------

	// Loop through the enemy tanks
	for(size_t i=0; i<enemies.size(); i++)
	{
		EnemyTank& enemy = enemies[i];
		enemy.Position += Vector2::FromAngle(enemy.Direction) * elapsedSeconds;
		enemy.Direction += Random::FromInterval(-Math::PI, Math::PI) * 0.5f * elapsedSeconds;
	}

	// Update the 2D Overlay
	// --------------------------------

	chopper->Update(elapsedSeconds);
}

// ***********************************************************************
void Tanks3DModule::OnMouseMove(MouseMoveArgs* args)
{
    chopper->Position = Vector2((float)args->NewX, (float)args->NewY);
}

// ***********************************************************************
void Tanks3DModule::OnDraw()
{
	// Draw Loop
	//   - Do visual rendering here

	// Clear the background
	Graphics::Clear(
		(ClearBuffers::Enum) (ClearBuffers::Color | ClearBuffers::Depth), 
		Color::FromNormalized(0.5f, 0.8f, 0.8f, 1.0f),
		1.0f
		);

	// 3D Game World Rendering
	// -------------------------------------------------------------------

	// Calculate the projection matrix
	Matrix3D proj = Matrix3D::CreatePerspectiveFOV(
        Math::PI * 0.5f,
		Application::GetWidth() / (float)Application::GetHeight(),
		0.1f, 100.0f
		);
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

	// Calculate the view matrix (behind the player tank)
	Vector3 cameraTarget(playerPosition.X, 0.0f, playerPosition.Y);
	Vector2 viewDirection = Vector2::FromAngle(playerDirection);
	float cameraZoom = 2.0f;
	Vector3 cameraPosition = cameraTarget + Vector3(-viewDirection.X * cameraZoom, 2.5f * cameraZoom, -viewDirection.Y * cameraZoom);
	Matrix3D view = Matrix3D::CreateLookAt(cameraPosition, cameraTarget, Vector3::UNIT_Y);
    Vector3 worldUp(0,1,0);
	Graphics::GlobalUniforms.View->SetMatrix4(view);
	Graphics::GlobalUniforms.WorldUp->SetFloat3(worldUp);
	Graphics::GlobalUniforms.CameraPosition->SetFloat3(cameraPosition);

	// Setup ambient lighting
	Graphics::GlobalUniforms.AmbientLight->SetFloat3(0.5f, 0.8f, 0.8f, 0);

	// Add 1 dynamic Yellow light at the camera position
	Graphics::GlobalUniforms.NumActiveLights->SetInt(1);  // 0 = no lighting
	Graphics::GlobalUniforms.LightPositionsAndFalloffs->SetFloat4(cameraPosition.X, cameraPosition.Y, cameraPosition.Z, 15.0f, 0);
	Graphics::GlobalUniforms.LightColors->SetFloat3(1.0f, 1.0f, 0.0f, 0);
    
	// Draw the Ground
	ground->World = Matrix3D::CreateTranslation(0.0f, -0.3f, 0.0f);
	ground->Draw();
    
	// Draw the Player Tank
	tank->World = Matrix3D::CreateRotationY(-playerDirection - Math::PI * 0.5f);  // We invert the angle here, as the 2D to 3D conversion has Y = -Z (see Matrix3D.h for 3d coordinate orientations)
	tank->World.Translate(playerPosition.X, 0.0f, playerPosition.Y);
	tank->Draw();
    
	// Draw the Enemy tanks
	for(size_t i=0; i<enemies.size(); i++)
	{
		EnemyTank& enemy = enemies[i];
		tank->World = Matrix3D::CreateRotationY(-enemy.Direction - Math::PI * 0.5f);  // We invert the angle here, as the 2D to 3D conversion has Y = -Z (see Matrix3D.h for 3d coordinate orientations)
		tank->World.Translate(enemy.Position.X, 0.0f, enemy.Position.Y);
		tank->Draw();
	}
    
	// 2D Rendering
	// -------------------------------------------------------------------

	// Setup a 2D projection matrix to draw in screen coordinates
    proj = Matrix3D::CreateOrthoOffCenter(
		0, (float) Application::GetWidth(),					// left / right
		(float) Application::GetHeight(), 0,				// bottom / top
		-1.0f, 1.0f											// far / near
		);
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

	// Draw some Text
    wchar_t sampleText[256];
    swprintf(sampleText, 256, L"Sample Text...");
    Vector2 textSize = SharedAssets::Fonts.Arial20->GetTextSize(sampleText);
    Vector2 textPosition((Application::GetWidth() - textSize.X) * 0.5f, 10);
	Renderer2D::DrawText(SharedAssets::Fonts.Arial20, sampleText, textPosition, Color::BLUE);

	// Draw the chopper sprite
	Renderer2D::DrawSprite(chopper);
    
    // Draw some touch circles
    vector<Touch>& touches = TouchInput::GetAllTouches();
    for(vector<Touch>::iterator touchIter = touches.begin(); touchIter != touches.end(); touchIter++)
    {
        Touch& touch = *touchIter;
        Renderer2D::DrawCircle(Circle2(touch.GetPosition(), 20.0f), 16, Color::PURPLE);
    }

	// Flush the 2D renderer
	Renderer2D::Flush();
}
