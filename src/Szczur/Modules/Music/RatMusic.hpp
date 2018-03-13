#pragma once

#include "SFML/Audio/Music.hpp"

#include "Szczur/Utility/Audio/AudioEffects.hpp"

#include "Szczur/Debug.hpp"

namespace rat
{
	class RatMusic : public sf::Music, public AudioEffects
	{

	private:

		unsigned int _counter = 0;

	public:

		RatMusic();

		void incrementCounter();
		void decrementCounter();

		unsigned int getCounterValue() const;

	};
}