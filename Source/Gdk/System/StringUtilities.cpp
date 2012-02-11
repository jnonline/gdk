/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "StringUtilities.h"
#include "UTF8/unchecked.h"

using namespace Gdk;

// Common String Literals
const char* StringUtilities::Empty = "";
const char* StringUtilities::Unknown = "Unknown";

// *****************************************************************
/// @brief
///     Replaces any occurances of a search string within an input string
/// @param input
///     The source string to do the replacements in
/// @param searchText
///     The text to search for.  All occurances of this text will be replaced
/// @param replacementText
///     The text to put in place of the searchText
/// @return 
///     The result string with the replacements
// *****************************************************************
string StringUtilities::Replace(const char* input, const char* searchText, const char* replacementText)
{
	string source(input);

	// Get the length of the search string
	size_t searchLen = strlen(searchText);
	size_t pos;

	// Loop while the search text is found in the input
	while( (pos = source.find(searchText)) != string::npos)
	{
		// We found the search string, so let's replace it
		source.replace(pos, searchLen, replacementText);
	}

	return source;
}

// *****************************************************************
/// @brief
///     Generates a highly unique hash code from a buffer of bytes
/// @param data
///     The buffer of bytes to hash
/// @param len
///     Number of bytes in the buffer
/// @return 
///     A hash code of the buffer bytes
// *****************************************************************
unsigned int StringUtilities::FastHash(const UInt8* data, int len)
{
    #undef get16bits
    #if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
        #define get16bits(d) (*((const UInt16 *) (d)))
    #else
        #define get16bits(d) ((((UInt32)(((const UInt8 *)(d))[1])) << 8) +(UInt32)(((const UInt8 *)(d))[0]) )
    #endif
	
	UInt32 hash = len, tmp;
	int rem;
	
    if (len <= 0 || data == NULL) return 0;
	
    rem = len & 3;
    len >>= 2;
	
    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (UInt16);
        hash  += hash >> 11;
    }
	
    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
			hash ^= hash << 16;
			hash ^= data[sizeof (UInt16)] << 18;
			hash += hash >> 11;
			break;
        case 2: hash += get16bits (data);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
        case 1: hash += *data;
			hash ^= hash << 10;
			hash += hash >> 1;
    }
	
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
	
    return hash;
}

// *****************************************************************
/// @brief
///     Generates a highly unique hash code of a c-string
/// @param str
///     The string to hash
/// @return 
///     A hash code of the string
// *****************************************************************
unsigned int StringUtilities::FastHash(const char* str)
{
	return FastHash((UInt8*)str, strlen(str));
}


