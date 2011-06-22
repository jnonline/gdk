/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "FileStream.h"

using namespace std;
using namespace Gdk;

// ***********************************************************************
FileStream::FileStream(const char *filePath, FileMode::Enum mode)
{
	file = NULL;

	// First, open the file stream
	if(mode == FileMode::Read)
	{
		file = fopen(filePath, "rb");
	}
	else if(mode == FileMode::Write)
	{
		file = fopen(filePath, "wb");
	}

#ifdef GDKPLATFORM_WINDOWS
	// Asset the file opened
	ASSERT(file != NULL, L"Failed to open the file [%hs] Error(%d): %hs", filePath, errno, _sys_errlist[errno]);
#else
    // Asset the file opened
	ASSERT(file != NULL, L"Failed to open the file [%hs] Error(%d): %hs", filePath, errno, sys_errlist[errno]);
#endif
        
	this->mode = mode;
}

// ***********************************************************************
FileStream::~FileStream()
{
	// Close the file
	Close();
}

// ***********************************************************************
bool FileStream::CanRead()
{
	return this->mode == FileMode::Read;
}

// ***********************************************************************
bool FileStream::CanWrite()
{
	return this->mode == FileMode::Write;
}

// ***********************************************************************
bool FileStream::CanSeek()
{
	return true;
}

// ***********************************************************************
void FileStream::SetPosition(int position)
{
	fseek(this->file, position, SEEK_SET);
}

// ***********************************************************************
int FileStream::GetPosition()
{
	return ftell(this->file);
}

// ***********************************************************************
void FileStream::Seek(int offset, SeekOrigin::Enum origin)
{
	if(origin == SeekOrigin::Begin)
		fseek(this->file, offset, SEEK_SET);
	else if(origin == SeekOrigin::End)
		fseek(this->file, offset, SEEK_END);
	else
		fseek(this->file, offset, SEEK_CUR);
}

// ***********************************************************************
void FileStream::SetLength(int length)
{
	// TODO(P2):   Implement stream re-sizing for file streams
}

// ***********************************************************************
int FileStream::GetLength()
{
	// Store the current cursor
	fpos_t currentPosition;
	fgetpos(this->file, &currentPosition);

	// Move to the end of the file
	fseek(this->file, 0, SEEK_END);

	// Get the file size
	int length = ftell(this->file);

	// Restore the cusor
	fsetpos(this->file, &currentPosition);

	return length;
}

// ***********************************************************************
void FileStream::Close()
{
	// If the stream is open, close it
	if(this->file != NULL)
	{
		fflush(this->file);
		fclose(this->file);
		this->file = NULL;
	}
}

// ***********************************************************************
void FileStream::Flush()
{
	// Flush the stream
	fflush(this->file);
}

// ***********************************************************************
int FileStream::Read(void* destination, int size)
{
	return (int) fread(destination, 1, size, this->file);
}

// ***********************************************************************
int FileStream::Write(void* source, int size)
{
	return (int) fwrite(source, 1, size, this->file);
}
