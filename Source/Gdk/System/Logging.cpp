/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"

#include "Logging.h"
#include "../IO/Path.h"
#include "Threading/CriticalSection.h"

using namespace Gdk;

// Static properties
bool Log::Enabled = true;
LogLevel::Enum Log::MaxLevel = LogLevel::Verbose;
string Log::logFilePath;
char* Log::msgBuffer = NULL;

// *****************************************************************
const char* LogLevel::ToString(LogLevel::Enum logLevel)
{
	static const char* logLevelNames[] = 
	{
		"System ",
		"ASSERT ",
		"ERROR  ",
		"Warning",
		"Info   ",
		"Verbose"
	};
	return logLevelNames[logLevel];
}

// *****************************************************************
/// @brief 
///     Writes a log message to the GDK log
/// @remarks 
///     For simplicity, use the LOG_ macros.  
/// @param logLevel
///     The level of the log message
/// @param file
///     The name of the file generating the log message (usually __FILE__)
/// @param line
///     The line number in the code file of the method call (usually __LINE__)
/// @param format
///     A printf style format string to be written to the log.
/// @param ...
///     Insertion values for the format string
// *****************************************************************
void Log::Write(LogLevel::Enum logLevel, const char* file, int line, const char *format, ...)
{
	// Is logging disabled?
	if(Log::Enabled == false)
		return;
	
	// Are we ignoring this log level
	if((int)logLevel > (int)Log::MaxLevel)
		return;
	
	// Get the current time
	time_t currentTime;
	time( &currentTime );
	struct tm * timeinfo = NULL;
	
	#ifdef GDKPLATFORM_WINDOWS
		struct tm actualTimeInfo;
		timeinfo = &actualTimeInfo;
		localtime_s(timeinfo, &currentTime);
	#else
		timeinfo = localtime(&currentTime);
	#endif
	
	// Thread lock
	GDK_BEGIN_CRITICAL_SECTION(LogWriter)
	{
		// Add the current time & log level to the msg buffer
        int charsWritten = GDK_SPRINTF(
            msgBuffer, 19500, 
            "[%02d/%02d/%02d %02d:%02d:%02d][%s]:", 
            timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year % 100, 
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
            LogLevel::ToString(logLevel)
            );
        
        if(charsWritten < 0)
        {
            Gdk::DebugBreak();
        }
        
		char *offset = msgBuffer + charsWritten;
        
		// Add the custom message to the log  (using variable args)
		va_list args;
		va_start (args, format);
		int charsWritten2 = GDK_VSPRINTF(offset, 2048, format, args);
		va_end (args);
        offset += charsWritten2;

		// Is there a file & line number?
        int charsWritten3 = 0;
		if(file != NULL)
			charsWritten3 = GDK_SPRINTF(offset, 2048, "  [File: %s][Line: %d]\n", file, line);
		else // No file & line
			charsWritten3 = GDK_SPRINTF(offset, 2048, "\n");
        
        offset += charsWritten3;

		// For errors & warnings, put the message in the debug output
		if( logLevel <= LogLevel::Info )
		{
        #ifdef GDKPLATFORM_WINDOWS
			OutputDebugStringA(msgBuffer);
        #else
            printf(msgBuffer);
        #endif
		}

		// Open the log file
		FILE* logFile = NULL;
		#ifdef GDKPLATFORM_WINDOWS
			fopen_s(&logFile, logFilePath.c_str(), "a");
		#else
			logFile = fopen(logFilePath.c_str(), "a");
		#endif

		if(logFile == NULL)
			return;
		
		// Write the msg
		fprintf(logFile, msgBuffer);
		
		// Close the log
		fclose(logFile);

	}
	GDK_END_CRITICAL_SECTION(LogWriter)
}

// *****************************************************************
/// @brief 
///     Initializes the GDK logging system
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Log::Init()
{
    if(msgBuffer == NULL)
        msgBuffer = new char[20000];
    
	// Get the path to the folder for the logs
	string workingFolder = Path::GetCommonPath(CommonPaths::WorkingFolder);

	// Ensure the folder exists
	Directory::Create(workingFolder.c_str());

	// Build the full path to the log
	logFilePath = Path::Combine(workingFolder.c_str(), "Gdk.log");

	// Enable logging
	Log::Enabled = true;
	
	// Log the startup
	LOG_SYSTEM("-----------------------------------------------------------------------");
	LOG_SYSTEM("Initialized GDK Log");
}

// *****************************************************************
/// @brief 
///     Shuts down the GDK logging system
/// @note
///     GDK Internal Use Only
// *****************************************************************
void Log::Shutdown()
{
	// Log the shutdown
	LOG_SYSTEM("Shutdown GDK Log");
	LOG_SYSTEM("-----------------------------------------------------------------------");
    
    delete[] msgBuffer;
    msgBuffer = NULL;
	
}