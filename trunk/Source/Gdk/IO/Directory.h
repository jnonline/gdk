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
    ///     Static-only interface for creating, deleting, and managing file system directories
	// =================================================================================
	class Directory
	{
	public:
        
        // Public Methods
		// ================================
		
        // ---------------------------------
        /// @name Methods
        /// @{
        
		static void Create(const char* directoryPath);
        static bool Exists(const char* directoryPath);
		static void Delete(const char* directoryPath, bool recursive);
		static void Move(const char* originalDirectoryPath, const char* newDirectoryPath);

        /// @}
        
		// TODO(P2): 
		//   Get Child Dirs
		//   Get Child Files
		//   Search with wildcards!
	    
	};
    
    /// @} // IO

} // namespace Gdk
