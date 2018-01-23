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

    static TimePoint_t now()
    {
        return Clock_t::now();
    }

    Clock() :
        _refPoint(now())
    {

    }

    Clock(const Clock&) = default;

    Clock& operator = (const Clock&) = default;

    Clock(Clock&&) = default;

    Clock& operator = (Clock&&) = default;

    Time getElapsedTime() const
    {
        return now() - _refPoint;
    }

    Time restart()
    {
        Time ret = getElapsedTime();
        _refPoint = now();
        return ret;
    }

    bool hasPassed(const Duration_t& duration) const
    {
        return duration >= getElapsedTime().getDuration();
    }

    bool checkStep(const Duration_t& duration)
    {
        if(hasPassed(duration)) {
            restart();
            return true;
        }
        return false;
    }

private:

    TimePoint_t _refPoint;

};

}
