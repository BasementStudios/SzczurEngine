#include "Logger.hpp"

#include <cstdint>
#include <ctime>
#include <iomanip>

namespace rat
{

Logger::Logger()
{
	logger = this;

	std::snprintf(_logPath, sizeof(_logPath), "Logs/%lld.log", static_cast<long long>(std::time(nullptr)));

	LOG_INFO("Start of the application");
}

Logger::~Logger()
{
	LOG_INFO("End of the application");
}

void Logger::logException(const char* file, int line, const std::exception& exception, int level)
{
	log(file, line, "EXCEPTION", level, ' ', exception.what());

	try {
		std::rethrow_if_nested(exception);
	}
	catch (const std::exception& e) {
		logException(file, line, e, level + 1);
	}
	catch (...) {
		log(file, line, "EXCEPTION", level, ' ', "Unknown exception");
	}
}

void Logger::_formatTime(const char* format)
{
	const std::time_t tm = std::time(nullptr);

	std::strftime(_timeBuffer, sizeof(_timeBuffer), format, std::localtime(&tm));
}

}
