#pragma once

#ifndef NDEBUG

// C++
#include <ctime>
#include <fstream>

namespace rat
{

class DebugLogger
{
private:

	void _putTime(const char* format)
	{
		std::time_t tm = std::time(nullptr);

		std::strftime(_buffer, sizeof(_buffer), format, std::localtime(&tm));
	}

	char _buffer[64];
	std::ofstream _logFile;

public:

	DebugLogger() :
		_logFile(std::to_string(std::time(nullptr)) + ".log")
	{
		_putTime("%H:%M:%S - %d %B %Y\n");

		_logFile << _buffer;
	}

	DebugLogger(const DebugLogger&) = delete;

	DebugLogger& operator = (const DebugLogger&) = delete;

	DebugLogger(DebugLogger&&) = delete;

	DebugLogger& operator = (DebugLogger&&) = delete;

	template<typename... Ts>
	DebugLogger& log(const char* file, int line, Ts&&... args)
	{
		_putTime("%H:%M:%S ");

		_logFile << '\n' << '[' << _buffer << file << ' ' << line << ']' << ' ';

		(_logFile << ... << std::forward<Ts>(args));

		return *this;
	}

} inline logger;

}

#define rat_Log(...) { rat::logger.log(__FILE__, __LINE__, __VA_ARGS__); }
#define rat_LogIf(condition, ...) { if (condition) { rat_Log(__VA_ARGS__) } }
#define rat_LogIfCx(condition, ...) { if constexpr (condition) { rat_Log(__VA_ARGS__) } }

#else

#define rat_Log(...)
#define rat_LogIf(...)
#define rat_LogIfCx(...)

#endif
