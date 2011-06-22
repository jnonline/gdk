/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "Application.h"

namespace Gdk
{
	// *******************************************************************
	class Game
	{
		friend class Application;

	protected: 
		// Abstract Class CTor
		Game();
		virtual ~Game();

	public:
		// --------------------------------------
		// Public Events
		// --------------------------------------
		
		virtual bool OnLoadSettings(ApplicationSettings &settings);		// Called before any initialization, to get the basic app settings

		virtual bool OnInit();							// Called just after game is first initialized.
		virtual void OnShutdown();						// Called just before the game is about to shut down.
		virtual void OnUpdate(float elapsedSeconds);
		virtual void OnDraw(float elapsedSeconds);

		// Singleton Access
		static Game* GetSingleton();

	private:
		// --------------------------------------
		// Internal Methods
		// --------------------------------------

		static Game* Singleton;
		static void CreateSingleton();
	};

	// The derived Game class MUST use this macro EXACTLY ONCE in the class .cpp file
	#define GDK_GAME_CLASS(className) \
		void Gdk::Game::CreateSingleton() \
		{ \
			Gdk::Game::Singleton = new className(); \
		}

} // namespace Gdk
