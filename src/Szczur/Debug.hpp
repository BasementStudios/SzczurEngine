#pragma once

#if defined(NDEBUG)

#define INIT_LOGGER()
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_INFO_IF(...)
#define LOG_WARN_IF(...)
#define LOG_ERROR_IF(...)
#define LOG_INFO_IF_CX(...)
#define LOG_WARN_IF_CX(...)
#define LOG_ERROR_IF_CX(...)
#define LOG_EXCEPTION(...)
#define IF_EDITOR if constexpr(false)

#else

#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string_view>
#include <exception>
#include <stdexcept>

#include "Szczur/CompilerPortability.hpp"
#include "Szczur/ImGui.hpp"

namespace rat
{

namespace detail
{

class DebugLogger
{
public:

	template <typename... Ts>
	void log(const char* filePath, int line, Ts&&... args)
	{
		_formatTime("%H:%M:%S");

		std::string_view filePathView = filePath;
		
		// Remove local path to make relative
		static const char* localPath = __FILE__;
		std::string_view localPathView = localPath; 
		filePathView = filePathView.substr(localPathView.length() - 9 - 1 - 6 - 1 - 3);

		_logFile.open(_logFilePath, std::ios::app);
		_logFile << '[' << _buffer << ']' << ' ' << '[' << filePathView << ':' << line << ']' << ' '; (_logFile << ... << std::forward<Ts>(args)); _logFile << '\n' << std::flush;
		_logFile.close();

		std::cerr << '[' << _buffer << ']' << ' ' << '[' << filePathView << ':' << line << ']' << ' '; (std::cerr << ... << std::forward<Ts>(args)); std::cerr << '\n' << std::flush;
	}

	void log_exception(const std::exception& e, unsigned int level =  0)
	{
		if (level == 0) {
			log("", 0, "[EXCEPTION]");
		}
		log("", 0, std::string(level * 2, ' '), e.what());
		try {
			std::rethrow_if_nested(e);
		} 
		catch(const std::exception& E) {
			log_exception(E, level + 1);
		} 
		catch(...) {}
	}


private:

	void _formatTime(const char* format)
	{
		std::time_t tm = std::time(nullptr);

		std::strftime(_buffer, sizeof(_buffer), format, std::localtime(&tm));
	}

	char _buffer[64];
	std::string _logFilePath = "Logs/" + std::to_string(std::time(nullptr)) + ".log";
	std::ofstream _logFile;

};

inline DebugLogger* logger = nullptr;

#include "NotoMono.ttf.bin"

}

}

#define INIT_LOGGER() rat::detail::DebugLogger ratDebugLogger; rat::detail::logger = &ratDebugLogger
#define LOG_INFO(...) { rat::detail::logger->log(__FILE__, __LINE__, "[INFO] ", __VA_ARGS__); }
#define LOG_WARN(...) { rat::detail::logger->log(__FILE__, __LINE__, "[WARN] ", __VA_ARGS__); }
#define LOG_ERROR(...) { rat::detail::logger->log(__FILE__, __LINE__, "[ERROR] ", __VA_ARGS__); }
#define LOG_EXCEPTION(e) { rat::detail::logger->log_exception(e); }
#define LOG_INFO_IF(condition, ...) { if (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF(condition, ...) { if (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF(condition, ...) { if (condition) LOG_ERROR(__VA_ARGS__) }
#define LOG_INFO_IF_CX(condition, ...) { if constexpr (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF_CX(condition, ...) { if constexpr (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF_CX(condition, ...) { if constexpr (condition) LOG_ERROR(__VA_ARGS__) }
#define IF_EDITOR if constexpr(true)

#endif
