#include "RatMusic.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

	RatMusic::RatMusic()
		: AudioEffect(m_source)
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