#pragma once

#ifndef NDEBUG

#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string_view>

namespace rat
{

class DebugLogger
{
public:

    template <typename... Ts>
    void log(const char* file, int line, Ts&&... args)
    {
        _formatTime("%H:%M:%S");

        std::string_view view = file;
        view = view.substr(view.find_last_of('/') + 1);

        _logFile << '[' << _buffer << ' ' << view << ' ' << line << ']' << ' '; (_logFile << ... << std::forward<Ts>(args)); _logFile << '\n';
        std::cerr << '[' << _buffer << ' ' << view << ' ' << line << ']' << ' '; (std::cerr << ... << std::forward<Ts>(args)); std::cerr << '\n';
    }

private:

    void _formatTime(const char* format)
    {
        std::time_t tm = std::time(nullptr);

        std::strftime(_buffer, sizeof(_buffer), format, std::localtime(&tm));
    }

    char _buffer[64];
    std::ofstream _logFile = std::ofstream(std::to_string(std::time(nullptr)) + ".log");

} inline * logger;

}

#define LOG(...) { rat::logger->log(__FILE__, __LINE__, __VA_ARGS__); }
#define LOG_IF(condition, ...) { if(condition) { LOG(__VA_ARGS__) } }
#define LOG_IF_CX(condition, ...) { if constexpr(condition) { LOG(__VA_ARGS__) } }

#else

#define LOG(...)
#define LOG_IF(...)
#define LOG_IF_CX(...)

#endif
