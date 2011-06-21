
#include "BasePCH.h"
#include "Keyboard.h"

using namespace Gdk;

#define KBFLAG_KEY_DOWN		0x01		// The key is Down
#define KBFLAG_KEY_TOGGLED	0x02		// The key is Toggled
#define KBFLAG_KEY_CHANGED	0x04		// The key up/dow state just changed

// Static Instantiation
UInt8 Keyboard::keyStates[256];
UInt8 Keyboard::scanCodeToKeyTable[256];

// Events
Keyboard::KeyboardKeyEventHandler	Keyboard::KeyDown;
Keyboard::KeyboardKeyEventHandler	Keyboard::KeyUp;
Keyboard::KeyboardKeyEventHandler	Keyboard::KeyToggled;
Keyboard::KeyboardCharEventHandler	Keyboard::CharTyped;

// ********************************************************************
void Keyboard::Init()
{
    // Clear the key states array
    memset((void*)keyStates, 0, sizeof(UInt8[256]));
    
    // Setup the scan code to key lookup table
    InitScanCodeToKeyTable();
}

// ********************************************************************
void Keyboard::Update(float elapsedSeconds)
{

}

// ********************************************************************
void Keyboard::PostUpdate(float elapsedSeconds)
{
	// Loop through the key states
	for(int i=0; i<256; i++)
	{
		// Unflag the CHANGED flag on this key
		keyStates[i] = keyStates[i] & ~KBFLAG_KEY_CHANGED;
	}
}

// ********************************************************************
void Keyboard::Platform_ProcessKeyDown(Keys::Enum key)
{
	// Determine if the key was already down
	bool wasAlreadyDown = (keyStates[(int)key] & KBFLAG_KEY_DOWN) == KBFLAG_KEY_DOWN;

	// Set the "KEY DOWN" & "CHANGED" flags
	keyStates[(int)key] |= KBFLAG_KEY_DOWN;

	// Was the key not already down?
	if(wasAlreadyDown == false)
	{
		keyStates[(int)key] |= KBFLAG_KEY_CHANGED;

		// Toggle the key
		if(keyStates[(int)key] & KBFLAG_KEY_TOGGLED)
		{
			keyStates[(int)key] &= ~KBFLAG_KEY_TOGGLED;
		}
		else
		{
			keyStates[(int)key] |= KBFLAG_KEY_TOGGLED;

			// Call the event
			KeyToggled.Invoke(key);
		}
	}

	// Call the event
	KeyDown.Invoke(key);
}

// ********************************************************************
void Keyboard::Platform_ProcessKeyUp(Keys::Enum key)
{
	// Determine if the key was already up
	bool wasAlreadyUp = (keyStates[(int)key] & KBFLAG_KEY_DOWN) == 0;

	// Was the key NOT already up?
	if(wasAlreadyUp == false)
	{
		// Remove the "KEY DOWN" flag & set the "CHANGED" flag
		keyStates[(int)key] = (keyStates[(int)key] & ~KBFLAG_KEY_DOWN) | KBFLAG_KEY_CHANGED;

		// Call the events
		KeyUp.Invoke(key);
	}
}

// ********************************************************************
void Keyboard::Platform_ProcessChar(wchar_t c)
{
	// Call the event
	CharTyped.Invoke(c);
}

// ********************************************************************
Keys::Enum Keyboard::Platform_ConvertScanCodeToKey(UInt8 scanCode)
{
    // Lookup the key for the given scan code
    return (Keys::Enum) scanCodeToKeyTable[scanCode];
}


// ********************************************************************
bool Keyboard::IsKeyDown(Keys::Enum key)
{
	return (keyStates[(int)key] & KBFLAG_KEY_DOWN) == KBFLAG_KEY_DOWN;
}

// ********************************************************************
bool Keyboard::IsKeyUp(Keys::Enum key)
{
    return (keyStates[(int)key] & KBFLAG_KEY_DOWN) == 0;
}

// ********************************************************************
bool Keyboard::IsKeyToggled(Keys::Enum key)
{
	return (keyStates[(int)key] & KBFLAG_KEY_TOGGLED) == KBFLAG_KEY_TOGGLED;
}

// ********************************************************************
bool Keyboard::IsKeyJustPressed(Keys::Enum key)
{
	return (keyStates[(int)key] & (KBFLAG_KEY_DOWN | KBFLAG_KEY_CHANGED)) == (KBFLAG_KEY_DOWN | KBFLAG_KEY_CHANGED);
}

// ********************************************************************
bool Keyboard::IsKeyJustReleased(Keys::Enum key)
{
	return (keyStates[(int)key] & (KBFLAG_KEY_DOWN | KBFLAG_KEY_CHANGED)) == KBFLAG_KEY_CHANGED;
}






// ############################################################################
//
//    SCAN CODE TO KEY MAPPING TABLES
//
// ############################################################################

void Keyboard::InitScanCodeToKeyTable()
{
    
    // Set all the default scan code mappings to Keys::Unknown
    memset((void*)scanCodeToKeyTable, (UInt8)Keys::Unknown, sizeof(UInt8[256]));
    
    // The follow sections define the ScanCode to Gdk::Keys mapping for each platform.
    // If a line is commented out, then that Gdk::Keys enum value does not have a corresponding scan code on the platform
    
    // ==================================================================================================================
#if defined(GDKPLATFORM_WINDOWS)
    
    // Digits / Numbers
    scanCodeToKeyTable[0x30] = (UInt8) Keys::D0;
    scanCodeToKeyTable[0x31] = (UInt8) Keys::D1;
    scanCodeToKeyTable[0x32] = (UInt8) Keys::D2;
    scanCodeToKeyTable[0x33] = (UInt8) Keys::D3;
    scanCodeToKeyTable[0x34] = (UInt8) Keys::D4;
    scanCodeToKeyTable[0x35] = (UInt8) Keys::D5;
    scanCodeToKeyTable[0x36] = (UInt8) Keys::D6;
    scanCodeToKeyTable[0x37] = (UInt8) Keys::D7;
    scanCodeToKeyTable[0x38] = (UInt8) Keys::D8;
    scanCodeToKeyTable[0x39] = (UInt8) Keys::D9;
    
    // Letters
    scanCodeToKeyTable[0x41] = (UInt8) Keys::A;
    scanCodeToKeyTable[0x42] = (UInt8) Keys::B;
    scanCodeToKeyTable[0x43] = (UInt8) Keys::C;
    scanCodeToKeyTable[0x44] = (UInt8) Keys::D;
    scanCodeToKeyTable[0x45] = (UInt8) Keys::E;
    scanCodeToKeyTable[0x46] = (UInt8) Keys::F;
    scanCodeToKeyTable[0x47] = (UInt8) Keys::G;
    scanCodeToKeyTable[0x48] = (UInt8) Keys::H;
    scanCodeToKeyTable[0x49] = (UInt8) Keys::I;
    scanCodeToKeyTable[0x4A] = (UInt8) Keys::J;
    scanCodeToKeyTable[0x4B] = (UInt8) Keys::K;
    scanCodeToKeyTable[0x4C] = (UInt8) Keys::L;
    scanCodeToKeyTable[0x4D] = (UInt8) Keys::M;
    scanCodeToKeyTable[0x4E] = (UInt8) Keys::N;
    scanCodeToKeyTable[0x4F] = (UInt8) Keys::O;
    scanCodeToKeyTable[0x50] = (UInt8) Keys::P;
    scanCodeToKeyTable[0x51] = (UInt8) Keys::Q;
    scanCodeToKeyTable[0x52] = (UInt8) Keys::R;
    scanCodeToKeyTable[0x53] = (UInt8) Keys::S;
    scanCodeToKeyTable[0x54] = (UInt8) Keys::T;
    scanCodeToKeyTable[0x55] = (UInt8) Keys::U;
    scanCodeToKeyTable[0x56] = (UInt8) Keys::V;
    scanCodeToKeyTable[0x57] = (UInt8) Keys::W;
    scanCodeToKeyTable[0x58] = (UInt8) Keys::X;
    scanCodeToKeyTable[0x59] = (UInt8) Keys::Y;
    scanCodeToKeyTable[0x5A] = (UInt8) Keys::Z;
    
    // Function Keys
    scanCodeToKeyTable[VK_F1] = (UInt8) Keys::F1;
    scanCodeToKeyTable[VK_F2] = (UInt8) Keys::F2;
    scanCodeToKeyTable[VK_F3] = (UInt8) Keys::F3;
    scanCodeToKeyTable[VK_F4] = (UInt8) Keys::F4;
    scanCodeToKeyTable[VK_F5] = (UInt8) Keys::F5;
    scanCodeToKeyTable[VK_F6] = (UInt8) Keys::F6;
    scanCodeToKeyTable[VK_F7] = (UInt8) Keys::F7;
    scanCodeToKeyTable[VK_F8] = (UInt8) Keys::F8;
    scanCodeToKeyTable[VK_F9] = (UInt8) Keys::F9;
    scanCodeToKeyTable[VK_F10] = (UInt8) Keys::F10;
    scanCodeToKeyTable[VK_F11] = (UInt8) Keys::F11;
    scanCodeToKeyTable[VK_F12] = (UInt8) Keys::F12;
    scanCodeToKeyTable[VK_F13] = (UInt8) Keys::F13;
    scanCodeToKeyTable[VK_F14] = (UInt8) Keys::F14;
    scanCodeToKeyTable[VK_F15] = (UInt8) Keys::F15;
    scanCodeToKeyTable[VK_F16] = (UInt8) Keys::F16;
    scanCodeToKeyTable[VK_F17] = (UInt8) Keys::F17;
    scanCodeToKeyTable[VK_F18] = (UInt8) Keys::F18;
    scanCodeToKeyTable[VK_F19] = (UInt8) Keys::F19;
    scanCodeToKeyTable[VK_F20] = (UInt8) Keys::F20;
    scanCodeToKeyTable[VK_F21] = (UInt8) Keys::F21;
    scanCodeToKeyTable[VK_F22] = (UInt8) Keys::F22;
    scanCodeToKeyTable[VK_F23] = (UInt8) Keys::F23;
    scanCodeToKeyTable[VK_F24] = (UInt8) Keys::F24;
    
    // Number pad
    scanCodeToKeyTable[VK_NUMPAD0] = (UInt8) Keys::NumPad0;
    scanCodeToKeyTable[VK_NUMPAD1] = (UInt8) Keys::NumPad1;
    scanCodeToKeyTable[VK_NUMPAD2] = (UInt8) Keys::NumPad2;
    scanCodeToKeyTable[VK_NUMPAD3] = (UInt8) Keys::NumPad3;
    scanCodeToKeyTable[VK_NUMPAD4] = (UInt8) Keys::NumPad4;
    scanCodeToKeyTable[VK_NUMPAD5] = (UInt8) Keys::NumPad5;
    scanCodeToKeyTable[VK_NUMPAD6] = (UInt8) Keys::NumPad6;
    scanCodeToKeyTable[VK_NUMPAD7] = (UInt8) Keys::NumPad7;
    scanCodeToKeyTable[VK_NUMPAD8] = (UInt8) Keys::NumPad8;
    scanCodeToKeyTable[VK_NUMPAD9] = (UInt8) Keys::NumPad9;
    
    scanCodeToKeyTable[VK_ADD]          = (UInt8) Keys::NumPadAdd;
    scanCodeToKeyTable[VK_SUBTRACT]     = (UInt8) Keys::NumPadMinus;
    scanCodeToKeyTable[VK_MULTIPLY]     = (UInt8) Keys::NumPadMultiply;
    scanCodeToKeyTable[VK_DIVIDE]       = (UInt8) Keys::NumPadDivide;
    scanCodeToKeyTable[0x92]            = (UInt8) Keys::NumPadEquals;
    //scanCodeToKeyTable[]              = (UInt8) Keys::NumPadEnter;
    scanCodeToKeyTable[VK_DECIMAL]      = (UInt8) Keys::NumPadDecimal;
    scanCodeToKeyTable[VK_SEPARATOR]    = (UInt8) Keys::NumPadSeparator;
    
    // Modifiers
    scanCodeToKeyTable[VK_LSHIFT]   = (UInt8) Keys::LeftShift;
    scanCodeToKeyTable[VK_RSHIFT]   = (UInt8) Keys::RightShift;
    scanCodeToKeyTable[VK_LCONTROL] = (UInt8) Keys::LeftControl;
    scanCodeToKeyTable[VK_RCONTROL] = (UInt8) Keys::RightControl;
    scanCodeToKeyTable[VK_LMENU]    = (UInt8) Keys::LeftAlt;
    scanCodeToKeyTable[VK_RMENU]    = (UInt8) Keys::RightAlt;
    scanCodeToKeyTable[VK_LWIN]     = (UInt8) Keys::LeftGUI;
    scanCodeToKeyTable[VK_RWIN]     = (UInt8) Keys::RightGUI;
    //scanCodeToKeyTable[]          = (UInt8) Keys::Function;
    
    // KB State / Lock buttons
    scanCodeToKeyTable[VK_CAPITAL]  = (UInt8) Keys::CapsLock;
    scanCodeToKeyTable[VK_NUMLOCK]  = (UInt8) Keys::NumLock;
    scanCodeToKeyTable[VK_SCROLL]   = (UInt8) Keys::ScrollLock;
    
    // Editing
    scanCodeToKeyTable[VK_BACK]     = (UInt8) Keys::Backspace;
    scanCodeToKeyTable[VK_TAB]      = (UInt8) Keys::Tab;
    scanCodeToKeyTable[VK_RETURN]    = (UInt8) Keys::Enter;
    scanCodeToKeyTable[VK_PAUSE]    = (UInt8) Keys::Pause;
    scanCodeToKeyTable[VK_ESCAPE]   = (UInt8) Keys::Escape;
    scanCodeToKeyTable[VK_SPACE]    = (UInt8) Keys::Space;
    scanCodeToKeyTable[VK_PRIOR]   = (UInt8) Keys::PageUp;
    scanCodeToKeyTable[VK_NEXT] = (UInt8) Keys::PageDown;
    scanCodeToKeyTable[VK_END]      = (UInt8) Keys::End;
    scanCodeToKeyTable[VK_HOME]     = (UInt8) Keys::Home;
    scanCodeToKeyTable[VK_INSERT]   = (UInt8) Keys::Insert;
    scanCodeToKeyTable[VK_DELETE]   = (UInt8) Keys::Delete;
    
    // Directional Pad
    scanCodeToKeyTable[VK_LEFT]     = (UInt8) Keys::Left;
    scanCodeToKeyTable[VK_RIGHT]    = (UInt8) Keys::Right;
    scanCodeToKeyTable[VK_UP]       = (UInt8) Keys::Up;
    scanCodeToKeyTable[VK_DOWN]     = (UInt8) Keys::Down;
    
    // Common text keys
    scanCodeToKeyTable[VK_OEM_PLUS]     = (UInt8) Keys::Equals;
    scanCodeToKeyTable[VK_OEM_MINUS]    = (UInt8) Keys::Minus;
    scanCodeToKeyTable[VK_OEM_4]        = (UInt8) Keys::LeftBracket;
    scanCodeToKeyTable[VK_OEM_6]        = (UInt8) Keys::RightBracket;
    scanCodeToKeyTable[VK_OEM_2]        = (UInt8) Keys::ForwardSlash;
    scanCodeToKeyTable[VK_OEM_5]        = (UInt8) Keys::BackSlash;
    scanCodeToKeyTable[VK_OEM_7]        = (UInt8) Keys::Apostrophe;
    scanCodeToKeyTable[VK_OEM_1]        = (UInt8) Keys::Semicolon;
    scanCodeToKeyTable[VK_OEM_COMMA]    = (UInt8) Keys::Comma;
    scanCodeToKeyTable[VK_OEM_PERIOD]   = (UInt8) Keys::Period;
    scanCodeToKeyTable[VK_OEM_3]        = (UInt8) Keys::Tilde;
    
    // Misc Keys
    scanCodeToKeyTable[VK_SNAPSHOT]     = (UInt8) Keys::PrintScreen;
    scanCodeToKeyTable[VK_APPS]         = (UInt8) Keys::Application;
    
    // Browser control keys
    scanCodeToKeyTable[VK_BROWSER_BACK]         = (UInt8) Keys::BrowserBack;
    scanCodeToKeyTable[VK_BROWSER_FAVORITES]    = (UInt8) Keys::BrowserFavorites;
    scanCodeToKeyTable[VK_BROWSER_FORWARD]      = (UInt8) Keys::BrowserForward;
    scanCodeToKeyTable[VK_BROWSER_HOME]         = (UInt8) Keys::BrowserHome;
    scanCodeToKeyTable[VK_BROWSER_REFRESH]      = (UInt8) Keys::BrowserRefresh;
    scanCodeToKeyTable[VK_BROWSER_SEARCH]       = (UInt8) Keys::BrowserSearch;
    scanCodeToKeyTable[VK_BROWSER_STOP]         = (UInt8) Keys::BrowserStop;
    
    // Media control keys
    scanCodeToKeyTable[VK_VOLUME_UP]            = (UInt8) Keys::VolumeUp;
    scanCodeToKeyTable[VK_VOLUME_DOWN]          = (UInt8) Keys::VolumeDown;
    scanCodeToKeyTable[VK_VOLUME_MUTE]          = (UInt8) Keys::Mute;
    scanCodeToKeyTable[VK_MEDIA_NEXT_TRACK]     = (UInt8) Keys::MediaNext;
    scanCodeToKeyTable[VK_MEDIA_PREV_TRACK]     = (UInt8) Keys::MediaPrevious;
    scanCodeToKeyTable[VK_MEDIA_PLAY_PAUSE]     = (UInt8) Keys::MediaPlay;
    scanCodeToKeyTable[VK_MEDIA_STOP]           = (UInt8) Keys::MediaStop;
    
    // ==================================================================================================================
#elif defined(GDKPLATFORM_MACOSX)
    
    // Digits / Numbers
    scanCodeToKeyTable[kVK_ANSI_0] = (UInt8) Keys::D0;
    scanCodeToKeyTable[kVK_ANSI_1] = (UInt8) Keys::D1;
    scanCodeToKeyTable[kVK_ANSI_2] = (UInt8) Keys::D2;
    scanCodeToKeyTable[kVK_ANSI_3] = (UInt8) Keys::D3;
    scanCodeToKeyTable[kVK_ANSI_4] = (UInt8) Keys::D4;
    scanCodeToKeyTable[kVK_ANSI_5] = (UInt8) Keys::D5;
    scanCodeToKeyTable[kVK_ANSI_6] = (UInt8) Keys::D6;
    scanCodeToKeyTable[kVK_ANSI_7] = (UInt8) Keys::D7;
    scanCodeToKeyTable[kVK_ANSI_8] = (UInt8) Keys::D8;
    scanCodeToKeyTable[kVK_ANSI_9] = (UInt8) Keys::D9;
    
    // Letters
    scanCodeToKeyTable[kVK_ANSI_A] = (UInt8) Keys::A;
    scanCodeToKeyTable[kVK_ANSI_B] = (UInt8) Keys::B;
    scanCodeToKeyTable[kVK_ANSI_C] = (UInt8) Keys::C;
    scanCodeToKeyTable[kVK_ANSI_D] = (UInt8) Keys::D;
    scanCodeToKeyTable[kVK_ANSI_E] = (UInt8) Keys::E;
    scanCodeToKeyTable[kVK_ANSI_F] = (UInt8) Keys::F;
    scanCodeToKeyTable[kVK_ANSI_G] = (UInt8) Keys::G;
    scanCodeToKeyTable[kVK_ANSI_H] = (UInt8) Keys::H;
    scanCodeToKeyTable[kVK_ANSI_I] = (UInt8) Keys::I;
    scanCodeToKeyTable[kVK_ANSI_J] = (UInt8) Keys::J;
    scanCodeToKeyTable[kVK_ANSI_K] = (UInt8) Keys::K;
    scanCodeToKeyTable[kVK_ANSI_L] = (UInt8) Keys::L;
    scanCodeToKeyTable[kVK_ANSI_M] = (UInt8) Keys::M;
    scanCodeToKeyTable[kVK_ANSI_N] = (UInt8) Keys::N;
    scanCodeToKeyTable[kVK_ANSI_O] = (UInt8) Keys::O;
    scanCodeToKeyTable[kVK_ANSI_P] = (UInt8) Keys::P;
    scanCodeToKeyTable[kVK_ANSI_Q] = (UInt8) Keys::Q;
    scanCodeToKeyTable[kVK_ANSI_R] = (UInt8) Keys::R;
    scanCodeToKeyTable[kVK_ANSI_S] = (UInt8) Keys::S;
    scanCodeToKeyTable[kVK_ANSI_T] = (UInt8) Keys::T;
    scanCodeToKeyTable[kVK_ANSI_U] = (UInt8) Keys::U;
    scanCodeToKeyTable[kVK_ANSI_V] = (UInt8) Keys::V;
    scanCodeToKeyTable[kVK_ANSI_W] = (UInt8) Keys::W;
    scanCodeToKeyTable[kVK_ANSI_X] = (UInt8) Keys::X;
    scanCodeToKeyTable[kVK_ANSI_Y] = (UInt8) Keys::Y;
    scanCodeToKeyTable[kVK_ANSI_Z] = (UInt8) Keys::Z;
    
    // Function Keys
    scanCodeToKeyTable[kVK_F1] = (UInt8) Keys::F1;
    scanCodeToKeyTable[kVK_F2] = (UInt8) Keys::F2;
    scanCodeToKeyTable[kVK_F3] = (UInt8) Keys::F3;
    scanCodeToKeyTable[kVK_F4] = (UInt8) Keys::F4;
    scanCodeToKeyTable[kVK_F5] = (UInt8) Keys::F5;
    scanCodeToKeyTable[kVK_F6] = (UInt8) Keys::F6;
    scanCodeToKeyTable[kVK_F7] = (UInt8) Keys::F7;
    scanCodeToKeyTable[kVK_F8] = (UInt8) Keys::F8;
    scanCodeToKeyTable[kVK_F9] = (UInt8) Keys::F9;
    scanCodeToKeyTable[kVK_F10] = (UInt8) Keys::F10;
    scanCodeToKeyTable[kVK_F11] = (UInt8) Keys::F11;
    scanCodeToKeyTable[kVK_F12] = (UInt8) Keys::F12;
    scanCodeToKeyTable[kVK_F13] = (UInt8) Keys::F13;
    scanCodeToKeyTable[kVK_F14] = (UInt8) Keys::F14;
    scanCodeToKeyTable[kVK_F15] = (UInt8) Keys::F15;
    scanCodeToKeyTable[kVK_F16] = (UInt8) Keys::F16;
    scanCodeToKeyTable[kVK_F17] = (UInt8) Keys::F17;
    scanCodeToKeyTable[kVK_F18] = (UInt8) Keys::F18;
    scanCodeToKeyTable[kVK_F19] = (UInt8) Keys::F19;
    scanCodeToKeyTable[kVK_F20] = (UInt8) Keys::F20;
    //scanCodeToKeyTable[] = (UInt8) Keys::F21;
    //scanCodeToKeyTable[] = (UInt8) Keys::F22;
    //scanCodeToKeyTable[] = (UInt8) Keys::F23;
    //scanCodeToKeyTable[] = (UInt8) Keys::F24;
    
    // Number pad
    scanCodeToKeyTable[kVK_ANSI_Keypad0] = (UInt8) Keys::NumPad0;
    scanCodeToKeyTable[kVK_ANSI_Keypad1] = (UInt8) Keys::NumPad1;
    scanCodeToKeyTable[kVK_ANSI_Keypad2] = (UInt8) Keys::NumPad2;
    scanCodeToKeyTable[kVK_ANSI_Keypad3] = (UInt8) Keys::NumPad3;
    scanCodeToKeyTable[kVK_ANSI_Keypad4] = (UInt8) Keys::NumPad4;
    scanCodeToKeyTable[kVK_ANSI_Keypad5] = (UInt8) Keys::NumPad5;
    scanCodeToKeyTable[kVK_ANSI_Keypad6] = (UInt8) Keys::NumPad6;
    scanCodeToKeyTable[kVK_ANSI_Keypad7] = (UInt8) Keys::NumPad7;
    scanCodeToKeyTable[kVK_ANSI_Keypad8] = (UInt8) Keys::NumPad8;
    scanCodeToKeyTable[kVK_ANSI_Keypad9] = (UInt8) Keys::NumPad9;
    
    scanCodeToKeyTable[kVK_ANSI_KeypadPlus]      = (UInt8) Keys::NumPadAdd;
    scanCodeToKeyTable[kVK_ANSI_KeypadMinus]     = (UInt8) Keys::NumPadMinus;
    scanCodeToKeyTable[kVK_ANSI_KeypadMultiply]  = (UInt8) Keys::NumPadMultiply;
    scanCodeToKeyTable[kVK_ANSI_KeypadDivide]    = (UInt8) Keys::NumPadDivide;
    scanCodeToKeyTable[kVK_ANSI_KeypadEquals]    = (UInt8) Keys::NumPadEquals;
    scanCodeToKeyTable[kVK_ANSI_KeypadEnter]     = (UInt8) Keys::NumPadEnter;
    scanCodeToKeyTable[kVK_ANSI_KeypadDecimal]   = (UInt8) Keys::NumPadDecimal;
    scanCodeToKeyTable[kVK_JIS_KeypadComma]      = (UInt8) Keys::NumPadSeparator;
    
    // Modifiers
    scanCodeToKeyTable[kVK_Shift]           = (UInt8) Keys::LeftShift;
    scanCodeToKeyTable[kVK_RightShift]      = (UInt8) Keys::RightShift;
    scanCodeToKeyTable[kVK_Control]         = (UInt8) Keys::LeftControl;
    scanCodeToKeyTable[kVK_RightControl]    = (UInt8) Keys::RightControl;
    scanCodeToKeyTable[kVK_Option]          = (UInt8) Keys::LeftAlt;
    scanCodeToKeyTable[kVK_RightOption]     = (UInt8) Keys::RightAlt;
    scanCodeToKeyTable[kVK_Command]         = (UInt8) Keys::LeftGUI;
    scanCodeToKeyTable[0x36]                = (UInt8) Keys::RightGUI;
    scanCodeToKeyTable[kVK_Function]        = (UInt8) Keys::Function;
    
    // KB State / Lock buttons
    scanCodeToKeyTable[kVK_CapsLock]        = (UInt8) Keys::CapsLock;
    scanCodeToKeyTable[kVK_ANSI_KeypadClear]= (UInt8) Keys::NumLock;
    scanCodeToKeyTable[kVK_F13]             = (UInt8) Keys::ScrollLock;         // On some MAC Keyboards, F13 functions as the scroll lock
    
    // Editing      
    scanCodeToKeyTable[kVK_Delete]          = (UInt8) Keys::Backspace;
    scanCodeToKeyTable[kVK_Tab]             = (UInt8) Keys::Tab;
    scanCodeToKeyTable[kVK_Return]          = (UInt8) Keys::Enter;
    scanCodeToKeyTable[kVK_F15]             = (UInt8) Keys::Pause;              // On some MAC Keyboards, F15 functions as the pause
    scanCodeToKeyTable[kVK_Escape]          = (UInt8) Keys::Escape;
    scanCodeToKeyTable[kVK_Space]           = (UInt8) Keys::Space;
    scanCodeToKeyTable[kVK_PageUp]          = (UInt8) Keys::PageUp;
    scanCodeToKeyTable[kVK_PageDown]        = (UInt8) Keys::PageDown;
    scanCodeToKeyTable[kVK_End]             = (UInt8) Keys::End;
    scanCodeToKeyTable[kVK_Home]            = (UInt8) Keys::Home;
    scanCodeToKeyTable[kVK_Help]            = (UInt8) Keys::Insert;
    scanCodeToKeyTable[kVK_ForwardDelete]   = (UInt8) Keys::Delete;
    
    // Directional Pad
    scanCodeToKeyTable[kVK_LeftArrow]       = (UInt8) Keys::Left;
    scanCodeToKeyTable[kVK_RightArrow]      = (UInt8) Keys::Right;
    scanCodeToKeyTable[kVK_UpArrow]         = (UInt8) Keys::Up;
    scanCodeToKeyTable[kVK_DownArrow]       = (UInt8) Keys::Down;
    
    // Common text keys
    scanCodeToKeyTable[kVK_ANSI_Equal]          = (UInt8) Keys::Equals;
    scanCodeToKeyTable[kVK_ANSI_Minus]          = (UInt8) Keys::Minus;
    scanCodeToKeyTable[kVK_ANSI_LeftBracket]    = (UInt8) Keys::LeftBracket;
    scanCodeToKeyTable[kVK_ANSI_RightBracket]   = (UInt8) Keys::RightBracket;
    scanCodeToKeyTable[kVK_ANSI_Slash]          = (UInt8) Keys::ForwardSlash;
    scanCodeToKeyTable[kVK_ANSI_Backslash]      = (UInt8) Keys::BackSlash;
    scanCodeToKeyTable[kVK_ANSI_Quote]          = (UInt8) Keys::Apostrophe;
    scanCodeToKeyTable[kVK_ANSI_Semicolon]      = (UInt8) Keys::Semicolon;
    scanCodeToKeyTable[kVK_ANSI_Comma]          = (UInt8) Keys::Comma;
    scanCodeToKeyTable[kVK_ANSI_Equal]          = (UInt8) Keys::Period;
    scanCodeToKeyTable[kVK_ANSI_Grave]          = (UInt8) Keys::Tilde;
    
    // Misc Keys
    scanCodeToKeyTable[kVK_F14]                 = (UInt8) Keys::ScrollLock;     // On some MAC Keyboards, F14 functions as the print screen 
    scanCodeToKeyTable[0x6E]                    = (UInt8) Keys::Application;
    
    // Browser control keys
    //scanCodeToKeyTable[VK_BROWSER_BACK]         = (UInt8) Keys::BrowserBack;
    //scanCodeToKeyTable[VK_BROWSER_FAVORITES]    = (UInt8) Keys::BrowserFavorites;
    //scanCodeToKeyTable[VK_BROWSER_FORWARD]      = (UInt8) Keys::BrowserForward;
    //scanCodeToKeyTable[VK_BROWSER_HOME]         = (UInt8) Keys::BrowserHome;
    //scanCodeToKeyTable[VK_BROWSER_REFRESH]      = (UInt8) Keys::BrowserRefresh;
    //scanCodeToKeyTable[VK_BROWSER_SEARCH]       = (UInt8) Keys::BrowserSearch;
    //scanCodeToKeyTable[VK_BROWSER_STOP]         = (UInt8) Keys::BrowserStop;
    
    // Media control keys
    scanCodeToKeyTable[kVK_VolumeUp]            = (UInt8) Keys::VolumeUp;
    scanCodeToKeyTable[kVK_VolumeDown]          = (UInt8) Keys::VolumeDown;
    scanCodeToKeyTable[kVK_Mute]                = (UInt8) Keys::Mute;
    //scanCodeToKeyTable[VK_MEDIA_NEXT_TRACK]     = (UInt8) Keys::MediaNext;
    //scanCodeToKeyTable[VK_MEDIA_PREV_TRACK]     = (UInt8) Keys::MediaPrevious;
    //scanCodeToKeyTable[VK_MEDIA_PLAY_PAUSE]     = (UInt8) Keys::MediaPlay;
    //scanCodeToKeyTable[VK_MEDIA_STOP]           = (UInt8) Keys::MediaStop;
    
    // ==================================================================================================================
#elif defined(GDKPLATFORM_IOS)
    
    // iOS has no keyboard keyDown/keyUp support..   yet  TODO(P2)
    
    // ==================================================================================================================
#else
    
#error "Keyboard Scan Code Table Not defined for this platform!"
    
#endif
    
}
