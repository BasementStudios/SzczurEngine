#pragma once

#include "Time.hpp"

namespace rat
{

class Timer
{
public:

	using Clock_t     = Time::Clock_t;
	using Duration_t  = Time::Duration_t;
	using TimePoint_t = Time::TimePoint_t;
	using Period_t    = Time::Period_t;

	static TimePoint_t now();

	Timer();

	Timer(const Timer&) = default;

	Timer& operator = (const Timer&) = default;

	Timer(Timer&&) = default;

	Timer& operator = (Timer&&) = default;

	Time getElapsedTime() const;

	void start();

	void pause();

	void toggle();

	Time restart();

	bool isPaused() const;

	bool hasPassed(const Duration_t& duration) const;

	bool checkStep(const Duration_t& duration);

private:

	TimePoint_t _refPoint;
	TimePoint_t _pausePoint;

};

}
