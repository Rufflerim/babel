#pragma once
#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <sstream>
#include <array>
#include "Defines.h"

#define GAME_LOG_FILE "game.log"

using std::array;

enum class LogLevel {
    Fatal = 0,
    Error = 1,
    Warning,
    Info,
    Debug,
    Trace
};

namespace engine {

	/// <summary>
	/// Usage : LOG(LogLevel::Info) << "Message";
	/// </summary>
	class Log
	{
	public:
		GAPI Log();
		GAPI ~Log();
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		/// <summary>
		/// Create the logging text
		/// </summary>
		/// <param name="level">LogLevel with which to log</param>
		/// <returns>Base log text</returns>
		GAPI std::ostringstream& get(LogLevel level = LogLevel::Info);

		/// <summary>
		/// Restart log if config tells so
		/// </summary>
		GAPI static void restart();

	private:
		std::ostringstream os;
		static std::ofstream file;
		LogLevel logLevel { LogLevel::Trace };

		/// <summary>
		/// Takes a LogLevel and returns the corresponding string
		/// </summary>
		/// <param name="level">LogLevel to translate to a string</param>
		/// <returns>String corresponding to the LogLevel</returns>
		static str getLabel(LogLevel level);

		/// <summary>
		/// PLATFORM DEPENDANT
		/// Write into the console with colors
		/// </summary>
		/// <param name="message">Message to write</param>
		/// <param name="level">LogLevel that will be changed in a color</param>
		static void consoleWrite(const str& message, LogLevel level);

		/// <summary>
		/// PLATFORM DEPENDANT
		/// Get an iso date on 19 chars
		/// </summary>
		/// <returns>Iso date</returns>
		static array<char, 19> getDate();
	};

	// Log only if we pass a certain LogLevel
#ifdef GRELEASE
	constexpr i32 MAX_LOG_LEVEL = static_cast<i32>(LogLevel::Error);
#else
	constexpr i32 MAX_LOG_LEVEL = static_cast<i32>(LogLevel::Trace);
#endif

}

#define LOG(level)											\
	if (static_cast<i32>(level) > engine::MAX_LOG_LEVEL)	\
	;														\
	else													\
		engine::Log().get(level)
#endif
