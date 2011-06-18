//
// Logging:
// This file contains implemenation for the GDK logging system.  Useable in GDK components & games
//
// Configuration:
//		GdkLog::Enabled = true								- Enable / Disable logging
//		GdkLog::MaxLevel = GdkLogLevel::Error				- Sets the highest log level that will be written to the log file; 
// 
// Usage:
//		LOG_INFO( L"Error messase", ... );
//		LOG_ERROR( L"Error messase", ... );
//
//
// Format:
//   [date : time][level] - message;
//		[File: c:\test\test.cpp][Line: 10]		(For errors & warnings)
//
// Notes:
//   Strings:   swprintf() interprets %s & %S differently between platforms (windows is non ansi)
//   So, To be perfectly safe when logging (which uses ANSI-C wide strings) use these format specifiers:
//      %ls  - Wide-character string
//      %hs  - Single-byte character string

#pragma once

#include "GdkLIB.h"

// Comment this line to disable logging
#if defined(DEBUG) || defined(_DEBUG)
	#define GDK_LOGGING
#endif

namespace Gdk
{

	// ============================================================================

	namespace LogLevel
	{
		enum Enum
		{
			System = 0,
			Assert,
			Error,
			Warning,
			Info,
			Verbose
		};

		// Methods
		const char* ToString(LogLevel::Enum logLevel);
	}

	// ============================================================================

	class Log
	{
	private:
		// Private Hidden CTor - static class only
		Log();

	public:
		// Global configuration
		// ----------------------------
		static bool Enabled;
		static LogLevel::Enum MaxLevel;

		// Logging Methods
		// ----------------------------

		static void Write(LogLevel::Enum logLevel, const char* file, int line, const wchar_t *format, ...);

	private:
		// ----------------------------
		// Init / Shutdown control
		friend class Application;
		static void Init();
		static void Shutdown();

		// Static buffers for logging
		static string logFilePath;
		static wchar_t* msgBuffer;
	};

	// ============================================================================
	// MACROS

	#ifdef GDK_LOGGING
		#ifdef GDKPLATFORM_WINDOWS
			#define LOG_SYSTEM(format, ...)	 Gdk::Log::Write(Gdk::LogLevel::System,  NULL, 0, format, __VA_ARGS__)			
			#define LOG_INFO(format, ...)	 Gdk::Log::Write(Gdk::LogLevel::Info,    NULL, 0, format, __VA_ARGS__)
			#define LOG_VERBOSE(format, ...) Gdk::Log::Write(Gdk::LogLevel::Verbose, NULL, 0, format, __VA_ARGS__)
			#define LOG_WARN(format, ...)	 Gdk::Log::Write(Gdk::LogLevel::Warning, __FILE__, __LINE__, format, __VA_ARGS__)			
			#define LOG_ERROR(format, ...)	 Gdk::Log::Write(Gdk::LogLevel::Error,   __FILE__, __LINE__, format, __VA_ARGS__)
		#else
			#define LOG_SYSTEM(format, args...)  Gdk::Log::Write(Gdk::LogLevel::System,  NULL, 0, format, ##args)			
			#define LOG_INFO(format, args...)    Gdk::Log::Write(Gdk::LogLevel::Info,    NULL, 0, format, ##args)
			#define LOG_VERBOSE(format, args...) Gdk::Log::Write(Gdk::LogLevel::Verbose, NULL, 0, format, ##args)
			#define LOG_WARN(format, args...)	 Gdk::Log::Write(Gdk::LogLevel::Warning, __FILE__, __LINE__, format, ##args)			
			#define LOG_ERROR(format, args...)   Gdk::Log::Write(Gdk::LogLevel::Error,   __FILE__, __LINE__, format, ##args)
		#endif
	#else
		#define LOG_SYSTEM(format, ...)		
		#define LOG_INFO(format, ...)
		#define LOG_VERBOSE(format, ...)
		#define LOG_WARN(format, ...)		
		#define LOG_ERROR(format, ...)
	#endif

	// ============================================================================

} // namespace Gdk
