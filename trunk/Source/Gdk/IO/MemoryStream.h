/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Stream.h"

namespace Gdk
{
    /// @addtogroup IO
    /// @{

	// =================================================================================
    /// @brief
    ///     Provides a stream based interface to an in-memory buffer of bytes.
	// =================================================================================
	class MemoryStream : public Stream
	{
    public:
        
        // Public Methods
		// ================================
        
        // ---------------------------------
        /// @name Constructor / Destructor
        /// @{
        
		MemoryStream(void* buffer, int size);
		MemoryStream(int initialSize);
		virtual ~MemoryStream();

        /// @}
        // ---------------------------------
        /// @name Buffer Management Methods
        /// @{
        
		int GetCapacity();
		void SetCapacity();
		void* GetBufferStartPtr();
		void* GetBufferCurrentPtr();
        
        /// @}
		// -----------------------------------------------------------------------------------------
        /// @name Override Methods
        /// @{

        virtual bool CanRead();
		virtual bool CanWrite();
		virtual bool CanSeek();
		virtual void SetPosition(int position);
		virtual int GetPosition();
		virtual void Seek(int offset, SeekOrigin::Enum origin);
		virtual void SetLength(int length);
		virtual int GetLength();
		virtual void Close();
		virtual void Flush();
		virtual int Read(void* destination, int size);
		virtual int Write(void* source, int size);
        
        /// @}
        
    private:
        
        // Private Properties
		// ================================
        
		void* buffer;			// Pointer to the buffer containing the stream data
		int bufferSize;			// Size of the buffer (NOT the stream)
		int streamLength;		// Size of the data in the stream (streamLength <= bufferSize)
		bool autoGrow;			// Does the stream auto-grow when the buffer is full?
		int position;			// Current position within the stream
		bool bufferOwner;		// Do we own the buffer?  (IE: Are we responsible for cleanup)
        
        // Private Methods
		// ================================
        
		void EnsureSizeIsBigEnough(int totalBytes);
		void Resize(int newSize);
	};
    
    /// @} // IO

} // namespace Gdk
