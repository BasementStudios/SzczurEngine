#include "Time.hpp"

namespace rat
{

Time::Duration_t Time::getDuration() const
{
	return _duration;
}

std::chrono::nanoseconds::rep Time::asNanoseconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(_duration).count();
}

std::chrono::microseconds::rep Time::asMicroseconds() const
{
	return std::chrono::duration_cast<std::chrono::microseconds>(_duration).count();
}

std::chrono::milliseconds::rep Time::asMilliseconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(_duration).count();
}

std::chrono::seconds::rep Time::asSeconds() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(_duration).count();
}

std::chrono::duration<float>::rep Time::asFSeconds() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(_duration).count();
}

bool Time::operator == (const Time& time) const
{
	return _duration == time._duration;
}

bool Time::operator != (const Time& time) const
{
	return !operator==(time);
}

bool Time::operator < (const Time& time) const
{
	return _duration < time._duration;
}

bool Time::operator <= (const Time& time) const
{
	return !operator>(time);
}

bool Time::operator > (const Time& time) const
{
	return _duration > time._duration;
}

bool Time::operator >= (const Time& time) const
{
	return !operator<(time);
}

}
