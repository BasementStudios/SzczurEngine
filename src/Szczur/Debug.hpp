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
#define IF_EDITOR if constexpr(false)

#else

#include <any>
#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <streambuf>
#include <string>
#include <string_view>

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

inline std::map<std::string, std::any> vars;

template <typename T, typename K, typename... Ts>
void createVar(K&& name, Ts&&... args)
{
	vars[std::forward<K>(name)].DEPENDENT_TEMPLATE_SCOPE emplace<T>(std::forward<Ts>(args)...);
}

template <typename T, typename K>
void setVar(K&& name, T&& arg)
{
	vars[std::forward<K>(name)] = std::forward<T>(arg);
}

template <typename T, typename K>
decltype(auto) getVar(K&& name)
{
	return std::any_cast<T&>(vars[std::forward<K>(name)]);
}

#include "NotoMono.ttf.bin"

}

}

#define INIT_LOGGER() rat::detail::DebugLogger ratDebugLogger; rat::detail::logger = &ratDebugLogger
#define LOG_INFO(...) { rat::detail::logger->log(__FILE__, __LINE__, "[INFO] ", __VA_ARGS__); }
#define LOG_WARN(...) { rat::detail::logger->log(__FILE__, __LINE__, "[WARN] ", __VA_ARGS__); }
#define LOG_ERROR(...) { rat::detail::logger->log(__FILE__, __LINE__, "[ERROR] ", __VA_ARGS__); }
#define LOG_INFO_IF(condition, ...) { if (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF(condition, ...) { if (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF(condition, ...) { if (condition) LOG_ERROR(__VA_ARGS__) }
#define LOG_INFO_IF_CX(condition, ...) { if constexpr (condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF_CX(condition, ...) { if constexpr (condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF_CX(condition, ...) { if constexpr (condition) LOG_ERROR(__VA_ARGS__) }
#define IF_EDITOR if constexpr(true)

#endif
