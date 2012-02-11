/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Assert.h"
#include "Logging.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Asserts that a condition is true
/// @remarks
///     If the condition evaluates to false, the application will halt with a 
///     dialog or alert that contains the details of the failed assertion.
// *****************************************************************
void Gdk::Assert(bool condition, const char* file, int line, const char* format, ...)
{   
	// First, test the condition
	if(condition == true)
		return;

	// Build the formatted string
	char assertMessage[16048];
	va_list args;
	va_start (args, format);
	GDK_VSPRINTF(assertMessage, 16048, format, args);
	va_end (args);

	// Log the assertion
	Gdk::Log::Write(LogLevel::Assert, file, line, assertMessage);

#ifdef GDKPLATFORM_WINDOWS

	
	char* assertDialogTitle = "GDK Assert";
	char messageBoxText[17000];
	GDK_SPRINTF(messageBoxText, 17000, "Gdk Assertion:\r\n\r\n%s\r\n\r\nWould you like to debug? (Cancel to exit)", assertMessage);

	// Display Message box
	int result = MessageBoxA(0, messageBoxText, assertDialogTitle, MB_YESNOCANCEL | MB_ICONEXCLAMATION);

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
    // TODO(P2): iOS, show an alert..
#endif
}

// *****************************************************************
/// @brief
///     Causes the application to halt and attach to a debugger.
// *****************************************************************
void Gdk::DebugBreak()
{
    
#ifdef GDKPLATFORM_WINDOWS
    ::__debugbreak();
#else
    __builtin_trap();
#endif
    
}
