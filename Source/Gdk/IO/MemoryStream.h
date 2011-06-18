
#pragma once

#include "GdkLIB.h"
#include "Stream.h"

namespace Gdk
{
	// ======================================
	class MemoryStream : public Stream
	{
	protected:
		void* buffer;			// Pointer to the buffer containing the stream data
		int bufferSize;			// Size of the buffer (NOT the stream)
		int streamLength;		// Size of the data in the stream (streamLength <= bufferSize)
		bool autoGrow;			// Does the stream auto-grow when the buffer is full?
		int position;			// Current position within the stream
		bool bufferOwner;		// Do we own the buffer?  (IE: Are we responsible for cleanup)

		// Internal Methods
		void EnsureSpace(int totalBytes);
		void Resize(int newSize);

	public:
		// Constructor
		MemoryStream(void* buffer, int size);      // Creates a NON-EXPANDABLE memory stream on a fixed buffer
		MemoryStream(int initialSize);             // Creates an EXPANDABLE memory stream of the given initial size

		// Gets the total capacity of the stream buffer
		int GetCapacity();

		// Sets the total capacity of the stream buffer
		void SetCapacity();

		// Gets a pointer to the beginning of the stream buffer
		void* GetBufferStartPtr();

		// Gets a pointer at the current position within the stream buffer
		void* GetBufferCurrentPtr();
    
		// Destructor
		virtual ~MemoryStream();
	    
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
