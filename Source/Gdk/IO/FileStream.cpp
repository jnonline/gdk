/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "FileStream.h"

using namespace std;
using namespace Gdk;

// *****************************************************************
/// @brief
///     Creates a file stream to the given file path
/// @param filePath
///     Path and name of the file.
/// @note
///     If the file already exists, it is opened, with the stream position at the beginning of the file.
///     If the file does not exist, a new one is created.
// *****************************************************************
FileStream::FileStream(const char *filePath, FileMode::Enum mode)
{
	file = NULL;
	
	// Select the file mode string for the fopen call
	const char *fopenMode = "rb";
	if(mode == FileMode::Write)
		fopenMode = "wb";

	// Open the file stream
	#ifdef GDKPLATFORM_WINDOWS
		
		errno_t result = fopen_s(&file, filePath, fopenMode);

		// Asset the file opened
		#ifdef GDK_ASSERTS
		char errString[256];
		strerror_s(errString, 256, errno);
		ASSERT(result == 0 && file != NULL, "Failed to open the file [%s] Result(%d) Error(%d): %s", filePath, result, errno, errString);
		#endif
	#else
		
		file = fopen(filePath, fopenMode);

		// Asset the file opened
		ASSERT(file != NULL, "Failed to open the file [%s] Error(%d): %s", filePath, errno, sys_errlist[errno]);

	#endif
        
	this->mode = mode;
}

// *****************************************************************
/// @brief
///     Destructor
/// @remarks
///     Closes the stream if it is not already closed.
// *****************************************************************
FileStream::~FileStream()
{
	// Close the file
	Close();
}

// *****************************************************************
/// @brief
///     Checks if the stream can be Read from
// *****************************************************************
bool FileStream::CanRead()
{
	return this->mode == FileMode::Read;
}

// *****************************************************************
/// @brief
///     Checks if the stream can be Written to
// *****************************************************************
bool FileStream::CanWrite()
{
	return this->mode == FileMode::Write;
}

// *****************************************************************
/// @brief
///     Checks if the current position within the stream can be changed [via the Seek() method] 
// *****************************************************************
bool FileStream::CanSeek()
{
	return true;
}

// *****************************************************************
/// @brief
///     Sets the current position within the stream.
/// @param position
///     The new position to set in the stream
// *****************************************************************
void FileStream::SetPosition(int position)
{
	fseek(this->file, position, SEEK_SET);
}

// *****************************************************************
/// @brief
///     Gets the current position within the stream
// *****************************************************************
int FileStream::GetPosition()
{
	return ftell(this->file);
}

// *****************************************************************
/// @brief
///     Changes the current position within the stream
/// @param offset
///     Offset to seek within the stream.
/// @param origin
///     Origin which the offset is relative to.
// *****************************************************************
void FileStream::Seek(int offset, SeekOrigin::Enum origin)
{
	if(origin == SeekOrigin::Begin)
		fseek(this->file, offset, SEEK_SET);
	else if(origin == SeekOrigin::End)
		fseek(this->file, offset, SEEK_END);
	else
		fseek(this->file, offset, SEEK_CUR);
}

// *****************************************************************
/// @brief
///     Changes the length of the stream. (in bytes)
/// @param length
///     New length for the stream (in bytes)
/// @note
///     Not supported for FileStreams
// *****************************************************************
void FileStream::SetLength(int length)
{
	// TODO(P2):   Implement stream re-sizing for file streams
}

// *****************************************************************
/// @brief
///     Gets the current length of the stream.  (in bytes)
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Closes the stream
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Flushes any intermediate buffers and ensures the stream is 
///     completely written to file/disk/io/etc
// *****************************************************************
void FileStream::Flush()
{
	// Flush the stream
	fflush(this->file);
}

// *****************************************************************
/// @brief
///     Reads a fixed size set of bytes from the stream into a buffer
///     and moves the current position within the stream.
/// @param destination
///     Buffer to read the bytes into.
/// @param size
///     Number of bytes to read.
// *****************************************************************
int FileStream::Read(void* destination, int size)
{
	return (int) fread(destination, 1, size, this->file);
}

// *****************************************************************
/// @brief
///     Writes a buffer of bytes of a given size to the stream and 
///     moves the current position within the stream.
/// @param source
///     Buffer of bytes to be written.
/// @param size
///     Number of bytes to write.
// *****************************************************************
int FileStream::Write(void* source, int size)
{
	return (int) fwrite(source, 1, size, this->file);
}
