/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	// NOTE: These all map to the exact ascii Virtual Key codes.
	namespace Keys
	{
		enum Enum
		{
            Unknown = 0,
            
            // Digits / Numbers
			D0,
			D1,
			D2,
			D3,
			D4,
			D5,
			D6,
			D7,
			D8,
			D9,

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

            NumPadAdd,
            NumPadMinus,
			NumPadMultiply,
			NumPadDivide,
            NumPadEquals,
            NumPadEnter,
            NumPadDecimal,
			NumPadSeparator,
            
			// Modifiers
			LeftShift,
			RightShift,
			LeftControl,
			RightControl,
			LeftAlt,
			RightAlt,
			LeftGUI,                // Windows / Apple-Command keys
			RightGUI,
            Function,
	        
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
            Application,        // Windows / Menu key 
	       
			// Browser control keys
			BrowserBack,
			BrowserFavorites,
			BrowserForward,
			BrowserHome,
			BrowserRefresh,
			BrowserSearch,
			BrowserStop,
            
            // Media control keys
            VolumeUp,
            VolumeDown,
            Mute,
            MediaNext,
            MediaPrevious,
			MediaPlay,
			MediaStop,
            
		};
	}
    
    
} // namespace Gdk
