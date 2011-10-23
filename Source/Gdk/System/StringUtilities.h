/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include <string>

using namespace std;

// Note on strings:
//
//  When using sprintf or swprintf (or some variation)
//  DO NOT use %s or %S (they are not cross-platform friendly).  
//      instead use [%ls] (wide) or [%hs] (single-byte)
//            also, [%lc] (wide) or [%hc] (single-byte) for characters

namespace Gdk
{
    /// @addtogroup System
    /// @{
    
	// =================================================================================
    ///	@brief
    ///		Provides utility methods for working with strings
    ///	@remarks 
    ///		This static-only class contains common constants and utility methods
    ///     string manipulation.
    // =================================================================================
    class StringUtilities
	{
	public:

        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Constants
        /// @{
        
		/// An empty c-string ""
		static const char* Empty;
        
        /// An empty w-string L""
		static const wchar_t* WEmpty;
        
        /// The c-string "Unknown"
		static const char* Unknown;
        
        /// The w-string L"Unknown"
		static const wchar_t* WUnknown;
        
        /// @}

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Common String Methods
        /// @{
        
		static string Replace(const char* input, const char* searchText, const char* replacementText);
		static wstring Replace(const wchar_t* input, const wchar_t* searchText, const wchar_t* replacementText);

        /// @}
        // ---------------------------------
        /// @name UTF8 to UTF16 conversions
        /// @{
        
		static wstring UTF8ToUTF16(const char* source);
		static string UTF16ToUTF8(const wchar_t* source);

        /// @}
        // ---------------------------------
        /// @name Hash Encoding Methods
        /// @{

		static unsigned int FastHash(const UInt8* data, int len);
		static unsigned int FastHash(const char* str);
		static unsigned int FastHash(const wchar_t* str);
        
        /// @}
	};

    /// @}
    
} // namespace Gdk
