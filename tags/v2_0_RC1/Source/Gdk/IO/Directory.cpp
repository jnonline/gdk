/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Directory.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifdef GDKPLATFORM_WINDOWS
#include <direct.h>
#endif

using namespace Gdk;

// *****************************************************************
/// @brief
///     Creates a new directory.
/// @remarks
///     The method can only create 1 directory at a time, so to create an heirarchy 
///     of multiple directors, call the method multiple times.
/// @param directoryPath
///     Path of the directory to create.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
void Directory::Create(const char* directoryPath)
{
#ifdef GDKPLATFORM_WINDOWS
	_mkdir(directoryPath);
#else
    mkdir(directoryPath, S_IRWXU | S_IRWXG | S_IRWXO);  // full permissions for User, Group, & Other
#endif
}

// *****************************************************************
/// @brief
///     Checks the given path is an existing directory.
/// @param directoryPath
///     Path of the directory to look for.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
bool Directory::Exists(const char* directoryPath)
{
	// Get the directory stats
	struct stat dirStat;
	int result = stat(directoryPath, &dirStat);
	//int err = errno;

	// Is this a directory & does it exist?
	if(result == 0 && (dirStat.st_mode & S_IFDIR))
		return true;

	return false;
}

// *****************************************************************
/// @brief
///     Deletes a directory, and optionally all files & subdirectories in the directory
/// @param directoryPath
///     Path of the directory to delete.
/// @param recursive
///     Specify true to delete all child files and sub-directories in the directory
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
void Directory::Delete(const char* directoryPath, bool recursive)
{
	remove(directoryPath);
}

// *****************************************************************
/// @brief
///     Moves a file or directory and its contents to a new path.
/// @param originalFilePath
///     Path to the source file or directory
/// @param newFilePath
///     Path to the new location for the file or directory.
/// @note
///     Use the '/' forward-slash path delimiter to ensure cross-platform support.
// *****************************************************************
void Directory::Move(const char* originalFilePath, const char* newFilePath)
{
	rename(originalFilePath, newFilePath);
}
