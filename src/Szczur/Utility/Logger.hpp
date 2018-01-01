#pragma once

/** @file Logger.hpp
 ** @description Header file with Logger class, its global object declaration and macros.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <ctime>		// time, strftime, localtime
#include <fstream>		// ofstream
#include <iostream>		// ostream, cout
#include <exception>	// exception

#include <boost/iostreams/stream.hpp>	// stream
#include <boost/iostreams/tee.hpp>		// tee_device
#include <boost/exception/diagnostic_information.hpp>	// current_exception_diagnostic_information

namespace rat
{

/** @class Logger
 ** @description Used for logging system. 
 **/
class Logger
{
	/* Types */
public:
	enum class Level {
		Every,
		Debug,
		Information,	Info = Information,		Default = Information,
		Warning, 		Warn = Warning,
		Error,			Err = Error,
		Exception,		Excpt = Exception,
		Nothing
	};

	using streamtee_t = boost::iostreams::tee_device<std::ofstream, std::ostream>;
	using logstream_t = boost::iostreams::stream<streamtee_t>;



	/* Variables */
private:
	// Buffer for _timestamp
	char _buffer[64];
	// Log file stream
	std::ofstream _file;
	// Tee device
	streamtee_t _tee;
public:
	// Logging Stream
	logstream_t stream;
	// Level of logging
	Level level = Level::Default;



	/* Operators */
public:
	Logger(const std::string& file, std::ostream& con = std::clog, const char* message = "", Level level = Level::Default) :
		_file(file),
		_tee(_file, con),
		stream(_tee),
		level(level)
	{
		stream << message;
	}
	
	Logger(Level level = Level::Default) :
		Logger(std::to_string(std::time(nullptr)) + ".log", std::clog, _timestamp("Starting @ %T %F \n"), level)
	{}

	// Disable copy and move constructors and operators
	Logger(const Logger&) = delete;
	Logger& operator = (const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator = (Logger&&) = delete;

	
	
	/* Methods */
public:
	template<typename... TArgs>
	Logger& log(Level level, const char* file, int line, TArgs&&... args);

	template<typename... TArgs>
	Logger& log(const char* file, int line, TArgs&&... args);



	/* Functions */
private:
	/// Helper function to place formated current timestamp. 
	const char* _timestamp(const char* format);
	/// Gets string for logging level. 
	const char* _levelString(Level level);

};

}

#include "Logger.tpp"



namespace rat {

// Global logger instance (defined in an implementation file) 
extern Logger logger; 

}



// Macros for logger
#ifndef NO_LOGGER

#define LOG(...)       { rat::logger.log(__FILE__, __LINE__, __VA_ARGS__); }
#define LOG_DEBUG(...) { rat::logger.log(rat::Logger::Level::Debug,       __FILE__, __LINE__, __VA_ARGS__); }
#define LOG_INFO(...)  { rat::logger.log(rat::Logger::Level::Information, __FILE__, __LINE__, __VA_ARGS__); }
#define LOG_WARN(...)  { rat::logger.log(rat::Logger::Level::Warning,     __FILE__, __LINE__, __VA_ARGS__); }
#define LOG_ERROR(...) { rat::logger.log(rat::Logger::Level::Error,       __FILE__, __LINE__, __VA_ARGS__); }
#define LOG_EXCPT(...) { rat::logger.log(rat::Logger::Level::Exception,   __FILE__, __LINE__, __VA_ARGS__); }
#define LOG_EXCEPTION(...) LOG_EXCPT(__VA_ARGS__)

#define LOG_IF(cond, ...)       { if(cond) LOG(__VA_ARGS__); }
#define LOG_DEBUG_IF(cond, ...) { if(cond) LOG_DEBUG(__VA_ARGS__); }
#define LOG_INFO_IF(cond, ...)  { if(cond) LOG_INFO(__VA_ARGS__); }
#define LOG_WARN_IF(cond, ...)  { if(cond) LOG_WARN(__VA_ARGS__); }
#define LOG_ERROR_IF(cond, ...) { if(cond) LOG_DEBUG(__VA_ARGS__); }
#define LOG_EXCPT_IF(cond, ...) { if(cond) LOG_EXCPT(__VA_ARGS__); }
#define LOG_EXCEPTION_IF(...) LOG_EXCPT_IF(__VA_ARGS__)

#else

#define LOG(...) 
#define LOG_DEBUG(...) 
#define LOG_INFO(...) 
#define LOG_WARN(...) 
#define LOG_ERROR(...) 
#define LOG_EXCPT(...) 
#define LOG_EXCEPTION(...) 

#define LOG_IF(cond, ...) 
#define LOG_DEBUG_IF(cond, ...) 
#define LOG_INFO_IF(cond, ...) 
#define LOG_WARN_IF(cond, ...) 
#define LOG_ERROR_IF(cond, ...) 
#define LOG_EXCPT_IF(cond, ...) 
#define LOG_EXCEPTION_IF(...) 

#endif
