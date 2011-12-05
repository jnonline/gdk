/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

//
// Asserts:
// This file contains implemenation for the GDK assertion system.  Useable in GDK components & games
//
// Configuration:
//		DEBUG			
//			Assertions are enabled if this pre-compiler symbol is defined
//			If not defined, assertions are replaced as NoOp 
//		
// Usage:
//		ASSERT(ptr != NULL, "Error message", ...);
//

#pragma once



// Use asserts
#if defined(DEBUG) || defined(_DEBUG)
	#define GDK_ASSERTS
#endif

namespace Gdk
{
    /// @addtogroup System
    /// @{
    
	void Assert(bool condition, const char* file, int line, const wchar_t* format, ...);
    void DebugBreak();
    
    /// @}
}


/// @addtogroup System
/// @{

// =================================================================================
///	@brief
///		Asserts a condition is true.
/// @remarks
///     This macro only performs the assertion on DEBUG builds.  On RELEASE builds, the macro does nothing.
/// @param condition
///     A condition that evaluates to true upon success.
/// @param format
///     A wprintf style format string that contains the error message if the assertion fails
/// @param ...
///     wprintf style insertion values for the format string
///	@see Gdk::Assert
// =================================================================================
#ifdef GDK_ASSERTS
	#ifdef GDKPLATFORM_WINDOWS
		#define ASSERT(condition, format, ...) Gdk::Assert(condition, __FILE__, __LINE__, format, __VA_ARGS__)
	#else
		#define ASSERT(condition, format, args...) Gdk::Assert(condition, __FILE__, __LINE__, format, ##args)
	#endif
#else
	#define ASSERT(condition, format, ...)
#endif

/// @}
