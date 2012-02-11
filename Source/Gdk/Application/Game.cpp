/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Game.h"
#include "Application.h"
#include "../System/Logging.h"

using namespace Gdk;

// Static instantiations
Game* Game::singleton = NULL;

// *****************************************************************
/// @brief
///     Default Constructor
// *****************************************************************
Game::Game ()
{
}

// *****************************************************************
/// @brief
///     Deconstructor
// *****************************************************************
Game::~Game ()
{
}

// *****************************************************************
/// @brief
///     Gets basic application settings from the game, before the game and the application are initialized.
/// @remarks
///     When this method is called, many of the 2nd tier GDK systems (graphics, audio, assets, 
///     platform-specific window or view, etc) are not initialized.  Do not load assets here.
/// @param settings
///     An ApplicationSettings struct for the derived class to populate.
// *****************************************************************
bool Game::OnLoadSettings(ApplicationSettings &settings)
{
	// Default implementation, Sets some default values for Games
	settings.Title = "Gdk Game";
	settings.Width = 640;
	settings.Height = 480;

	return true;
}

// *****************************************************************
/// @brief
///     Called once, when the game is first initialized.
/// @remarks 
///     When this method is called, the entire GDK system is initialized and running.  
///     You are free to load assets, allocate resources, and use any GDK libaries
// *****************************************************************
bool Game::OnInit()
{
	return true;
}

// *****************************************************************
/// @brief
///     Called once, when the GDK is shutting down the game.
/// @remarks 
///     When this method is called, the GDK is giving you one last chance 
///     to save or persist any information, before the application is terminated.
// *****************************************************************
void Game::OnShutdown()
{
}

// *****************************************************************
/// @brief
///     Called once per frame, at the beginning of the frame.
/// @remarks 
///     You should perform any game state updates during the execution of this method, such as
///     input processing, game object updates, and physics updates.
/// @param elapsedSeconds
///     The number of seconds that have elapsed since the last frame.  This value should be used
///     to update any time-dependent data, such as animation playback.
// *****************************************************************
void Game::OnUpdate(float elapsedSeconds)
{
}

// *****************************************************************
/// @brief
///     Called once per frame, after the Update() call.
/// @remarks 
///     You should perform any rendering related tasks during this method execution. 
/// @param elapsedSeconds
///     The number of seconds that have elapsed since the last frame.  This value should be used
///     to update any time-dependent data, such as animation playback.
// *****************************************************************
void Game::OnDraw(float elapsedSeconds)
{
}

// *****************************************************************
/// @brief
///     Gets the singleton game instance.
/// @note
///     GDK Internal Use Only
// *****************************************************************
Game* Game::GetSingleton()
{
	return singleton;
}

// *****************************************************************
/// @brief
///     Destroys the singleton game instance.
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Game::DestroySingleton()
{
	GdkDelete(singleton);
    singleton = NULL;
}

