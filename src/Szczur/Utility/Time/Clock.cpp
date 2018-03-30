#include "Clock.hpp"

namespace rat
{

Clock::TimePoint_t Clock::now()
{
	return Clock_t::now();
}

Clock::Clock()
	: _refPoint{ now() }
{

}

Time Clock::getElapsedTime() const
{
	return now() - _refPoint;
}

Time Clock::restart()
{
	auto ret = getElapsedTime();
	_refPoint = now();
	return ret;
}

bool Clock::hasPassed(const Duration_t& duration) const
{
	return duration >= getElapsedTime().getDuration();
}

bool Clock::checkStep(const Duration_t& duration)
{
	if(hasPassed(duration)) {
		restart();
		return true;
	}
	return false;
}

}
