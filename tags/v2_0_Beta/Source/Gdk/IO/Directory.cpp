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

// ***********************************************************************
Directory::Directory()
{
}

// ***********************************************************************
void Directory::Create(const char* directoryPath)
{
#ifdef GDKPLATFORM_WINDOWS
	_mkdir(directoryPath);
#else
    mkdir(directoryPath, S_IRWXU | S_IRWXG | S_IRWXO);  // full permissions for User, Group, & Other
#endif
}

// ***********************************************************************
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

// ***********************************************************************
void Directory::Delete(const char* directoryPath, bool recursive)
{
	remove(directoryPath);
}

// ***********************************************************************
void Directory::Move(const char* originalFilePath, const char* newFilePath)
{
	rename(originalFilePath, newFilePath);
}
