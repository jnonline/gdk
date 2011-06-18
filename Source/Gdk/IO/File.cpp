
#include "BasePCH.h"
#include "File.h"
#include "FileStream.h"

#include <sys/types.h>
#include <sys/stat.h>

using namespace Gdk;

// ***********************************************************************
File::File()
{
}

// ***********************************************************************
FileStream* File::Create(const char* filePath)
{
	return new FileStream(filePath, FileMode::Write);
}

// ***********************************************************************
FileStream* File::Open(const char* filePath, FileMode::Enum mode)
{
	return new FileStream(filePath, mode);
}

// ***********************************************************************
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

// ***********************************************************************
int File::GetFileSize(const char* filePath)
{
	// Get the file stats
	struct stat fileStat;
	//int result = 
        stat(filePath, &fileStat);

	return (int) fileStat.st_size;
}

// ***********************************************************************
void File::Delete(const char* filePath)
{
	remove(filePath);
}

// ***********************************************************************
void File::Move(const char* originalFilePath, const char* newFilePath)
{
	rename(originalFilePath, newFilePath);
}

// ***********************************************************************
void File::Copy(const char* originalFilePath, const char* newFilePath)
{
}
