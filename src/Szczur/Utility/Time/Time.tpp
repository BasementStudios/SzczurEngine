namespace rat
{

template <typename Rep, typename Period>
Time::Time(const std::chrono::duration<Rep, Period>& duration)
	: _duration{ std::chrono::duration_cast<Duration_t>(duration) }
{

}

}
