#pragma once

#include "Time.hpp"

namespace rat
{

class Clock
{
public:

	using Clock_t     = Time::Clock_t;
	using Duration_t  = Time::Duration_t;
	using TimePoint_t = Time::TimePoint_t;
	using Period_t    = Time::Period_t;

	static TimePoint_t now();

	Clock();

	Clock(const Clock&) = default;
	Clock& operator = (const Clock&) = default;

	Clock(Clock&&) = default;
	Clock& operator = (Clock&&) = default;

	Time getElapsedTime() const;

	Time restart();

	bool hasPassed(const Duration_t& duration) const;

	bool checkStep(const Duration_t& duration);

private:

	TimePoint_t _refPoint;

};

}
