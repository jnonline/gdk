/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Path.h"

using namespace Gdk;

// Static instantiations
const char Path::PathSeperator  = '/';
const int Path::MaxPathLength = 256;

// *****************************************************************
/// @brief
///     Combines two parts of a path.
/// @remarks
///     The method will ensure that two parts have a single '/' path delimiter between them.
/// @param path1
///     Part 1 of the final path
/// @param newFilePath
//      Part 2 of the final path
// *****************************************************************
string Path::Combine(const char* path1, const char* path2)
{
	string result(path1);

	// Does path1 need a trailing seperator?
	int lenPath1 = strlen(path1);
	if(lenPath1 > 0)
	{
		if(path1[lenPath1-1] != '/' && path1[lenPath1-1] != '\\')
			result += Path::PathSeperator;
	}

	// Does path2 have a prefixing seperator?
	if(path2[0] == '/' || path2[0] == '\\')
		result.append(path2 + 1);
	else
		result.append(path2);

	return result;
}

// *****************************************************************
/// @brief
///     Gets the Directory part of a path.
/// @remarks
///     Returns the entire path before the last path seperator ('/' or '\') in the input path.
///     For example:  "C:/foo/bar.txt"  returns  "C:/foo"
/// @param input
///     Path to get the directory from
// *****************************************************************
string Path::GetDirectory(const char* input)
{
	int len = strlen(input);
	int index = len-1;

	// Walk backwards to find the last path seperator, and return everything before it
	while(index >= 0)
	{
		if(input[index] == '/' || input[index] == '\\')
		{
			string result(input, index);
			return result;
		}
		index--;
	}

	// No seperator found, return an emtpy path
	return string();
}

// *****************************************************************
/// @brief
///     Gets the FileName part of a path.
/// @remarks
///     Returns the entire filename after the last path seperator ('/' or '\') in the input path.
///     For example:  "C:/foo/bar.txt"  returns  "bar.txt"
/// @param input
///     Path to get the filename from
// *****************************************************************
string Path::GetFileName(const char* input)
{
	int len = strlen(input);
	int index = len-1;

	// If the path ends with a path seperator, return blank
	if(input[index] == '/' || input[index] == '\\')
		return string();

	// Walk backwards to find the last path seperator, and return everything after it
	while(index >= 0)
	{
		if(input[index] == '/' || input[index] == '\\')
		{
			string result(input + index + 1);
			return result;
		}
		index--;
	}

	// No seperator found, return the whole path as the file name
	return string(input);
}

// *****************************************************************
/// @brief
///     Gets the FileName part of a path, without the extension
/// @remarks
///     Returns the entire filename between the last path seperator ('/' or '\') in the input path and the last
///     extension seperator '.'
///     For example:  "C:/foo/bar.txt"  returns  "bar"
/// @param input
///     Path to get the filename from
// *****************************************************************
string Path::GetFileNameWithoutExtension(const char* input)
{
	// Get the file name
	string fileName = GetFileName(input);

	int len = fileName.length();
	int index = len-1;

	// Walk backwards to find the last '.' and return everything before it
	while(index >= 0)
	{
		if(fileName[index] == '.')
		{
			string result(fileName, 0, index);
			return result;
		}
		index--;
	}

	// No seperator found, there is no extension
	return fileName;
}

// *****************************************************************
/// @brief
///     Gets the extension part of a path
/// @remarks
///     Returns the path part after the last extension seperator '.' in the input path.
///     For example:  "C:/foo/bar.txt"  returns  "txt"
/// @param input
///     Path to get the extension from
// *****************************************************************
string Path::GetExtension(const char*  input)
{
	int len = strlen(input);
	int index = len-1;
    
	// If the path ends with a '.' return blank
	if(input[index] == '.')
		return string();
    
	// Walk backwards to find the last '.' and return everything after it
	while(index >= 0)
	{
		if(input[index] == '.')
		{
			string result(input + index + 1);
			return result;
		}
		index--;
	}
    
	// No '.' found, there is no extension
	return string();
}

// *****************************************************************
/// @brief
///     Changes the extension of a path string
/// @remarks
///     If no prior extension exists, the new one is appended.  
///     The prepended period in the extension is not necessary.
///     For example:  "C:/foo/bar.txt"  returns  "C:/foo/bar.newExt"
/// @param input
///     Path string to be modified.
/// @param extentsion
///     New extension for the input path
// *****************************************************************
string Path::ChangeExtension(const char* input, const char* extension)
{
	int len = strlen(input);
	int index = len-1;

	// Walk backwards in the input string
	while(index >= 0)
	{
		// Is this the last '.' in the input string?
		if(input[index] == '.')
		{
			// Append the new extension from this point forward
			string result(input, index);
			if(extension[0] != '.')
				result += '.';
			result.append(extension);
			return result;
		}

		// Is this a path seperator?
		if(input[index] == '/' || input[index] == '\\')
		{
			// The file name portion of the path doesnt have an existing extension. so let's append the new one
			string result(input);
			if(extension[0] != '.')
				result += '.';
			result.append(extension);
			return result;
		}
		
		index--;
	}

	// No '.' or '/' found, so lets just set the extension on the original input
	string result(input);
	if(extension[0] != '.')
		result += '.';
	result.append(extension);
	return result;
}

// *****************************************************************
/// @brief
///     Gets the root directory of the specified path
/// @remarks
///     For example:  "C:/foo/bar.txt"  returns  "C:/"
/// @param input
///     Path string to get the root of.
// *****************************************************************
string Path::GetPathRoot(const char* input)
{
	int index = 0;
	int len = strlen(input);

	// Walk forward to the first path seperator (and return everything before it, including the seperator)
	while(index < len)
	{
		if(input[index] == '/' || input[index] == '\\')
		{
			string result(input + index + 1);
			return result;
		}
		index++;
	}

	// No path seperator found
	return string();
}

// *****************************************************************
/// @brief
///     Gets one of the common paths used by the GDK
/// @remarks
///     The GDK automatically manages several paths for you, such as where
///     the application binaries are, where the assets and resources are, or
///     where you are allowed to write save files.
/// @param path
///     Specifies which common path to get.
// *****************************************************************
string Path::GetCommonPath(CommonPaths::Enum path)
{
	// Get the platform specific path
	string result = _Gdk_Platform_IO_GetCommonPath(path);

	// If we got here, then the path isnt defined.
	ASSERT(result.size() > 0, "Unable to resolve the path for the CommonPath value [%d]", (int)path); 
    
    return result;
}
