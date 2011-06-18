#pragma once

#include "GdkLIB.h"
#include <string>

using namespace std;

// Note on strings:
//
//  When using sprintf or swprintf (or some variation)
//  DO NOT use %s or %S (they are not cross-platform friendly).  
//      instead use [%ls] (wide) or [%hs] (single-byte)
//            also, [%lc] (wide) or [%hc] (single-byte) for characters

namespace Gdk
{
	// ============================================================================

	class StringUtilities
	{
	public:

		// Common string literals
		static const char* Empty;
		static const wchar_t* WEmpty;
		static const char* Unknown;
		static const wchar_t* WUnknown;

		// Common String functions
		// -------------------------------

		static string Replace(const char* input, const char* searchText, const char* replacementText);
		static wstring Replace(const wchar_t* input, const wchar_t* searchText, const wchar_t* replacementText);

		// UTF8 to UTF16 conversions
		// -------------------------------

		static wstring UTF8ToUTF16(const char* source);
		static string UTF16ToUTF8(const wchar_t* source);

		// Hash Encoding
		// -------------------------------

		static unsigned int FastHash(const UInt8* data, int len);
		static unsigned int FastHash(const char* str);
		static unsigned int FastHash(const wchar_t* str);

	};

} // namespace Gdk
