/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Assert.h"
#include "Logging.h"

using namespace Gdk;

// ********************************************************
void Gdk::Assert(bool condition, const char* file, int line, const wchar_t* format, ...)
{   
	// First, test the condition
	if(condition == true)
		return;

	// Build the formatted string
	wchar_t assertMessage[16048];
	va_list args;
	va_start (args, format);
	vswprintf(assertMessage, 16048, format, args);
	va_end (args);

	// Log the assertion
	Gdk::Log::Write(LogLevel::Assert, file, line, assertMessage);

#ifdef GDKPLATFORM_WINDOWS

	
	wchar_t* assertDialogTitle = L"GDK Assert";
	wchar_t messageBoxText[17000];
	swprintf(messageBoxText, 17000, L"Gdk Assertion:\r\n\r\n%s\r\n\r\nWould you like to debug? (Cancel to exit)", assertMessage);

	// Display Message box
	int result = MessageBoxW(0, messageBoxText, assertDialogTitle, MB_YESNOCANCEL | MB_ICONEXCLAMATION);

	// Did the user hit Yes?
	if(result == IDYES)
	{
		// Debug breakpoint
        Gdk::DebugBreak();
	}
	else if(result == IDCANCEL)
	{
		exit(0);
	}

#else
    
    Gdk::DebugBreak();

    // TODO(P2): MAC, should show message box with alert msg
#endif
}

// *****************************************
void Gdk::DebugBreak()
{
    
#ifdef GDKPLATFORM_WINDOWS
    ::__debugbreak();
#else
    __builtin_trap();
#endif
    
}
