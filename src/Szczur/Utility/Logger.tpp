namespace rat
{

template <typename... Us>
void Logger::log(const char* file, int line, const char* levelName, Us&&... args)
{
	_formatTime("%H:%M:%S");

	std::sprintf(_buffer, "[%s] [%s] [%s:%i] ", _timeBuffer, levelName, file, line);

	_logFile.open(_logPath, std::ios::app);
	_logFile << _buffer;
	if constexpr (sizeof...(Us) != 0) (_logFile << ... << std::forward<Us>(args));
	_logFile << std::endl;
	_logFile.close();

	#ifdef DEBUG
	std::cerr << _buffer;
	if constexpr (sizeof...(Us) != 0) (std::cerr << ... << std::forward<Us>(args));
	std::cerr << std::endl;
	#endif
}

}
