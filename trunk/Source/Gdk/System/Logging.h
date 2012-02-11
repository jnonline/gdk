//
/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


// Logging:
// This file contains implemenation for the GDK logging system.  Useable in GDK components & games
//
// Configuration:
//		GdkLog::Enabled = true								- Enable / Disable logging
//		GdkLog::MaxLevel = GdkLogLevel::Error				- Sets the highest log level that will be written to the log file; 
// 
// Usage:
//		LOG_INFO( "Error messase", ... );
//		LOG_ERROR( "Error messase", ... );
//
//
// Format:
//   [date : time][level] - message;
//		[File: c:\test\test.cpp][Line: 10]		(For errors & warnings)
//

#pragma once



// Comment this line to disable logging
#if defined(DEBUG) || defined(_DEBUG)
	#define GDK_LOGGING
#endif

namespace Gdk
{
    /// @addtogroup System
    /// @{

	// =================================================================================
    ///	@brief
    ///     Enumeration of logging levels
    // =================================================================================
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

        // *****************************************************************
        /// @brief
        ///     Gets a display-friendly string from the given enum value
        // *****************************************************************
		const char* ToString(LogLevel::Enum logLevel);
	}

	// =================================================================================
    ///	@brief
    ///     Provides logging functionality to the GDK platform and application.
    /// @remarks
    ///     For simplified logging, use the logging macros:
    ///     LOG_SYSTEM, LOG_INFO, LOG_VERBOSE, LOG_WARN, LOG_ERROR
    // =================================================================================
    class Log
	{
	public:
        
        // Public Properties
		// =====================================================
        
		// ---------------------------------
        /// @name Global Settings
        /// @{
        
        /// Enable/Disable the GDK logging system
		static bool Enabled;
        
        /// Sets the maximum logging level that will be written to the log.
		static LogLevel::Enum MaxLevel;
        
        /// @}
        
        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{

		static void Write(LogLevel::Enum logLevel, const char* file, int line, const char *format, ...);

        /// @}t        
	private:
        
        // Private Properties
		// =====================================================
        
        static string logFilePath;
		static char* msgBuffer;	

		// Private Methods
		// =====================================================

        static void Init();
		static void Shutdown();

        friend class Application;
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

    // *****************************************************************
    /// @def LOG_SYSTEM(format, ...)
    /// @brief 
    ///     Writes a System-level log message.
    /// @param format
    ///     A printf style format string to be written to the log.
    /// @param ...
    ///     Insertion values for the format string
    // *****************************************************************
    /// @def LOG_INFO(format, ...)
    /// @brief 
    ///     Writes a Info-level log message.
    /// @param format
    ///     A printf style format string to be written to the log.
    /// @param ...
    ///     Insertion values for the format string
    // *****************************************************************
    /// @def LOG_VERBOSE(format, ...)
    /// @brief 
    ///     Writes a Verbose-level log message.
    /// @param format
    ///     A printf style format string to be written to the log.
    /// @param ...
    ///     Insertion values for the format string
    // *****************************************************************
    /// @def LOG_WARN(format, ...)
    /// @brief 
    ///     Writes a Warning-level log message.
    /// @param format
    ///     A printf style format string to be written to the log.
    /// @param ...
    ///     Insertion values for the format string
    // *****************************************************************
    /// @def LOG_ERROR(format, ...)
    /// @brief 
    ///     Writes a Error-level log message.
    /// @param format
    ///     A printf style format string to be written to the log.
    /// @param ...
    ///     Insertion values for the format string
    // *****************************************************************

    /// @}
    
} // namespace Gdk
