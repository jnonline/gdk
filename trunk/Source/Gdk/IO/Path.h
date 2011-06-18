
#pragma once

#include "GdkLIB.h"

namespace Gdk
{

	// ======================================

	namespace CommonPaths
	{
		enum Enum
		{	
			ApplicationFolder,	// Where the application binaries are located
			AssetFolder,		// Where GDK resources & assets are located
			WorkingFolder,		// Where the application has rights to store & retrieve data (saves, logs, etc)
		};
	}

	// ======================================

	class Path
	{
	private:
		// Static only class
		Path();

	public:
		// Combines two parts of a path (adding or removing seperators where needed)
		static string Combine(const char* path1, const char* path2);

		// Gets the Directory part of a path:  "C:\foo\bar.txt"  ->  "C:\foo"
		static string GetDirectory(const char* input);

		// Gets the Filename part of a path:  "C:\foo\bar.txt"  ->  "bar.txt"
		static string GetFileName(const char* input);

		// Gets the Filename part of a path (without the extension):  "C:\foo\bar.txt"  ->  "bar"
		static string GetFileNameWithoutExtension(const char* input);

		// Gets the File Extension part of a path:  "C:\foo\bar.txt"  ->  "txt"
		static string GetExtension(const char* input);

		// Changes the File Extension part of the path:  "C:\foo\bar.txt"  ->  "C:\foo\bar.newExt".   If no prior extension exists, the new one is appended.  The prepended period in the extension is not necessary.
		static string ChangeExtension(const char* input, const char* extension);

		// Gets the Path Root part of a path:  "C:\foo\bar.txt"  ->  "C:\"
		static string GetPathRoot(const char* input);

		// Gets a common GDK system path
		static string GetCommonPath(CommonPaths::Enum path);

		// Public static Properties
		// ----------------------------

		static const char PathSeperator;
		static const int MaxPathLength;
	};

} // namespace Gdk
