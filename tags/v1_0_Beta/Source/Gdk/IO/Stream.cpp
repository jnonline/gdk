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

// ***********************************************************************
Stream::Stream()
{
}

// ***********************************************************************
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

// ***********************************************************************
wstring Stream::ReadWString()
{
	// Read the string length
	wstring::size_type length = ReadUInt16();
	wstring result;
	result.reserve(length + 1);

	// Read the string characters
	for(wstring::size_type i=0; i<length; i++) 
	{ 
		wchar_t c = ReadUInt16();
		result.push_back(c); 
	}

	return result;
}

// ***********************************************************************
void Stream::WriteString(const char* value)
{
	// Add the string length
	UInt16 length = strlen(value);
	WriteUInt16(length);
	
	// Add the string characters
	Write((Byte*)value, length);
}

// ***********************************************************************
void Stream::WriteWString(const wchar_t* value)
{
	// Add the string length
	UInt16 length = wcslen(value);
	WriteUInt16(length);
	
	Write((Byte*)value, length * sizeof(wchar_t));
}


// ***********************************************************************
bool Stream::Decompress(Stream* destination, CompressionType::Enum compressionType)
{
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