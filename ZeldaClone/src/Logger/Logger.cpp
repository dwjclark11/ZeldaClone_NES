#include "Logger/Logger.h"
#include <ctime>
#include <chrono>
#pragma warning( disable : 4996 ) // This is put in place because the time functions have been depricated

#ifndef NDEBUG
bool Logger::debugMode = true;
#elif NDEBUG
bool Logger::debugMode = false;
#endif

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString()
{
	time_t currentTime = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
	std::string output( 30, '\0' );
	std::strftime( &output[ 0 ], output.size(), "%d-%b-%y %H:%M:%S", std::localtime( &currentTime ) );
	return output;
}

void Logger::Log( const std::string& message )
{
	if ( Logger::debugMode )
	{
		LogEntry logEntry;
		logEntry.type = LOG_INFO;
		logEntry.message = "LOG: [" + CurrentDateTimeToString() + "] -- " + message;

		std::cout << "\x1B[32m" << logEntry.message << "\033[0m\n"; // Set the colour to green
		messages.push_back( logEntry );
	}
}

void Logger::Err( const std::string& message )
{
	if ( Logger::debugMode )
	{
		LogEntry logEntry;
		logEntry.type = LOG_ERROR;
		logEntry.message = "Err: [" + CurrentDateTimeToString() + "] -- " + message;

		std::cout << "\x1B[91m" << logEntry.message << "\033[0m\n"; // Set the colour to red
		messages.push_back( logEntry );
	}
}
