#pragma once

#include <fstream>
#ifdef DEBUG
#	include <iostream>
#endif

namespace rat
{

class Logger
{
public:

	Logger();

	Logger(const Logger&) = delete;
	Logger& operator = (const Logger&) = delete;

	Logger(Logger&&) = delete;
	Logger& operator = (Logger&&) = delete;

	~Logger();

	template <typename... Us>
	void log(const char* file, int line, const char* levelName, Us&&... args);

	void logException(const std::exception& exception, int level = 0);

private:

	void _formatTime(const char* format);

	char _buffer[64];
	char _logPath[128];
	std::ofstream _logFile;

};

inline Logger* logger = nullptr;

}

#include "Logger.tpp"

#define LOG_INFO(...) { rat::logger->log(__FILE__, __LINE__, "[INFO]", __VA_ARGS__); }
#define LOG_WARN(...) { rat::logger->log(__FILE__, __LINE__, "[WARN]", __VA_ARGS__); }
#define LOG_ERROR(...) { rat::logger->log(__FILE__, __LINE__, "[ERROR]", __VA_ARGS__); }
#define LOG_INFO_IF(condition, ...) { if (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF(condition, ...) { if (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF(condition, ...) { if (condition) LOG_ERROR(__VA_ARGS__) }
#define LOG_INFO_IF_CX(condition, ...) { if constexpr (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF_CX(condition, ...) { if constexpr (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF_CX(condition, ...) { if constexpr (condition) LOG_ERROR(__VA_ARGS__) }
#define LOG_EXCEPTION(exception) { rat::logger->logException(exception); }
