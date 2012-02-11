/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include <string>

using namespace std;


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
        
        /// The c-string "Unknown"
		static const char* Unknown;
        
        /// @}

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Common String Methods
        /// @{
        
		static string Replace(const char* input, const char* searchText, const char* replacementText);

        /// @}
        // ---------------------------------
        /// @name Hash Encoding Methods
        /// @{

		static unsigned int FastHash(const UInt8* data, int len);
		static unsigned int FastHash(const char* str);
        
        /// @}
	};

    /// @}
    
} // namespace Gdk
