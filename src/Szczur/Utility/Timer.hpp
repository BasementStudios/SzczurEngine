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

    static TimePoint_t now()
    {
        return Clock_t::now();
    }

    Timer() :
        _refPoint(now()), _pausePoint(now())
    {

    }

    Timer(const Timer&) = default;

    Timer& operator = (const Timer&) = default;

    Timer(Timer&&) = default;

    Timer& operator = (Timer&&) = default;

    Time getElapsedTime() const
    {
        return isPaused() ? _pausePoint - _refPoint : now() - _refPoint;
    }

    void start()
    {
        _refPoint += now() - _pausePoint;
        _pausePoint = TimePoint_t(Duration_t::zero());
    }

    void pause()
    {
        _pausePoint = now();
    }

    void toggle()
    {
        if(isPaused())
            start();
        else
            pause();
    }

    Time restart()
    {
        Time Ret = getElapsedTime();
        _pausePoint = TimePoint_t(Duration_t::zero());
        _refPoint = now();
        return Ret;
    }

    bool isPaused() const
    {
        return _pausePoint.time_since_epoch() != Duration_t::zero();
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
    TimePoint_t _pausePoint;

};

}
