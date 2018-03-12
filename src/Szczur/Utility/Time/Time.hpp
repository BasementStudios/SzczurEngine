#pragma once

#include <chrono>

#include <SFML/System/Time.hpp>

namespace rat
{

class Time
{
public:

	using Clock_t     = std::chrono::steady_clock;
	using Duration_t  = Clock_t::duration;
	using TimePoint_t = Clock_t::time_point;
	using Period_t    = Clock_t::period::type;

	Time() = default;

	template <typename Rep, typename Period>
	Time(const std::chrono::duration<Rep, Period>& duration);

	Time(const Time&) = default;

	Time& operator = (const Time&) = default;

	Time(Time&&) = default;

	Time& operator = (Time&&) = default;

	Duration_t getDuration() const;

	std::chrono::nanoseconds::rep asNanoseconds() const;

	std::chrono::microseconds::rep asMicroseconds() const;

	std::chrono::milliseconds::rep asMilliseconds() const;

	std::chrono::seconds::rep asSeconds() const;

	std::chrono::duration<float>::rep asFSeconds() const;

	sf::Time asSfTime() const;

	bool operator == (const Time& time) const;

	bool operator != (const Time& time) const;

	bool operator < (const Time& time) const;

	bool operator <= (const Time& time) const;

	bool operator > (const Time& time) const;

	bool operator >= (const Time& time) const;

private:

	Duration_t _duration;

};

}

#include "Time.tpp"
