#pragma once

#include "SFML/Audio/Music.hpp"

#include "Szczur/Modules/AudioEffects/AudioEffect.hpp"

namespace rat
{
	class RatMusic : public sf::Music, public AudioEffect
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