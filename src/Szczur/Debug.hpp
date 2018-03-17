#pragma once

#if defined(NDEBUG)

#include <SFML/System/Err.hpp>

#define INIT_DEBUG() sf::err().rdbuf(nullptr)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_INFO_IF(...)
#define LOG_WARN_IF(...)
#define LOG_ERROR_IF(...)
#define LOG_INFO_IF_CX(...)
#define LOG_WARN_IF_CX(...)
#define LOG_ERROR_IF_CX(...)

#else

#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <streambuf>
#include <string>
#include <string_view>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "Szczur/Utility/Convert/Hash.hpp"
#include "Szczur/Utility/Convert/Unicode.hpp"

#include "Szczur/CompilerPortability.hpp"
#include "Szczur/GlobalVarsHolder.hpp"
#include "Szczur/ImGui.hpp"

namespace rat
{

class DebugLogger
{
public:

	DebugLogger() = default;

	~DebugLogger() = default;

	DebugLogger(const DebugLogger&) = delete;

	DebugLogger& operator = (const DebugLogger&) = delete;

	DebugLogger(DebugLogger&&) = delete;

	DebugLogger& operator = (DebugLogger&&) = delete;

	template <typename... Ts>
	void log(const char* file, int line, Ts&&... args)
	{
		_formatTime("%H:%M:%S");

		std::string_view view = file;
		view = view.substr(view.find_last_of(DIRECTORY_SEPARATOR_CHAR) + 1);

		_logFile.open(_logFilePath, std::ios::app);
		_logFile << '[' << _buffer << ']' << ' ' << '[' << view << ':' << line << ']' << ' '; (_logFile << ... << std::forward<Ts>(args)); _logFile << '\n' << std::flush;
		_logFile.close();

		std::cerr << '[' << _buffer << ']' << ' ' << '[' << view << ':' << line << ']' << ' '; (std::cerr << ... << std::forward<Ts>(args)); std::cerr << '\n' << std::flush;
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

namespace detail
{

#include "NotoMono.ttf.bin"

}

}

#define INIT_DEBUG() rat::DebugLogger ratDebugLogger; rat::logger = &ratDebugLogger; rat::GlobalVarsHolder ratGlobalVarsHolder; rat::gVar = &ratGlobalVarsHolder
#define LOG_INFO(...) { rat::logger->log(__FILE__, __LINE__, "[INFO] ", __VA_ARGS__); }
#define LOG_WARN(...) { rat::logger->log(__FILE__, __LINE__, "[WARN] ", __VA_ARGS__); }
#define LOG_ERROR(...) { rat::logger->log(__FILE__, __LINE__, "[ERROR] ", __VA_ARGS__); }
#define LOG_INFO_IF(condition, ...) { if (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF(condition, ...) { if (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF(condition, ...) { if (condition) LOG_ERROR(__VA_ARGS__) }
#define LOG_INFO_IF_CX(condition, ...) { if constexpr (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF_CX(condition, ...) { if constexpr (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF_CX(condition, ...) { if constexpr (condition) LOG_ERROR(__VA_ARGS__) }
#define EDITOR

#endif
