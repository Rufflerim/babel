#include "pch.h"
#include "Log.h"
#include <iostream>

#ifdef GPLATFORM_WINDOWS
	#include <windows.h>
	#include <windowsx.h>
#endif // GPLATFORM_WINDOWS

using engine::Log;

std::ofstream Log::file;

Log::Log()
{
	file.open(GAME_LOG_FILE, std::fstream::app);
}

Log::~Log() 
{
	// Write both in file and in stream
	// when losing the log object scope
	os << std::endl;
	file << os.str();

#ifdef GPLATFORM_WINDOWS
	consoleWrite(os.str(), logLevel);
#else
	std::cout << os.str();
#endif // GPLATFORM_WINDOWS

	os.clear();
	file.close();
}

std::ostringstream& engine::Log::get(LogLevel level)
{
	// TODO Assert file exists

	logLevel = level;

	// Logging
	os << getDate().data() << " " << getLabel(level) << ": \t";
	return os;
}

void Log::restart()
{
	file.open(GAME_LOG_FILE, std::fstream::trunc);
	file.close();
}

str Log::getLabel(LogLevel level)
{
	str label;
	switch (level) {
	case LogLevel::Trace:
		label = "TRACE";
		break;
	case LogLevel::Debug:
		label = "DEBUG";
		break;
	case LogLevel::Info:
		label = "INFO";
		break;
	case LogLevel::Warning:
		label = "WARNING";
		break;
	case LogLevel::Error:
		label = "ERROR";
		break;
	case LogLevel::Fatal:
		label = "FATAL";
		break;
	}
	return label;
}

void Log::consoleWrite(const str& message, LogLevel level)
{
	// Colors for Fatal, Error, Warning, Info, Debug, Trace
	array<u8, 6> levels { 64, 4, 6, 2, 1, 8 };

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, levels[static_cast<i32>(level)]);
	OutputDebugStringA(message.c_str());
	LPDWORD numberWritten {};
	WriteConsoleA(consoleHandle, message.c_str(), static_cast<DWORD>(message.length()), numberWritten, 0);
}

array<char, 19> engine::Log::getDate()
{
#ifdef GPLATFORM_WINDOWS
	time_t now;
	array<char, 19> date {};
	struct tm timeInfo {};
	time(&now);
	localtime_s(&timeInfo, &now);
	strftime(date.data(), date.size(), "%y-%m-%d %H:%M:%S", &timeInfo);
	return date;
#else
	array<char, 19> date {};
	return date;
#endif // GPLATFORM_WINDOWS

}

