
#pragma once

#include "GdkLIB.h"
#include "Stream.h"

namespace Gdk
{
	// ======================================

	namespace FileMode
	{
		enum Enum
		{	
			Read,
			Write
		};
	}

	// ======================================
	class FileStream : public Stream
	{
	protected:
		// File handle
		FILE *file;
		FileMode::Enum mode;

	public:
		// Constructor
		FileStream(const char *filePath, FileMode::Enum mode); 

		// Destructor
		virtual ~FileStream();
	    
		// Overrides from: Stream
		// ---------------------------------

		// Checks if the stream can be Read from
		virtual bool CanRead();
		
		// Checks if the stream can be Written to
		virtual bool CanWrite();
		
		// Checks if the current position within the stream can be changed [via the Seek() method]
		virtual bool CanSeek();
		
		// Sets the current position within the stream
		virtual void SetPosition(int position);

		// Gets the current position within the stream
		virtual int GetPosition();

		// Changes the current position within the stream
		virtual void Seek(int offset, SeekOrigin::Enum origin);

		// Changes the length of the stream
		virtual void SetLength(int length);

		// Gets the current length of the stream
		virtual int GetLength();

		// Closes the stream
		virtual void Close();

		// Flushes any intermediate buffers and ensures the stream is completely written to file/disk/io/etc
		virtual void Flush();

		// Reads a fixed size set of bytes from the stream into a buffer
		virtual int Read(void* destination, int size);

		// Writes a buffer of bytes of a given size to the stream
		virtual int Write(void* source, int size);
	};

} // namespace Gdk
