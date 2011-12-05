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
    ///     Enumeration of modes for file streams
	// =================================================================================
    namespace FileMode
	{
		enum Enum
		{	
			Read,
			Write
		};
	}

	// =================================================================================
    /// @brief
    ///     Provides a stream based interface to a file.
	// =================================================================================
	class FileStream : public Stream
	{
	public:
        
        // Public Methods
		// ================================
        
        // ---------------------------------
        /// @name Constructor / Destructor
        /// @{
        
		FileStream(const char *filePath, FileMode::Enum mode); 
        virtual ~FileStream();
	    
        /// @}
		        
        // ---------------------------------
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
        
		// File handle
		FILE *file;
		FileMode::Enum mode;
	};
    
    /// @} // IO

} // namespace Gdk
