/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Application.h"

namespace Gdk
{
	/// @addtogroup Application
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Base class for GDK Games
    ///	@remarks
    ///		The primary interface between a Game and the GDK system is a derived instance of this class.
    ///     There will only be 1 game instance (a singleton) which is controlled by the GDK application.
    // =================================================================================
	class Game
	{
	public:
        
		// Public Methods
		// =====================================================
        
        virtual ~Game();
        
        // ---------------------------------
        /// @name Virtual Methods for the derived Game
        /// @{
		
		virtual bool OnLoadSettings(ApplicationSettings &settings);
		virtual bool OnInit();
		virtual void OnShutdown();
		virtual void OnUpdate(float elapsedSeconds);
		virtual void OnDraw(float elapsedSeconds);

        /// @}
        
		static Game* GetSingleton();

	private:
        
		// Private Properties
		// =====================================================
        
		static Game* singleton;
        
        // Private Methods
		// =====================================================
        
        static void DestroySingleton();
		static void CreateSingleton();
        
	protected: 
        
        // Protected Methods
		// =====================================================
        
		Game();
        
        // Friends
        // =====================================================
        
        friend class Application;

	};

    // =================================================================================
    ///	@brief
    ///		Declares the class that is the Singleton Game of the GDK application
    ///	@remarks
    ///		This macro must exist exactly ONCE.  It defines the class that is the singleton
    ///     game class, and is usually declared in your "MyCoolGame.cpp" file
    // =================================================================================
	#define GDK_GAME_CLASS(className) \
		void Gdk::Game::CreateSingleton() \
		{ \
			Gdk::Game::singleton = GdkNew className(); \
		}
    
    /// @}

} // namespace Gdk
