#pragma once

#ifndef NDEBUG

#include <ctime>
#include <fstream>

namespace rat {
	class {
	private:

		std::ofstream _logFile = std::ofstream(std::to_string(std::time(nullptr)) + ".log");

	public:

		template<typename T>
		decltype(auto) operator << (T&& value) {
			_logFile << std::forward<T>(value);
			return (*this);
		}
	} inline logger;
}

#define rat_Log(code) { rat::logger << code; }
#define rat_LogIf(condition, code) { if (condition) { rat::logger << code; } }
#define rat_LogIfCx(condition, code) { if constexpr (condition) { rat::logger << code; } }

#else

#define rat_Log(...)
#define rat_LogIf(...)
#define rat_LogIfCx(...)

#endif
