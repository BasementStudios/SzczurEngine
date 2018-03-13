#include "RatMusic.hpp"

namespace rat
{

	RatMusic::RatMusic()
		: AudioEffects(m_source)
	{
		LOG_INFO("RatMusic created");
	}

	void RatMusic::incrementCounter()
	{
		++_counter;
	}

	void RatMusic::decrementCounter()
	{
		--_counter;
	}

	unsigned int RatMusic::getCounterValue() const
	{
		return _counter;
	}

}