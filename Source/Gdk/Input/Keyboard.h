#pragma once

#include "GdkLIB.h"
#include "Keys.h"
#include "../System/Delegates.h"

namespace Gdk
{
	class Keyboard
	{
	private:
		Keyboard();
	    
		// Application Interface Methods
		// -----------------------------------

		friend class Application;

		// Init / Update
		static void Init();
		static void Update(float elapsedSeconds);
		static void PostUpdate(float elapsedSeconds);
        
        // Internal Methods
        // -------------------------
        
        static void InitScanCodeToKeyTable();

		// Internal Properties
		// -------------------------

		// Current state
		static UInt8 keyStates[256];
        
        // Scan code mapping table
        static UInt8 scanCodeToKeyTable[256];

	public:

		// Public Platform Interfaces
		// ----------------------------------

		// Input processing
		static void Platform_ProcessKeyDown(Keys::Enum key);
		static void Platform_ProcessKeyUp(Keys::Enum key);
		static void Platform_ProcessChar(wchar_t c);
        
        static Keys::Enum Platform_ConvertScanCodeToKey(UInt8 scanCode);
		
		// Public Interfaces for Games
		// -----------------------------------
		
		static bool IsKeyDown(Keys::Enum key);
		static bool IsKeyUp(Keys::Enum key);
		static bool IsKeyToggled(Keys::Enum key);
		static bool IsKeyJustPressed(Keys::Enum key);
		static bool IsKeyJustReleased(Keys::Enum key);
			
		// Delegates
		typedef Gdk::MulticastDelegate1<void, Keys::Enum>	KeyboardKeyEventHandler;
		typedef Gdk::MulticastDelegate1<void, wchar_t>		KeyboardCharEventHandler;

		// Events
		static KeyboardKeyEventHandler		KeyDown;
		static KeyboardKeyEventHandler		KeyUp;
		static KeyboardKeyEventHandler		KeyToggled;
		static KeyboardCharEventHandler		CharTyped;
        
        
	};

} // namespace Gdk
