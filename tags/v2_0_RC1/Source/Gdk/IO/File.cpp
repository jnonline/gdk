/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "File.h"
#include "FileStream.h"

#include <sys/types.h>
#include <sys/stat.h>

using namespace Gdk;

// *****************************************************************
/// @brief
///     Creates or overwrites a file at the specified path.
/// @param filePath
///     Path and name of the file to create.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
FileStream* File::Create(const char* filePath)
{
	return GdkNew FileStream(filePath, FileMode::Write);
}

// *****************************************************************
/// @brief
///     Opens a file at the specified path
/// @param filePath
///     Path and name of the file to open.
/// @param mode
///     Read/Write mode to use when opening the file.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
FileStream* File::Open(const char* filePath, FileMode::Enum mode)
{
	return GdkNew FileStream(filePath, mode);
}

// *****************************************************************
/// @brief
///     Checks if a file exists at the given path
/// @param filePath
///     Path and name of the file to check for.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
bool File::Exists(const char* filePath)
{
	// Get the file stats
	struct stat fileStat;
	int result = stat(filePath, &fileStat);

	// Is this a file & does it exist?
	if(result == 0 && (fileStat.st_mode & S_IFREG))
		return true;

	return false;
}

// *****************************************************************
/// @brief
///     Gets the size of the file (in Bytes) at the given path.
/// @param filePath
///     Path and name of the file to get the size of.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
int File::GetFileSize(const char* filePath)
{
	// Get the file stats
	struct stat fileStat;
	//int result = 
        stat(filePath, &fileStat);

	return (int) fileStat.st_size;
}

// *****************************************************************
/// @brief
///     Deletes a file at the given path
/// @param filePath
///     Path and name of the file to delete.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
void File::Delete(const char* filePath)
{
	remove(filePath);
}

// *****************************************************************
/// @brief
///     Moves a file to a new location
/// @param originalFilePath
///     Path and name of the source file
/// @param newFilePath
///     Path to the new location for the file.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
void File::Move(const char* originalFilePath, const char* newFilePath)
{
	rename(originalFilePath, newFilePath);
}

// *****************************************************************
/// @brief
///     Copies a file to a new location
/// @param originalFilePath
///     Path and name of the source file to copy.
/// @param newFilePath
//      Path and name of the new file.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
void File::Copy(const char* originalFilePath, const char* newFilePath)
{
    // Open the source file stream
    FileStream* source = Open(originalFilePath, FileMode::Read);

    // Open the output file stream
    FileStream* output = Open(newFilePath, FileMode::Write);
    
    // Loop through the file, a chunk at a time
    char tempBuffer[16000];
    int bytesLeft = source->GetLength();
    while(bytesLeft > 0)
    {
        int bytesToCopy = bytesLeft;
        if(bytesToCopy > 16000)
            bytesToCopy = 16000;
        
        // Read the next chunk from the source file
        source->Read(tempBuffer, bytesToCopy);
        
        // Write the chunk to the output file
        output->Write(tempBuffer, bytesToCopy);
        
        // Decrement the bytes counter
        bytesLeft -= bytesToCopy;
    }
    
    // Close the streams
    source->Close();
    output->Close();
}
