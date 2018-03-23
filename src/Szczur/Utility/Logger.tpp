namespace rat
{

template <typename... Us>
void Logger::log(const char* file, int line, const char* levelName, Us&&... args)
{
	_formatTime("%H:%M:%S");

	std::sprintf(_buffer + 9, "] [%s:%i] ", file, line);

	_logFile.open(_logPath, std::ios::app);
	_logFile << _buffer << levelName << ' ';
	if constexpr (sizeof...(Us)) (_logFile << ... << std::forward<Us>(args));
	_logFile << std::endl;
	_logFile.close();

	#ifdef DEBUG
	std::cerr << _buffer << levelName << ' ';
	if constexpr (sizeof...(Us)) (std::cerr << ... << std::forward<Us>(args));
	std::cerr << std::endl;
	#endif
}

inline void Logger::logException(const std::exception& exception, int level)
{
	log("EXCEPTION", level, exception.what());

	try {
		std::rethrow_if_nested(exception);
	}
	catch (const std::exception& e) {
		logException(e, level + 1);
	}
	catch (...) {
		log("EXCEPTION", level, "Unknown exception");
	}
}

}
