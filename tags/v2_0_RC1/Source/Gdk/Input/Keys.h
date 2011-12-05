/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup Input
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Enumeration of keyboard keys.
    ///	@remarks 
    ///		This enumeration contains all standard keyboard keys and various extension keys available 
    ///     on some OEM keyboards, such as media and browser control keys.
    /// @see
    ///     Keyboard
    // =================================================================================
	namespace Keys
	{
		enum Enum
		{
            Unknown = 0,
            
			D0,             ///< Digit - 0
			D1,             ///< Digit - 1
			D2,             ///< Digit - 2
			D3,             ///< Digit - 3
			D4,             ///< Digit - 4
			D5,             ///< Digit - 5
			D6,             ///< Digit - 6
			D7,             ///< Digit - 7
			D8,             ///< Digit - 8
			D9,             ///< Digit - 9

			// Letters
			A,
			B,
			C,
			D,
			E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,

			// Function Keys
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,

			// Number pad
            NumPad0,
            NumPad1,
            NumPad2,
            NumPad3,
            NumPad4,
            NumPad5,
            NumPad6,
            NumPad7,
            NumPad8,
            NumPad9,

            NumPadAdd,          ///< Number Pad Plus +
            NumPadMinus,        ///< Number Pad Minus -
			NumPadMultiply,     ///< Number Pad Multiply *
			NumPadDivide,       ///< Number Pad Divide /
            NumPadEquals,       ///< Number Pad Equals =
            NumPadEnter,        ///< Number Pad Enter 
            NumPadDecimal,      ///< Number Pad Decimal .
			NumPadSeparator,    ///< Number Pad Seperator
            
			// Modifiers
			LeftShift,
			RightShift,
			LeftControl,
			RightControl,
			LeftAlt,
			RightAlt,
            
			LeftGUI,            ///< Left Windows or Apple-Command key
			RightGUI,           ///< Right Windows or Apple-Command key
            Function,           ///< Function or Fn key.  Currently Mac-only
	        
			// KB State / Lock buttons
			CapsLock,
			NumLock,
			ScrollLock,

			// Editing
			Backspace,
			Tab,
			Enter,
			Pause,
			Escape,
			Space,
			PageUp,
			PageDown,
			End,
			Home,
			Insert,
			Delete,
            
            // Directional Pad
            Left,
			Right,
			Up,
			Down,
            
            // Common text keys
            Equals,
            Minus,
            LeftBracket,
            RightBracket,
            ForwardSlash,
            BackSlash,
            Apostrophe,
            Semicolon,
            Comma,
            Period,
            Tilde,
            
            // Misc Keys
            PrintScreen,
            Application,              ///< Windows / Menu key 
	       
			// Browser control keys
			BrowserBack,              ///< Currently Windows-only
			BrowserFavorites,         ///< Currently Windows-only
			BrowserForward,           ///< Currently Windows-only
			BrowserHome,              ///< Currently Windows-only
			BrowserRefresh,           ///< Currently Windows-only
			BrowserSearch,            ///< Currently Windows-only
			BrowserStop,              ///< Currently Windows-only
            
            // Media control keys
            VolumeUp,
            VolumeDown,
            Mute,
            MediaNext,                ///< Currently Windows-only
            MediaPrevious,            ///< Currently Windows-only
			MediaPlay,                ///< Currently Windows-only
			MediaStop,                ///< Currently Windows-only
            
		};
	}
    
    /// @}
    
} // namespace Gdk
