/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "../Graphics/Color.h"

namespace Gdk
{

    /// @addtogroup Application
    /// @{

    // =================================================================================
    ///	@brief
    ///		Settings for a GDK Application.
    ///	@remarks
    ///		This struct contains several basic settings for a GDK application that define 
    ///		the startup and running states of the application.
    ///	@see 
    ///		Game::OnLoadSettings
    // =================================================================================
	struct ApplicationSettings
	{
        // Public Properties
		// ================================
        
        // -----------------------------------
		/// @name Window Settings
        /// @{
        
        /// Width of the application window.  Ignored on mobile platforms.
		UInt16 Width;

        /// Height of the application window.  Ignored on mobile platforms.
		UInt16 Height;

        /// Title of the application.  Displayed in the application window title bar.
		string Title;

        /// True if the application window can be resized.  Ignored on mobile platforms
		bool AllowResize;

        /// True if the application window will have a minimize box.  Ignored on mobile platforms.
		bool ShowMinimizeBox;

        /// True if the application window will have a maximize box.  Ignored on mobile platforms.
		bool ShowMaximizeBox;
        
        /// True if the application window will have a close box.  Ignored on mobile platforms.
        bool ShowCloseBox;
        
        /// @}
        
        // -----------------------------------
        /// @name Update Loop Settings
        /// @{

        /// Should the Update & Draw loops happen at fixed intervals?
		bool UseFixedTimeStep;

        /// The interval (in seconds) used for updating when  UseFixedTimeStep is set to true.
		float FixedTimeStep;
        
        /// @}
        
        // -----------------------------------
        /// @name GDK System Settings
        /// @{
        
		int ResourceLoaderBackgroundThreads;      ///< Number of threads used by the background resource loading system.  (0 = disable background resource loading)
        
        /// @}
	};

    // =================================================================================
    /// @brief
    ///     Manages all program flow, execution, and internal systems.
    /// @remarks
    ///     This class is the central interface for controlling the overall GDK-based application.
    ///     It is responsible for controling the program flow, from startup to shutdown, and the update
    ///     and event dispatching loops in between.  Application is a static-only class, meaning there
    ///     are no instances of it, and all methods & properties are accessed as statics.
	// =================================================================================
	class Application
	{
	public:
		// Public Properties
		// =====================================================

        // ---------------------------------
        /// @name Update Loop Settings
        /// @{
        
		/// Should the Update & Draw loops happen at fixed intervals?
		static bool IsUsingFixedTimeStep;

		/// The interval (in seconds) used for updating when IsUsingFixedTimeStep is set to true.
		static float FixedTimeStep;

		/// The maximum time step that will be used for an update interval.  (This is the cap for when the application is running slow or has paused for a long period of time)
		static float MaxTimeStep;         
        
        /// @}
        
        // ---------------------------------
        /// @name Debug Statistics Settings
        /// @{
            
		/// Specifies if the built-in debug statistics system is visible
        static bool DebugStatsVisible;          

		/// Color of the debug statistics text
        static Color DebugStatsColor;

		/// The current frames-per-second of the application
        static int CurrentFPS;                  
        
        /// @}
	
		// Public Methods
		// =====================================================

        // ---------------------------------
        /// @name Application Control Methods
        /// @{
        
        static bool IsExitRequest();
		static bool IsAppActive();
		static bool IsAppSuspended();
        static void Exit();
        
        /// @}
        
        // ---------------------------------
        /// @name Window Control Methods
        /// @{
        
		static int GetWidth();
		static int GetHeight();
		static const char* GetTitle();
		static void SetTitle(const char* title);
		static void Resize(int width, int height);

        /// @}
        
        // Public Types
		// =====================================================
		
        /// @name Delegate Types
        /// @{
        
        /// Multicast delegate type for all application events
		typedef Gdk::MulticastDelegate0<void>	ApplicationEventHandler;
        
        /// @}

        // Public Events
		// =====================================================
		
        /// @name Events
        /// @{
        
		/// @brief
		///     This event is raised when the application window is resized.
		static ApplicationEventHandler Resized;

		/// @brief
		///     This event is raised just before the application is closed/terminated.
		static ApplicationEventHandler Exitting;				

		/// @brief
		///     This event is raised when the application is made the primary view.  
        /// @remarks
        ///     For mobile platforms, this occurs when any the application gains the main foreground focus again (such as dismissing an alert)
        ///     For desktop platforms, this occurs when the game window is made the primary window.
		static ApplicationEventHandler Activating;			

		/// @brief
		///     This event is raised when the application is being made a non-primary view
        /// @remarks
        ///     For mobile platforms, this occurs when any the application is temporarily interupted (such as a text message alert)
        ///     For desktop platforms, this occurs when the game window loses focus or is alt-tabbed out of.
		static ApplicationEventHandler Deactivating;		

		/// @brief
		///     This event is raised when the application is being suspended.
        /// @remarks
        ///     This only applies on mobile platforms, when the application is being backgrounded or paused. (such as by hitting the home button on iOS)
		static ApplicationEventHandler Suspending;

		/// @brief
		///     This event is raised when the application is resuming from the suspended state  (Mobile platforms only).
		static ApplicationEventHandler Resuming;
        
        /// @}
        
    public:
        
		// INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
        
		static bool Platform_InitGdk();
		static void Platform_ShutdownGdk();
		static bool Platform_InitGame();
		static void Platform_ShutdownGame();
		static void Platform_MainLoop();
		
		static void Platform_OnSuspend();
		static void Platform_OnResume();
		static void Platform_OnActive();
		static void Platform_OnDeactive();
		static void Platform_OnResize(int newWidth, int newHeight);
        
		static ApplicationSettings* Platform_GetInitialAppSettings() { return &initialAppSettings;}
        
        /// @}

        /// @endcond

    private:
        
		// Internal Properties
		// ================================
        
		// Initial application settings
		static ApplicationSettings initialAppSettings;
        
		// Settings
		static int width;
		static int height;
		static bool exitRequest;
		static bool appIsActive;
		static bool appIsSuspended;
		static string title;
        
		// Update loop timers
		static float secondsSinceLastUpdate;
        static double lastUpdateTime;
        
		// Debug stats
        static float fpsTimer;
        static int fpsCounter;
        
        // Internal Methods
		// ================================
        
		// Hidden Ctor / Dtor
		Application();
        
		static void Update(float elapsedSeconds);
	};
    
    /// @}

} // namespace Gdk