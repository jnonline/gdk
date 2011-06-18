
#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ======================================

	namespace SeekOrigin
	{
		enum Enum
		{	
			Begin,		// Seek from the beginning of the file
			Current,	// Seek from the current position
			End			// Seek from the end of the file
		};
	}

	// ======================================

	namespace CompressionType
	{
		enum Enum
		{	
			ZLib
		};
	}

	// ======================================

	class Stream
	{
	protected:
		// Abstract CTor
		Stream();

	public:
		
		// Interfaces for derived classes
		// ---------------------------------

		// Destructor
		virtual ~Stream() {};

		// Checks if the stream can be Read from
		virtual bool CanRead() = 0;
		
		// Checks if the stream can be Written to
		virtual bool CanWrite() = 0;
		
		// Checks if the current position within the stream can be changed [via the Seek() method]
		virtual bool CanSeek() = 0;
		
		// Sets the current position within the stream
		virtual void SetPosition(int position) = 0;

		// Gets the current position within the stream
		virtual int GetPosition() = 0;

		// Changes the current position within the stream
		virtual void Seek(int offset, SeekOrigin::Enum origin) = 0;

		// Changes the length of the stream
		virtual void SetLength(int length) = 0;

		// Gets the current length of the stream
		virtual int GetLength() = 0;

		// Closes the stream
		virtual void Close() = 0;

		// Flushes any intermediate buffers and ensures the stream is completely written to file/disk/io/etc
		virtual void Flush() = 0;

		// Reads a fixed size set of bytes from the stream into a buffer
		virtual int Read(void* destination, int size) = 0;

		// Writes a buffer of bytes of a given size to the stream
		virtual int Write(void* source, int size) = 0;

		// Read Utilities
		// ---------------------------

		// Read for common types
		inline bool ReadBool()		{Int8 value; Read(&value, sizeof(Int8)); return value!=0;}
		inline Int8 ReadInt8()		{Int8 value; Read(&value, sizeof(Int8)); return value;}
		inline Int16 ReadInt16()	{Int16 value; Read(&value, sizeof(Int16)); return value;}
		inline Int32 ReadInt32()	{Int32 value; Read(&value, sizeof(Int32)); return value;}
		inline Int64 ReadInt64()	{Int64 value; Read(&value, sizeof(Int64)); return value;}
		inline UInt8 ReadUInt8()	{UInt8 value; Read(&value, sizeof(UInt8)); return value;}
		inline UInt16 ReadUInt16()	{UInt16 value; Read(&value, sizeof(UInt16)); return value;}
		inline UInt32 ReadUInt32()	{UInt32 value; Read(&value, sizeof(UInt32)); return value;}
		inline UInt64 ReadUInt64()	{UInt64 value; Read(&value, sizeof(UInt64)); return value;}
		inline float ReadFloat()	{float value; Read(&value, sizeof(float)); return value;}
		inline double ReadDouble()	{double value; Read(&value, sizeof(double)); return value;}

		// Reads a char* string from the stream
		std::string ReadString();

		// Reads a wchar_t* wide string to the stream
		std::wstring ReadWString();

		// Checks if the stream is past its end
		inline bool AtEnd() { return this->GetPosition() >= this->GetLength(); }

		// Write Utilities
		// ---------------------------

		// Write for common types
		inline void WriteBool(bool value)		{Int8 realValue = value; Write(&realValue, sizeof(Int8));}
		inline void WriteInt8(Int8 value)		{Write(&value, sizeof(Int8));}
		inline void WriteInt16(Int16 value)		{Write(&value, sizeof(Int16));}
		inline void WriteInt32(Int32 value)		{Write(&value, sizeof(Int32));}
		inline void WriteInt64(Int64 value)		{Write(&value, sizeof(Int64));}
		inline void WriteUInt8(UInt8 value)		{Write(&value, sizeof(UInt8));}
		inline void WriteUInt16(UInt16 value)	{Write(&value, sizeof(UInt16));}
		inline void WriteUInt32(UInt32 value)	{Write(&value, sizeof(UInt32));}
		inline void WriteUInt64(UInt64 value)	{Write(&value, sizeof(UInt64));}
		inline void WriteFloat(float value)		{Write(&value, sizeof(float));}
		inline void WriteDouble(double value)	{Write(&value, sizeof(double));}

		// Writes a char* string to the stream
		void WriteString(const char* value);

		// Writes a wchar_t* wide string to the stream
		void WriteWString(const wchar_t* value);

		// Compression Utilities
		// --------------------------

		// Decompresses this stream into the given stream
		bool Decompress(Stream* destination, CompressionType::Enum compressionType);	

		// Compresses this stream into the given stream
		// TODO(P2)  int Compress(Stream* destination, CompressionType::Enum compressionType);
	};

} // namespace Gdk
