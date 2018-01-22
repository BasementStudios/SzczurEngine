#pragma once

#include <chrono>

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
    Time(const std::chrono::duration<Rep, Period>& duration) :
        _duration(std::chrono::duration_cast<Duration_t>(duration))
    {

    }

    Time(const Time&) = default;

    Time& operator = (const Time&) = default;

    Time(Time&&) = default;

    Time& operator = (Time&&) = default;

    Duration_t getDuration() const
    {
        return _duration;
    }

    std::chrono::nanoseconds::rep asNanoseconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_duration).count();
    }

    std::chrono::microseconds::rep asMicroseconds() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(_duration).count();
    }

    std::chrono::milliseconds::rep asMilliseconds() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_duration).count();
    }

    std::chrono::seconds::rep asSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(_duration).count();
    }

    std::chrono::duration<float>::rep asFSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::duration<float>>(_duration).count();
    }

    bool operator == (const Time& time) const
    {
        return _duration == time._duration;
    }

    bool operator != (const Time& time) const
    {
        return !operator==(time);
    }

    bool operator < (const Time& time) const
    {
        return _duration < time._duration;
    }

    bool operator <= (const Time& time) const
    {
        return !operator>(time);
    }

    bool operator > (const Time& time) const
    {
        return _duration > time._duration;
    }

    bool operator >= (const Time& time) const
    {
        return !operator<(time);
    }

private:

    Duration_t _duration;

};

}
