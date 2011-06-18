
#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	class Directory
	{
	private:
		// Static only class
		Directory();

	public:
		// Creates a new directory
		static void Create(const char* directoryPath);

		// Checks if a directory exists at the given path
		static bool Exists(const char* directoryPath);

		// Deletes a directory (and optionally all child files & directories)
		static void Delete(const char* directoryPath, bool recursive);

		// Moves / Renames a file
		static void Move(const char* originalDirectoryPath, const char* newDirectoryPath);

		// TODO(P2): 
		//   Get Child Dirs
		//   Get Child Files
		//   Search with wildcards!
	    
	};

} // namespace Gdk
