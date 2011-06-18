
#include "BasePCH.h"
#include "Game.h"
#include "Application.h"
#include "../System/Logging.h"

using namespace Gdk;

// Static instantiations
Game* Game::Singleton = NULL;

// ***********************************************************************
Game::Game ()
{
}

// ***********************************************************************
Game::~Game ()
{
}

// ##########################################################################################
//
// Events
//
// ##########################################################################################


// ***********************************************************************
bool Game::OnLoadSettings(ApplicationSettings &settings)
{
	// Default implementation, Sets some default values for Games
	settings.Title = L"Gdk Game";
	settings.Width = 640;
	settings.Height = 480;

	return true;
}

// ***********************************************************************
bool Game::OnInit()
{
	return true;
}

// ***********************************************************************
void Game::OnShutdown()
{
}

// ***********************************************************************
void Game::OnUpdate(float elapsedSeconds)
{
}

// ***********************************************************************
void Game::OnDraw(float elapsedSeconds)
{
}


// ##########################################################################################
//
// Public Methods
//
// ##########################################################################################

// ***********************************************************************
Game* Game::GetSingleton()
{
	return Singleton;
}

