/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Application.h"
#include "Game.h"
#include "../System/Logging.h"
#include "../Graphics/Graphics.h"

using namespace Gdk;

// Static Property Instantiations
// ======================================

// Publics
bool Application::IsUsingFixedTimeStep = false;
float Application::FixedTimeStep = 0.02f;
bool Application::DebugStatsVisible = true;
Color Application::DebugStatsColor = Color(0,0,128,255);
int Application::CurrentFPS = 0;
float Application::MaxTimeStep = 0.2f;

// Internals
int Application::width = 0;
int Application::height = 0;
bool Application::exitRequest = false;
bool Application::appIsActive = false;
bool Application::appIsSuspended = false;
wstring Application::title = L"";
float Application::secondsSinceLastUpdate = 0.0f;

double Application::lastUpdateTime = 0.0f;
float Application::fpsTimer = 0.0f;
int Application::fpsCounter = 0;


// Static Event Instantiations
// ======================================

Application::ApplicationEventHandler Application::Resized;
Application::ApplicationEventHandler Application::Exitting;
Application::ApplicationEventHandler Application::Activating;
Application::ApplicationEventHandler Application::Deactivating;
Application::ApplicationEventHandler Application::Suspending;
Application::ApplicationEventHandler Application::Resuming;

// ***********************************************************************
Application::Application ()
{
}

// ***********************************************************************
void Application::Update(float elapsedSeconds)
{
	// GDK Pre-Update tasks
	// ----------------------

	Keyboard::Update(elapsedSeconds);
	Mouse::Update(elapsedSeconds);
	TouchInput::Update(elapsedSeconds);
	
	Graphics::Update(elapsedSeconds);

	// Update & Draw the game 
	// -----------------------------

	// Update the game
	Game::Singleton->OnUpdate(elapsedSeconds);
	
	// Draw the game
	Game::Singleton->OnDraw(elapsedSeconds);

    // GDK Debug Stats
    // ----------------------
    
    // Update the FPS counter
    fpsTimer += elapsedSeconds;
    fpsCounter++;
    if(fpsTimer > 1.0f)
    {
        fpsTimer -= 1.0f;
        CurrentFPS = fpsCounter;
        fpsCounter = 0;
    }
    
    // Render the debug stats
    if(DebugStatsVisible == true)
    {
		// Setup a 2D projection matrix to draw in screen coordinates
        int w = Application::GetWidth();
        int h = Application::GetHeight();
		Matrix3D proj = Matrix3D::CreateOrthoOffCenter(
			0, (float) w,	// left / right
			(float) h, 0,	// bottom / top
			-1.0f, 1.0f		// far / near
			);
		Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

		// Draw the FPS string
        wchar_t temp[64];
		Vector2 textScale(0.7f, 0.7f);
        swprintf(temp, 64, L"FPS: %d", CurrentFPS);
        Renderer2D::DrawText(SharedAssets::Fonts.Arial20, temp, Vector2(width - 60.0f, 10), DebugStatsColor, textScale);
        
        Renderer2D::Flush();
    }
    
	// GDK Post-Update tasks
	// ----------------------

	Keyboard::PostUpdate(elapsedSeconds);
}


// ##########################################################################################
//
// Platform Methods
//
// ##########################################################################################

// ***********************************************************************
bool Application::Platform_InitGdk()
{
	// Init the GDK Memory 
	Memory::Init();

	// Init the GDK System 
	Log::Init();
	Device::Init();
	Mouse::Init();
	Keyboard::Init();
	TouchInput::Init();
    PerlinNoise::Init();
	
	// Create the game Singleton
	Game::CreateSingleton();

	// Load the application settings
	ApplicationSettings appSettings;
	appSettings.FixedTimeStep = FixedTimeStep;
	appSettings.UseFixedTimeStep = IsUsingFixedTimeStep;
	appSettings.AssetManagerBackgroundThreads = 1;
	if(Game::Singleton->OnLoadSettings(appSettings) == false)
		return false;

	// Copy the application settings 
	IsUsingFixedTimeStep = appSettings.UseFixedTimeStep;
	FixedTimeStep = appSettings.FixedTimeStep;
	width = appSettings.Width;
	height = appSettings.Height;
	title = appSettings.Title;

	// Initialize the Singleton asset manager
	AssetManager::InitSingleton(appSettings.AssetManagerBackgroundThreads);

	// Setup the application states
	exitRequest = false;
	appIsActive = true;
	appIsSuspended = false;

	return true;
}

// ***********************************************************************
void Application::Platform_ShutdownGdk()
{
	// Destroy the game singleton
	delete Game::Singleton;
	Game::Singleton = NULL;

	// Shutdown GDK Systems
	AssetManager::ShutdownSingleton();
	Log::Shutdown();

	// Shutdown GDK Memory
	Memory::Shutdown();
}

// ***********************************************************************
bool Application::Platform_InitGame()
{
	// Init 2nd Tier GDK Systems
	Graphics::Init();
	SharedAssets::Init();
	Graphics::InitAssetDependencies();

	// Init the game
	if(Game::Singleton->OnInit() == false)
		return false;

	return true;
}

// ***********************************************************************
void Application::Platform_ShutdownGame()
{
	// Fire the Exitting event
	Exitting.Invoke();

	// Shutdown the game
	Game::Singleton->OnShutdown();

	// Shutdown 2nd Tier GDK Systems
	SharedAssets::Shutdown();
	Graphics::Shutdown();
}

// ***********************************************************************
void Application::Platform_MainLoop()
{
    // Calculate the number of seconds that have elapsed since the last update
    double currentTime = HighResTimer::GetSeconds();
    float elapsedSeconds = 0.0f;
    if(lastUpdateTime != 0.0)
    {
        elapsedSeconds = (float)(currentTime - lastUpdateTime);
        
        // Cap the update time to the max step of the system
        if(elapsedSeconds > MaxTimeStep)
            elapsedSeconds = MaxTimeStep;
    }
    lastUpdateTime = currentTime;

	// Are we using a fixed time step?
	if(IsUsingFixedTimeStep)
	{
		// Add the amount of time passed to our counter
		secondsSinceLastUpdate += elapsedSeconds;
		
		// Perform the necessary number of update cycles
		while(secondsSinceLastUpdate > FixedTimeStep)
		{
			secondsSinceLastUpdate -= FixedTimeStep;
			Update(FixedTimeStep);
		}
	}
	else
	{
		// Update with the variable time step
		Update(elapsedSeconds);
	}
}

// ##########################################################################################
//
// Platform Events
//
// ##########################################################################################


// ***********************************************************************
void Application::Platform_OnSuspend()
{
	appIsSuspended = true;

	// Raise the event
	Suspending.Invoke();
}

// ***********************************************************************
void Application::Platform_OnResume()
{
	appIsSuspended = false;

	// Raise the event
	Resuming.Invoke();
}

// ***********************************************************************
void Application::Platform_OnActive()
{
	appIsActive = true;

	// Raise the event
	Activating.Invoke();
}

// ***********************************************************************
void Application::Platform_OnDeactive()
{
	appIsActive = false;

	// Raise the event
	Deactivating.Invoke();
}

// ***********************************************************************
void Application::Platform_OnResize(int newWidth, int newHeight)
{
	// Save the new dimensions
	Application::width = newWidth;
	Application::height = newHeight;

	// Fire the re-sized event
	Resized.Invoke();
}

// ##########################################################################################
//
// Get methods
//
// ##########################################################################################

// ***********************************************************************
int Application::GetWidth()
{
	return width;
}

// ***********************************************************************
int Application::GetHeight()
{
	return height;
}

// ***********************************************************************
const wchar_t* Application::GetTitle()
{
	return title.c_str();
}

// ***********************************************************************
bool Application::IsExitRequest()
{
	return exitRequest;
}

// ***********************************************************************
bool Application::IsAppActive()
{
	return appIsActive;
}

// ##########################################################################################
//
// Set methods
//
// ##########################################################################################


// ***********************************************************************
void Application::Exit()
{
	exitRequest = true;
}

// ***********************************************************************
void Application::SetTitle(const wchar_t* title)
{
	// Call the platform specific "SetTitle" method
	_Gdk_Platform_SetTitle(title);
}

// ***********************************************************************
void Application::Resize(int width, int height)
{
	// Call the platform specific Resize method
	_Gdk_Platform_Resize(width, height);
}