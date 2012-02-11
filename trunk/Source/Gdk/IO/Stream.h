/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	/// @addtogroup IO
    /// @{
    
	// =================================================================================
    /// @brief
    ///     Enumeration of starting positions for the Stream::Seek operations
    /// @see
    ///     Stream::Seek
	// =================================================================================
	namespace SeekOrigin
	{
		enum Enum
		{	
            /// Seek from the beginning of the stream
			Begin,		
            
            /// Seek from the current position within the stream
			Current,
            
            /// Seek from the end of the stream
			End			
		};
	}

    // =================================================================================
    /// @brief
    ///     Enumeration of compression types supported by streams
	// =================================================================================
	namespace CompressionType
	{
		enum Enum
		{	
            /// Standard ZLib based compression
			ZLib
		};
	}

	// =================================================================================
    /// @brief
    ///     Provides methods for reading, writing, and accessing a generic stream of bytes
	// =================================================================================
	class Stream
	{
	public:
		
		// Public Virtual Methods
		// ================================

		virtual ~Stream();

		// -----------------------------------------------------------------------------------------
        /// @name Control Methods
        /// @{
        
        /// @brief
        ///     Checks if the stream can be Read from
        virtual bool CanRead() = 0;
		
        /// @brief
        ///     Checks if the stream can be Written to
		virtual bool CanWrite() = 0;
		
        /// @brief
        ///     Checks if the current position within the stream can be changed [via the Seek() method] 
        virtual bool CanSeek() = 0;
		
        /// @brief
        ///     Sets the current position within the stream
        /// @param position
        ///     The new position to set in the stream
        virtual void SetPosition(int position) = 0;

        /// @brief
        ///     Gets the current position within the stream
        virtual int GetPosition() = 0;

        /// @brief
        ///     Changes the current position within the stream
        /// @param offset
        ///     Offset to seek within the stream.
        /// @param origin
        ///     Origin which the offset is relative to.
        virtual void Seek(int offset, SeekOrigin::Enum origin) = 0;

        /// @brief
        ///     Changes the length of the stream. (in bytes)
        /// @param length
        ///     New length for the stream (in bytes)
        /// @note
        ///     Not all streams support resizing
        virtual void SetLength(int length) = 0;

        /// @brief
        ///     Gets the current length of the stream.  (in bytes)
        virtual int GetLength() = 0;

        /// @brief
        ///     Closes the stream
        virtual void Close() = 0;

        /// @brief
        ///     Flushes any intermediate buffers and ensures the stream is 
        ///     completely written to file/disk/io/etc
        virtual void Flush() = 0;

        /// @brief
        ///     Checks if the internal stream position is at the end of the stream
        inline bool AtEnd() { return this->GetPosition() >= this->GetLength(); }

        
        /// @}
		// -----------------------------------------------------------------------------------------
        /// @name Reading Methods
        /// @{
        
        /// @brief
        ///     Reads a fixed size set of bytes from the stream into a buffer
        ///     and moves the current position within the stream.
        /// @param destination
        ///     Buffer to read the bytes into.
        /// @param size
        ///     Number of bytes to read.
        virtual int Read(void* destination, int size) = 0;

        
		/// @brief
        ///     Reads a bool value from the stream.
        inline bool ReadBool()		{Int8 value; Read(&value, sizeof(Int8)); return value!=0;}
        
        /// @brief
        ///     Reads an Int8 value from the stream.
        inline Int8 ReadInt8()		{Int8 value; Read(&value, sizeof(Int8)); return value;}
        
        /// @brief
        ///     Reads an Int16 value from the stream.
        inline Int16 ReadInt16()	{Int16 value; Read(&value, sizeof(Int16)); return value;}
        
        /// @brief
        ///     Reads an Int32 value from the stream.
        inline Int32 ReadInt32()	{Int32 value; Read(&value, sizeof(Int32)); return value;}
        
        /// @brief
        ///     Reads an Int64 value from the stream.
        inline Int64 ReadInt64()	{Int64 value; Read(&value, sizeof(Int64)); return value;}
        
        /// @brief
        ///     Reads an UInt8  value from the stream.
        inline UInt8 ReadUInt8()	{UInt8 value; Read(&value, sizeof(UInt8)); return value;}
        
        /// @brief
        ///     Reads an UInt16 value from the stream.
        inline UInt16 ReadUInt16()	{UInt16 value; Read(&value, sizeof(UInt16)); return value;}
        
        /// @brief
        ///     Reads an UInt32 value from the stream.
        inline UInt32 ReadUInt32()	{UInt32 value; Read(&value, sizeof(UInt32)); return value;}
        
        /// @brief
        ///     Reads an UInt64 value from the stream.
        inline UInt64 ReadUInt64()	{UInt64 value; Read(&value, sizeof(UInt64)); return value;}
        
        /// @brief
        ///     Reads a float value from the stream.
        inline float ReadFloat()	{float value; Read(&value, sizeof(float)); return value;}
        
        /// @brief
        ///     Reads a double value from the stream.
        inline double ReadDouble()	{double value; Read(&value, sizeof(double)); return value;}

        /// @brief
        ///     Reads a string value from the stream.
        std::string ReadString();

		/// @}
		// -----------------------------------------------------------------------------------------
        /// @name Writing Methods
        /// @{
        
        /// @brief
        ///     Writes a buffer of bytes of a given size to the stream and 
        ///     moves the current position within the stream.
        /// @param source
        ///     Buffer of bytes to be written.
        /// @param size
        ///     Number of bytes to write.
        virtual int Write(void* source, int size) = 0;

        /// @brief
        ///     Writes a bool value to the stream.
        inline void WriteBool(bool value)		{Int8 realValue = value; Write(&realValue, sizeof(Int8));}
        
		/// @brief
        ///     Writes an Int8 value to the stream.
        inline void WriteInt8(Int8 value)		{Write(&value, sizeof(Int8));}
        
		/// @brief
        ///     Writes an Int16 value to the stream.
        inline void WriteInt16(Int16 value)		{Write(&value, sizeof(Int16));}
        
		/// @brief
        ///     Writes an Int32 value to the stream.
        inline void WriteInt32(Int32 value)		{Write(&value, sizeof(Int32));}
        
		/// @brief
        ///     Writes an Int64 value to the stream.
        inline void WriteInt64(Int64 value)		{Write(&value, sizeof(Int64));}
		
		/// @brief
        ///     Writes an UInt8 value to the stream.
        inline void WriteUInt8(UInt8 value)		{Write(&value, sizeof(UInt8));}
		
		/// @brief
        ///     Writes an UInt16 value to the stream.
        inline void WriteUInt16(UInt16 value)	{Write(&value, sizeof(UInt16));}
		
		/// @brief
        ///     Writes an UInt32 value to the stream.
        inline void WriteUInt32(UInt32 value)	{Write(&value, sizeof(UInt32));}
		
		/// @brief
        ///     Writes an UInt64 value to the stream.
        inline void WriteUInt64(UInt64 value)	{Write(&value, sizeof(UInt64));}
		
		/// @brief
        ///     Writes a float value to the stream.
        inline void WriteFloat(float value)		{Write(&value, sizeof(float));}
		
		/// @brief
        ///     Writes a double value to the stream.
        inline void WriteDouble(double value)	{Write(&value, sizeof(double));}

        /// @brief
        ///     Writes a string value to the stream.
        void WriteString(const char* value);

        /// @}
		// -----------------------------------------------------------------------------------------
        /// @name Compression Methods
        /// @{

        bool Decompress(Stream* destination, CompressionType::Enum compressionType);	

		// Compresses this stream into the given stream
		// TODO(P2)  int Compress(Stream* destination, CompressionType::Enum compressionType);
        
        /// @}
        
    protected:
		
        // Protected Methods
		// ================================
        
		Stream();
        
	};
    
    /// @} // IO

} // namespace Gdk
