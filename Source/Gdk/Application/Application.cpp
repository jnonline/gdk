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

ApplicationSettings Application::initialAppSettings;


// Static Event Instantiations
// ======================================

Application::ApplicationEventHandler Application::Resized;
Application::ApplicationEventHandler Application::Exitting;
Application::ApplicationEventHandler Application::Activating;
Application::ApplicationEventHandler Application::Deactivating;
Application::ApplicationEventHandler Application::Suspending;
Application::ApplicationEventHandler Application::Resuming;

// *****************************************************************
/// @brief
///     Default Constructor
// *****************************************************************
Application::Application ()
{
}

// *****************************************************************
/// @brief
///     Internal Update method
// *****************************************************************
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
    Game* game = Game::GetSingleton();
	game->OnUpdate(elapsedSeconds);
	
	// Draw the game
    if(exitRequest == false)
        game->OnDraw(elapsedSeconds);

    // GDK Debug Stats
    // ----------------------
    
    if(exitRequest == false)
    {
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
            Renderer2D::DrawText(SharedAssets::Fonts.Arial20, temp, Vector2(width - 80.0f, 10), DebugStatsColor, textScale);
            
            Renderer2D::Flush();
        }
    }
    
	// GDK Post-Update tasks
	// ----------------------

	Keyboard::PostUpdate(elapsedSeconds);
}

// *****************************************************************
/// @brief
///     Initializes all 1st-tier GDK sub-systems.
/// @remarks
///     1st-tier GDK sub-systems are any system that must be initialized before doing any platform specific setup.  Such as getting
///     the initial application settings and setting up memory tracking & logging.  This must be the first GDK method called on a platform
/// @note
///     GDK Internal Use Only
// *****************************************************************
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

	// Setup the default application settings
	initialAppSettings.Width = 480;
	initialAppSettings.Height = 320;
	initialAppSettings.Title = L"GDK Application";
	initialAppSettings.AllowResize = true;
	initialAppSettings.ShowMinimizeBox = true;
	initialAppSettings.ShowMaximizeBox = true;
    initialAppSettings.ShowCloseBox = true;
	initialAppSettings.FixedTimeStep = FixedTimeStep;
	initialAppSettings.UseFixedTimeStep = IsUsingFixedTimeStep;
	initialAppSettings.AssetManagerBackgroundThreads = 0;

	// Load the application settings from the game
    Game* game = Game::GetSingleton();
	if(game->OnLoadSettings(initialAppSettings) == false)
		return false;

	// Copy the application settings that are dynamic
	width = initialAppSettings.Width;
	height = initialAppSettings.Height;
	title = initialAppSettings.Title;
	IsUsingFixedTimeStep = initialAppSettings.UseFixedTimeStep;
	FixedTimeStep = initialAppSettings.FixedTimeStep;

	// Initialize the Singleton asset manager
	AssetManager::InitSingleton(initialAppSettings.AssetManagerBackgroundThreads);

	// Setup the application states
	exitRequest = false;
	appIsActive = true;
	appIsSuspended = false;

	return true;
}

// *****************************************************************
/// @brief
///     Shuts down the 1st-tier GDK sub-systems.
/// @remarks
///     Shuts down all 1st-tier GDK sub-systems in order.  This must be the last GDK method called on a platform
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Application::Platform_ShutdownGdk()
{
	// Destroy the game singleton
    Game::DestroySingleton();

	// Shutdown GDK Systems
	AssetManager::ShutdownSingleton();
	Log::Shutdown();

	// Shutdown GDK Memory
	Memory::Shutdown();
}

// *****************************************************************
/// @brief
///     Initializes the 2nd-tier GDK sub-systems
/// @remarks
///     2nd-tier GDK sub-systems are any systems that require an platform-specific or external context to be initialized.  Such as
///     an OpenGL context, a platform windowing system window, or any File system priviledges that must first be created.
///     This method also initializes the Game.  
/// @note
///     GDK Internal Use Only
// *****************************************************************
bool Application::Platform_InitGame()
{
	// Init 2nd Tier GDK Systems
	Graphics::Init();
	SharedAssets::Init();
	Graphics::InitAssetDependencies();

	// Init the game
	if(Game::GetSingleton()->OnInit() == false)
		return false;

	return true;
}

// *****************************************************************
/// @brief
///     Shuts down the 2nd-tier GDK sub-systems.
/// @remarks
///     Shuts down all 2nd-tier GDK sub-systems in order.  This method must be called before 
///     destroying any platform-specific or external contexts.
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Application::Platform_ShutdownGame()
{
	// Fire the Exitting event
	Exitting.Invoke();

	// Shutdown the game
	Game::GetSingleton()->OnShutdown();

	// Shutdown 2nd Tier GDK Systems
	SharedAssets::Shutdown();
	Graphics::Shutdown();
}

// *****************************************************************
/// @brief
///     The Main Loop interface for GDK platforms
/// @remarks
///     Internally processes all game-loop based GDK systems.  Including calling the Game::Update and Game::Draw methods
/// @note
///     GDK Internal Use Only
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Tells the GDK about a platform suspending the application
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Application::Platform_OnSuspend()
{
	appIsSuspended = true;

	// Raise the event
	Suspending.Invoke();
}

// *****************************************************************
/// @brief
///     Tells the GDK about a platform resuming the application
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Application::Platform_OnResume()
{
	appIsSuspended = false;

	// Raise the event
	Resuming.Invoke();
}

// *****************************************************************
/// @brief
///     Tells the GDK about a platform activating the application
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Application::Platform_OnActive()
{
	appIsActive = true;

	// Raise the event
	Activating.Invoke();
}

// *****************************************************************
/// @brief
///     Tells the GDK about a platform de-activating the application
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Application::Platform_OnDeactive()
{
	appIsActive = false;

	// Raise the event
	Deactivating.Invoke();
}

// *****************************************************************
/// @brief
///     Tells the GDK about a platform resizing of the application window
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Application::Platform_OnResize(int newWidth, int newHeight)
{
	// Save the new dimensions
	Application::width = newWidth;
	Application::height = newHeight;

	// Fire the re-sized event
	Resized.Invoke();
}

// *****************************************************************
/// @brief
///     Returns true if an application shutdown has been requested, either by the GDK internally or by calling Application::Exit
/// @see 
///     Application::Exit
// *****************************************************************
bool Application::IsExitRequest()
{
	return exitRequest;
}

// *****************************************************************
/// @brief
///     Returns true if an application is currently active
/// @see
///     Application::Activating
///     Application::Deactivating
// *****************************************************************
bool Application::IsAppActive()
{
	return appIsActive;
}

// *****************************************************************
/// @brief
///     Returns true if an application is currently suspended
/// @see
///     Application::Resuming
///     Application::Suspending
// *****************************************************************
bool Application::IsAppSuspended()
{
	return appIsSuspended;
}

// *****************************************************************
/// @brief
///     Tells the GDK to shut the application down (Exit's gracefully)
// *****************************************************************
void Application::Exit()
{
	exitRequest = true;
}

// *****************************************************************
/// @brief
///     Gets the width of the application window / view
// *****************************************************************
int Application::GetWidth()
{
	return width;
}

// *****************************************************************
/// @brief
///     Gets the height of the application window / view
// *****************************************************************
int Application::GetHeight()
{
	return height;
}

// *****************************************************************
/// @brief
///     Gets the text displayed in the application window title
// *****************************************************************
const wchar_t* Application::GetTitle()
{
	return title.c_str();
}

// *****************************************************************
/// @brief
///     Sets the text displayed in the application window title
/// @param title
///     Text title to be displayed in the application window title bar
// *****************************************************************
void Application::SetTitle(const wchar_t* title)
{
	// Call the platform specific "SetTitle" method
	_Gdk_Platform_SetTitle(title);
}

// *****************************************************************
/// @brief
///     Changes the size of the application window.
/// @remarks
///     Sets a new width and height of the application window's client area.  This method is ignored on 
///     mobile platforms, as they are always running at full screen resolution.
/// @param width
///     New width of the application window's client area
/// @param height
///     New height of the application window's client area
// *****************************************************************
void Application::Resize(int width, int height)
{
	// Call the platform specific Resize method
	_Gdk_Platform_Resize(width, height);
}