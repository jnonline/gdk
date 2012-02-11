/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Stream.h"

#include "zlib.h"
#include "zconf.h"

using namespace std;
using namespace Gdk;

#define COMPRESSION_BUFFER_SIZE		65536			// Size of temp buffers used for compression

// *****************************************************************
/// @brief
///     Default Constructor
// *****************************************************************
Stream::Stream()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
Stream::~Stream()
{
}

// *****************************************************************
/// @brief
///     Reads a char* string from the stream
/// @remarks
///     char* strings are stored as a 2-byte string length, followed by
///     the actual character data of the string, NOT including the NULL terminator.
// *****************************************************************
string Stream::ReadString()
{
	// Read the string length
	string::size_type length = ReadUInt16();
	string result;
	result.reserve(length + 1);

	// Read the string characters
	for(string::size_type i=0; i<length; i++) 
	{ 
		char c = ReadUInt8();
		result.push_back(c); 
	}

	return result;
}

// *****************************************************************
/// @brief
///     Writes a char* string to the stream
/// @remarks
///     char* strings are stored as a 2-byte string length, followed by
///     the 8-bit characters of the string, NOT including the NULL terminator.
// *****************************************************************
void Stream::WriteString(const char* value)
{
	// Add the string length
	UInt16 length = strlen(value);
	WriteUInt16(length);
	
	// Add the string characters
	Write((Byte*)value, length);
}

// *****************************************************************
/// @brief
///     Decompresses this stream into another stream
/// @param destination
///     Stream to where the decompressed data is to be written
/// @param compressionType
///     Which compression type the data is compressed with
// *****************************************************************
bool Stream::Decompress(Stream* destination, CompressionType::Enum compressionType)
{
    // TODO(P2): This method should take a # of bytes to decompress...   
    
	switch(compressionType)
	{
		// ---------------------------------- ZLib ------------------------------------
		case CompressionType::ZLib:
		{
			int ret;
			unsigned have;
			z_stream strm;
			unsigned char in[COMPRESSION_BUFFER_SIZE];
			unsigned char out[COMPRESSION_BUFFER_SIZE];

			/* allocate inflate state */
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			strm.avail_in = 0;
			strm.next_in = Z_NULL;
			ret = inflateInit(&strm);
			if (ret != Z_OK)
				return false;

			/* decompress until deflate stream ends or end of file */
			do 
			{
				// Read in from this stream
				strm.avail_in = this->Read(in, COMPRESSION_BUFFER_SIZE);
		        
				// Was there no data left in the stream?
				if (strm.avail_in == 0)
					break;

				strm.next_in = in;

				/* run inflate() on input until output buffer not full */
				do 
				{
					strm.avail_out = COMPRESSION_BUFFER_SIZE;
					strm.next_out = out;

					ret = inflate(&strm, Z_NO_FLUSH);
		            
					switch (ret) 
					{
						case Z_NEED_DICT:
							ret = Z_DATA_ERROR;     /* and fall through */
						case Z_DATA_ERROR:
						case Z_MEM_ERROR:
						case Z_STREAM_ERROR:
							(void)inflateEnd(&strm);
							return false;
					}

					have = COMPRESSION_BUFFER_SIZE - strm.avail_out;

					// Write the decompressed data to the destination stream
					destination->Write(out, have);

				} while (strm.avail_out == 0);

				/* done when inflate() says it's done */
			} while (ret != Z_STREAM_END);

			 /* clean up and return */
			(void)inflateEnd(&strm);
		}
		break;
	}

	return true;
}