/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "FileStream.h"

namespace Gdk
{
    /// @addtogroup IO
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Static-only interface for creating, opening, moving, and deleting files and
    ///     file based streams.
	// =================================================================================
	class File
	{
	public:
        
        // Public Methods
		// ================================
		
        // ---------------------------------
        /// @name Methods
        /// @{
        
		static FileStream* Create(const char* filePath);
        static FileStream* Open(const char* filePath, FileMode::Enum mode);
		static bool Exists(const char* filePath);
		static int GetFileSize(const char* filePath);
		static void Delete(const char* filePath);
		static void Move(const char* originalFilePath, const char* newFilePath);
		static void Copy(const char* originalFilePath, const char* newFilePath);
	  
        /// @}
	};

    /// @} // IO
    
} // namespace Gdk
