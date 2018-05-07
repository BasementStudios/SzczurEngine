#include "Timer.hpp"

namespace rat
{

Timer::TimePoint_t Timer::now()
{
	return Clock_t::now();
}

Timer::Timer()
	: _refPoint { now() }
	, _pausePoint { now() }
{

}

Time Timer::getElapsedTime() const
{
	return isPaused() ? _pausePoint - _refPoint : now() - _refPoint;
}

void Timer::start()
{
	_refPoint += now() - _pausePoint;
	_pausePoint = TimePoint_t{ Duration_t::zero() };
}

void Timer::pause()
{
	_pausePoint = now();
}

void Timer::toggle()
{
	if (isPaused())
	{
		start();
	}
	else
	{
		pause();
	}
}

Time Timer::restart()
{
	Time Ret = getElapsedTime();

	_pausePoint = TimePoint_t{ Duration_t::zero() };
	_refPoint = now();

	return Ret;
}

bool Timer::isPaused() const
{
	return _pausePoint.time_since_epoch() != Duration_t::zero();
}

bool Timer::hasPassed(const Duration_t& duration) const
{
	return duration >= getElapsedTime().getDuration();
}

bool Timer::checkStep(const Duration_t& duration)
{
	if (hasPassed(duration))
	{
		restart();

		return true;
	}

	return false;
}

}
