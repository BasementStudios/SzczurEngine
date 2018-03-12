#pragma once

#include "SFML/Audio/Music.hpp"

#include "AudioEffects.hpp"

namespace rat
{
	class RatMusic : public sf::Music, public AudioEffects
	{

	public:

		RatMusic()
			: AudioEffects(m_source)
		{}



	};
}