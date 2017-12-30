#include "Logger.hpp"

/** @file Logger.cpp
 ** @description Implmentaion file with Logger class, its global object declaration and macros.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <ctime>		// time, strftime, localtime
#include <fstream>		// ofstream
#include <iostream>		// ostream, cout
#include <exception>	// exception

#include <boost/iostreams/stream.hpp>	// stream
#include <boost/iostreams/tee.hpp>		// tee_device

namespace rat
{

// Global logger instance
Logger logger(Logger::Level::Debug);

const char* Logger::_timestamp(const char* format)
{
    std::time_t tm = std::time(nullptr);
    std::strftime(_buffer, sizeof(_buffer), format, std::localtime(&tm));
    return (const char*)_buffer;
}

const char* Logger::_levelString(Level level)
{
    switch(level) {
#ifdef COLORS
        case Level::Debug: 			return "\033[34m.\033[0m][\033[37mDebug\033[0m";
        case Level::Information: 	return "\033[32m+\033[0m][\033[37mInfor\033[0m";
        case Level::Warning: 		return "\033[33m-\033[0m][\033[37mWarni\033[0m";
        case Level::Error: 			return "\033[31m!\033[0m][\033[37mError\033[0m";
        case Level::Exception:      return "\033[35m?\033[0m][\033[37mExcpt\033[0m";
#else
        case Level::Debug: 			return ".][Debug";
        case Level::Information: 	return "+][Infor";
        case Level::Warning: 		return "-][Warni";
        case Level::Error: 			return "!][Error";
        case Level::Exception:      return "?][Excpt";
#endif
        default: 					return "";
    }
}

}
