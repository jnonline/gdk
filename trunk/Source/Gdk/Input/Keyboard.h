/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Keys.h"
#include "../System/Delegates.h"

namespace Gdk
{
    /// @addtogroup Input
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Provides access to Keyboard based input.
    ///	@remarks 
    ///		This static-only class contains properties, methods, and events for accessing 
    ///     keyboard input.  It also controls virtual keyboard devices such as the iOS or 
    ///     android pop-up keyboard.
    // =================================================================================
	class Keyboard
	{
    public:
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        static bool IsKeyDown(Keys::Enum key);
		static bool IsKeyUp(Keys::Enum key);
		static bool IsKeyToggled(Keys::Enum key);
		static bool IsKeyJustPressed(Keys::Enum key);
		static bool IsKeyJustReleased(Keys::Enum key);
        
        static bool IsAnyKeyDown();
        static bool IsAnyKeyJustPressed();
        static bool IsAnyKeyJustReleased();
        
        /// @}

        // Public Types
		// =====================================================
        
        // ---------------------------------
        /// @name Delegate Types
        /// @{
        
        /// Multicast delegate type for Keys enumeration events
		typedef Gdk::MulticastDelegate1<void, Keys::Enum>	KeyboardKeyEventHandler;
        
        /// Multicast delegate type for keyboard character events
		typedef Gdk::MulticastDelegate1<void, char>         KeyboardCharEventHandler;

        /// @}
        
        // Public Events
		// =====================================================
		
        /// @name Events
        /// @{
        
		/// @brief
		///     This event is raised when a keyboard key is pressed
		static KeyboardKeyEventHandler		KeyDown;
        
        /// @brief
		///     This event is raised when a keyboard key is released
		static KeyboardKeyEventHandler		KeyUp;
        
        /// @brief
		///     This event is raised when a keyboard key is toggled.
        /// @remarks
        ///     Each time a key is pressed, its state is changed from toggled to not-toggled
		static KeyboardKeyEventHandler		KeyToggled;
        
        /// @brief
		///     This event is raised when a keyboard character is typed
        /// @remarks
        ///     The character is automatically updated depending on the modifer key states (shift, ctrl, alt, etc).
        ///     Handle this event if you are trying to simulate the user typing formatted text into the keyboard
		static KeyboardCharEventHandler		CharTyped;    
        
        /// @}

    public:
        
		// INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
        
		static void Platform_ProcessKeyDown(Keys::Enum key);
		static void Platform_ProcessKeyUp(Keys::Enum key);
		static void Platform_ProcessChar(char c);
        
        static Keys::Enum Platform_ConvertScanCodeToKey(UInt8 scanCode);

        /// @}
        
        /// @endcond
        
	private:
        
        // Internal Properties
		// ================================
        
        // Current state
		static UInt8 keyStates[256];
        
        // Scan code mapping table  (platform-specific)
        static UInt8 scanCodeToKeyTable[256]; 
        
        // Internal Methods
		// ================================

		static void Init();
		static void Update(float elapsedSeconds);
		static void PostUpdate(float elapsedSeconds);
        static void InitScanCodeToKeyTable();
        
        friend class Application;

	};
    
     /// @} // Input

} // namespace Gdk
