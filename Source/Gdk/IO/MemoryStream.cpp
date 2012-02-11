/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "MemoryStream.h"

using namespace std;
using namespace Gdk;

// *****************************************************************
/// @brief
///     Creates a memory stream around an existing buffer
/// @param buffer
///     The buffer to expose as a stream
/// @param size
///     The size of the buffer
/// @note
///     The caller is responsible for managing and freeing the buffer memory.
// *****************************************************************
MemoryStream::MemoryStream(void* buffer, int size)
{
	// Use the given buffer pointer
	this->buffer = buffer;
	this->bufferSize = size;
	this->streamLength = size;
	this->autoGrow = false;
	this->position = 0;
	this->bufferOwner = false;
}

// *****************************************************************
/// @brief
///     Creates a memory stream with an internal auto-growing buffer.
/// @param initialSize
///     The initial size (in bytes) of the memory buffer
/// @note
///     The MemoryStream owns the buffer memory, and will free it when the 
///     stream is destroyed.
// *****************************************************************
MemoryStream::MemoryStream(int initialSize)
{
	// Allocate the initial buffer
	this->buffer = GdkAlloc(initialSize);
	this->bufferSize = initialSize;
	this->streamLength = 0;
	this->autoGrow = true;
	this->position = 0;
	this->bufferOwner = true;
	
	// Verify the allocation
	ASSERT(this->buffer != NULL, "Memorystream Allocation Failed: Bytes[%d]", initialSize);
}

// *****************************************************************
/// @brief
///     Destructor
/// @remarks
///     Also frees the internal buffer if the memory stream is managing it.
// *****************************************************************
MemoryStream::~MemoryStream()
{
	// Do we own the stream buffer?
	if(this->bufferOwner == true)
	{
		// Release the stream buffer
		if(buffer != NULL)
		{
			GdkFree(buffer);
			buffer = NULL;
		}
	}
}

// *****************************************************************
/// @brief
///     Checks if the stream can be Read from
// *****************************************************************
bool MemoryStream::CanRead()
{
	return true;
}

// *****************************************************************
/// @brief
///     Checks if the stream can be Written to
// *****************************************************************
bool MemoryStream::CanWrite()
{
	return true;
}

// *****************************************************************
/// @brief
///     Checks if the current position within the stream can be changed [via the Seek() method] 
// *****************************************************************
bool MemoryStream::CanSeek()
{
	return true;
}

// *****************************************************************
/// @brief
///     Sets the current position within the stream.
/// @param position
///     The new position to set in the stream
// *****************************************************************
void MemoryStream::SetPosition(int position)
{
	this->position = position;
}

// *****************************************************************
/// @brief
///     Gets the current position within the stream
// *****************************************************************
int MemoryStream::GetPosition()
{
	return this->position;
}

// *****************************************************************
/// @brief
///     Changes the current position within the stream
/// @param offset
///     Offset to seek within the stream.
/// @param origin
///     Origin which the offset is relative to.
// *****************************************************************
void MemoryStream::Seek(int offset, SeekOrigin::Enum origin)
{
	if(origin == SeekOrigin::Begin)
		this->position = offset;
	else if(origin == SeekOrigin::End)
		this->position = this->streamLength + offset;
	else 
		this->position += offset;

	// Verify user didnt jump outside the stream
	ASSERT(this->position >= 0 && this->position <= this->streamLength, "Seek has jumped outside the stream");
}

// *****************************************************************
/// @brief
///     Changes the length of the stream. (in bytes)
/// @param length
///     New length for the stream (in bytes)
/// @note
///     If the length is greater than the current buffer size and the
///     MemoryStream instance supports Auto-growing, the buffer will be resized
// *****************************************************************
void MemoryStream::SetLength(int length)
{
	// Ensure the buffer is big enough for the new length
	EnsureSizeIsBigEnough(length);

	// Use the new length
	this->streamLength = length;

	// If the position is now outside the stream, set it to the end
	if(this->position > this->streamLength)
		this->position = this->streamLength;
}

// *****************************************************************
/// @brief
///     Gets the current length of the stream.  (in bytes)
// *****************************************************************
int MemoryStream::GetLength()
{
	return this->streamLength;
}

// *****************************************************************
/// @brief
///     Closes the stream
// *****************************************************************
void MemoryStream::Close()
{
}

// *****************************************************************
/// @brief
///     Flushes any intermediate buffers and ensures the stream is 
///     completely written to file/disk/io/etc
// *****************************************************************
void MemoryStream::Flush()
{
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
int MemoryStream::Read(void* destination, int size)
{
	// Limit the size if the requested size will go outside the stream
	int actualSize = size;
	if(size + this->position > this->streamLength)
		actualSize = this->streamLength - this->position;

	// Copy memory from the current position to the given buffer
	void* cursor = (void*)((Byte*)this->buffer + this->position);
	memcpy(destination, cursor, actualSize);

	// Advance the stream position
	position += actualSize;
	return actualSize;
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
int MemoryStream::Write(void* source, int size)
{
	// Ensure there is enough space for the given size
	EnsureSizeIsBigEnough(this->position + size);

	// Copy memory from the given buffer to the current stream position
	void* cursor = (void*)((Byte*)this->buffer + this->position);
	memcpy(cursor, source, size);

	// Advance the stream position
	this->position += size;

	// Advance the stream length, if the caller has written past the end of the stream
	if(this->position > this->streamLength)
		this->streamLength = this->position;

	return size;
}

// *****************************************************************
/// @brief
///     Makes sure the buffer is at least as big as the given number of bytes.
/// @remarks
///     The buffer will auto-resize if there is not enough space, growing by double every time
///     it needs to resize;
/// @param totalbytes
///     The total number of bytes that the caller wishes to support in the buffer
/// @note
///     This is only supported on internally managed buffers
// *****************************************************************
void MemoryStream::EnsureSizeIsBigEnough(int totalBytes)
{
	// Is the requested space bigger than the current buffer size?
	if(totalBytes > this->bufferSize)
	{
		// Calculate the new size for the buffer  (using the 2X growth method)
		int newSize = this->bufferSize;
		while(newSize < totalBytes)
			newSize *= 2;

		// Resize the buffer
		Resize(newSize);
	}
}

// *****************************************************************
/// @brief
///     Resizes the buffer and copies the current bytes in the buffers.
/// @param newSize
///     The new size for the buffer.
/// @note
///     This is only supported on internally managed buffers
// *****************************************************************
void MemoryStream::Resize(int newSize)
{
	// Is this NOT an autogrow buffer?
	if(this->autoGrow == false)
	{
		ASSERT(false, "Resize called on a non-growing MemoryStream");
	}

	// Allocate a new buffer
	void* newBuffer = GdkAlloc(newSize);

	// Copy the memory over
    int bytesToCopy = this->streamLength;
    if(bytesToCopy > newSize)
        bytesToCopy = newSize;
	memcpy(newBuffer, this->buffer, bytesToCopy);

	// Release the old buffer
	GdkFree(this->buffer);

	// Use the new buffer & size
	this->buffer = newBuffer;
	this->bufferSize = newSize;

	// Truncate the stream if the buffer is now smaller than the stream
	if(this->streamLength > this->bufferSize)
		this->streamLength = this->bufferSize;
	if(this->position > this->bufferSize)
		this->position = this->bufferSize;
}

// *****************************************************************
/// @brief
///     Gets the current capacity of the internal buffer.
/// @remarks
///     The capacity is the total size of the internal buffer, which
///     can be more than the number of bytes in the stream.
// *****************************************************************
int MemoryStream::GetCapacity()
{
	return this->bufferSize;
}

// *****************************************************************
/// @brief
///     Sets the capacity of the internal buffer
/// @remarks
///     The capacity is the total size of the internal buffer, which
///     can be more than the number of bytes in the stream.
/// @note
///     This is only supported on internally managed buffers
// *****************************************************************
void MemoryStream::SetCapacity()
{
	Resize(this->bufferSize);
}

// *****************************************************************
/// @brief
///     Gets a void* at the beginning of the stream's internal buffer
// *****************************************************************
void* MemoryStream::GetBufferStartPtr()
{
	return this->buffer;
}

// *****************************************************************
/// @brief
///     Gets a void* at the current position within the stream's internal buffer
// *****************************************************************
void* MemoryStream::GetBufferCurrentPtr()
{
	return (void*)((Byte*)this->buffer + this->position);
}