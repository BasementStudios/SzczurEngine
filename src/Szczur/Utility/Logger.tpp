/** @file Logger.cpp
 ** @description Templates file with Logger class, its global object declaration and macros.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

namespace rat
{

template<typename... TArgs>
Logger& Logger::log(Level level, const char* file, int line, TArgs&&... args)
{
    if (this->level > level) return *this;
    stream << '[' << _timestamp("%T ") << _levelString(level) << '@' << file << ':' << line << "] ";
    (stream << ... << std::forward<TArgs>(args));
    stream << '\n' << std::flush;
    return *this;
}

template<typename... TArgs>
Logger& Logger::log(const char* file, int line, TArgs&&... args)
{
    return log(Level::Default, file, line, args...);
}

}
