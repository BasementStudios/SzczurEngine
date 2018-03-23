#include "Logger.hpp"

#include <cstdint>
#include <ctime>
#include <iomanip>

namespace rat
{

Logger::Logger()
{
	_buffer[0] = '[';
	std::snprintf(_logPath, sizeof(_logPath), "Logs/%lld.log", static_cast<long long>(std::time(nullptr)));
}

Logger::~Logger()
{
	LOG_INFO("End of the application");
}

void Logger::_formatTime(const char* format)
{
	const std::time_t tm = std::time(nullptr);

	std::strftime(_buffer + 1, sizeof(_buffer) - 1, format, std::localtime(&tm));
}

}
