/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "MemoryStream.h"

using namespace std;
using namespace Gdk;

// ***********************************************************************
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

// ***********************************************************************
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
	ASSERT(this->buffer != NULL, L"Memorystream Allocation Failed: Bytes[%d]", initialSize);
}

// ***********************************************************************
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

// ***********************************************************************
bool MemoryStream::CanRead()
{
	return true;
}

// ***********************************************************************
bool MemoryStream::CanWrite()
{
	return true;
}

// ***********************************************************************
bool MemoryStream::CanSeek()
{
	return true;
}

// ***********************************************************************
void MemoryStream::SetPosition(int position)
{
	this->position = position;
}

// ***********************************************************************
int MemoryStream::GetPosition()
{
	return this->position;
}

// ***********************************************************************
void MemoryStream::Seek(int offset, SeekOrigin::Enum origin)
{
	if(origin == SeekOrigin::Begin)
		this->position = offset;
	else if(origin == SeekOrigin::End)
		this->position = this->streamLength + offset;
	else 
		this->position += offset;

	// Verify user didnt jump outside the stream
	ASSERT(this->position >= 0 && this->position <= this->streamLength, L"Seek has jumped outside the stream");
}

// ***********************************************************************
void MemoryStream::SetLength(int length)
{
	// Ensure the buffer is big enough for the new length
	EnsureSpace(length);

	// Use the new length
	this->streamLength = length;

	// If the position is now outside the stream, set it to the end
	if(this->position > this->streamLength)
		this->position = this->streamLength;
}

// ***********************************************************************
int MemoryStream::GetLength()
{
	return this->streamLength;
}

// ***********************************************************************
void MemoryStream::Close()
{
}

// ***********************************************************************
void MemoryStream::Flush()
{
}

// ***********************************************************************
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

// ***********************************************************************
int MemoryStream::Write(void* source, int size)
{
	// Ensure there is enough space for the given size
	EnsureSpace(this->position + size);

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

// ***********************************************************************
void MemoryStream::EnsureSpace(int totalBytes)
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

// ***********************************************************************
void MemoryStream::Resize(int newSize)
{
	// Is this NOT an autogrow buffer?
	if(this->autoGrow == false)
	{
		ASSERT(false, L"Resize called on a non-growing MemoryStream");
	}

	// Allocate a new buffer
	void* newBuffer = malloc(newSize);

	// Copy the memory over
	memcpy(newBuffer, this->buffer, this->streamLength);

	// Release the old buffer
	free(this->buffer);

	// Use the new buffer & size
	this->buffer = newBuffer;
	this->bufferSize = newSize;

	// Truncate the stream if the buffer is now smaller than the stream
	if(this->streamLength > this->bufferSize)
		this->streamLength = this->bufferSize;
	if(this->position > this->bufferSize)
		this->position = this->bufferSize;
}

// ***********************************************************************
int MemoryStream::GetCapacity()
{
	return this->bufferSize;
}

// ***********************************************************************
void MemoryStream::SetCapacity()
{
	Resize(this->bufferSize);
}

// ***********************************************************************
void* MemoryStream::GetBufferStartPtr()
{
	return this->buffer;
}

// ***********************************************************************
void* MemoryStream::GetBufferCurrentPtr()
{
	return (void*)((Byte*)this->buffer + this->position);
}