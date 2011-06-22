/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "../Graphics/Color.h"

namespace Gdk
{
	// ******************************************************************
	struct ApplicationSettings
	{
		UInt16 Width;
		UInt16 Height;
		wstring Title;
		bool UseFixedTimeStep;
		float FixedTimeStep;
		int AssetManagerBackgroundThreads;
	};

	// *******************************************************************
	class Application
	{
	public:
		// Public Properties
		// ================================

		static bool IsUsingFixedTimeStep;
		static float FixedTimeStep;
		static float MaxTimeStep;
        
        // Debug Stats
        static bool DebugStatsVisible;
        static Color DebugStatsColor;
        static int CurrentFPS;
        
	private:

		// Internal Properties
		// ================================

		static int width;
		static int height;
		static bool exitRequest;
		static bool appIsActive;
		static bool appIsSuspended;
		static wstring title;
		static float secondsSinceLastUpdate;
        
        static double lastUpdateTime;
        
        static float fpsTimer;
        static int fpsCounter;

	private:	
		// Hidden Ctor / Dtor
		Application();

		// Internal Methods
		static void Update(float elapsedSeconds);

	public:
		// =====================================================
		// Platform -> Application : interfaces
		//		These methods are intended to be called from the platform-dependent Main() implementation
		// =====================================================
		
		// Main execution methods
		static bool Platform_InitGdk();
		static void Platform_ShutdownGdk();
		static bool Platform_InitGame();
		static void Platform_ShutdownGame();
		static void Platform_MainLoop();
		
		// Execution events
		static void Platform_OnSuspend();
		static void Platform_OnResume();
		static void Platform_OnActive();
		static void Platform_OnDeactive();
		static void Platform_OnResize(int newWidth, int newHeight);

		// =====================================================
		// Game -> Application : interfaces
		//		These methods are intended to be called by the Game code
		// =====================================================

		// Application 'Get' methods
		// ------------------------------

		static int GetWidth();
		static int GetHeight();
		static bool IsExitRequest();
		static bool IsAppActive();
		static bool IsAppSuspended();
		static const wchar_t* GetTitle();

		// Application 'Set' methods
		// ------------------------------

		static void Exit();
		static void SetTitle(const wchar_t* title);
		static void Resize(int width, int height);

		// Application Delegates
		// ------------------------------

		typedef Gdk::MulticastDelegate0<void>	ApplicationEventHandler;

		// Application Events
		// ------------------------------

		// Called when the application window or display is resized.
		static ApplicationEventHandler Resized;

		// Called just before the application is closed/terminated
		static ApplicationEventHandler Exitting;				

		// Called when the application is made the primary view (iOS: dismiss text msg, WIN: alt-tab back, etc)
		static ApplicationEventHandler Activating;			

		// Called when the application is being made a non-primary view (iOS: txt msg pops up / incoming call, WIN: alt-tab away, etc)
		static ApplicationEventHandler Deactivating;		

		// Called when the application is being suspended (iOS: hit home button or change apps, WIN: does nothing)
		static ApplicationEventHandler Suspending;

		// Called when the application is resuming from the suspended state
		static ApplicationEventHandler Resuming;
	};

} // namespace Gdk