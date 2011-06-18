
#pragma once

#include "GdkLIB.h"
#include "FileStream.h"

namespace Gdk
{
	class File
	{
	private:
		// Static only class
		File();

	public:
		// Creates a new file and returns a writeable FileStream
		static FileStream* Create(const char* filePath);

		// Opens an existing file as a FileStream in the given mode
		static FileStream* Open(const char* filePath, FileMode::Enum mode);

		// Checks if a file exists at the given path
		static bool Exists(const char* filePath);

		// Checks if a file exists at the given path
		static int GetFileSize(const char* filePath);

		// Deletes the given file
		static void Delete(const char* filePath);

		// Moves / Renames a file
		static void Move(const char* originalFilePath, const char* newFilePath);

		// Copies a file
		static void Copy(const char* originalFilePath, const char* newFilePath);
	    
	};

} // namespace Gdk
