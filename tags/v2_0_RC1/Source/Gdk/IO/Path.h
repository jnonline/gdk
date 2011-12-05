/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup IO
    /// @{
    
	// =================================================================================
    /// @brief
    ///     Enumeration of common paths used by GDK based applications   
    /// @see
    ///     Path::GetCommonPath
	// =================================================================================
	namespace CommonPaths
	{
		enum Enum
		{	
            /// Path to a directory where the executing application binaries are located
			ApplicationFolder,	// Where the application binaries are located
            
            /// Path to a directory where the GDK assets and resources are located.
			AssetFolder,
            
            /// Path to a directory where the application has rights to read/write data 
            /// files such as saves, logs, temp files, etc.
			WorkingFolder,
		};
	}

    // =================================================================================
    /// @brief
    ///     Static-only interface for creating & manipulating cross-platform friendly 
    ///     file system paths.  
	// =================================================================================
	class Path
	{
	public:
        // Public Properties
		// ================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{
        
        /// The character to use for seperating path parts.
        static const char PathSeperator;
        
        /// The maximum length of paths supported by the platform.
		static const int MaxPathLength;
        
        /// @}
        
        // Public Methods
		// ================================
		
        // ---------------------------------
        /// @name Methods
        /// @{
        
		static string Combine(const char* path1, const char* path2);
		static string GetDirectory(const char* input);
        static string GetFileName(const char* input);
        static string GetFileNameWithoutExtension(const char* input);
		static string GetExtension(const char* input);
		static string ChangeExtension(const char* input, const char* extension);
		static string GetPathRoot(const char* input);
		static string GetCommonPath(CommonPaths::Enum path);
        
        /// @}
	};

    /// @} // IO
    
} // namespace Gdk
