#pragma once

#include "SFML/Audio/Sound.hpp"

#include "AudioEffect.hpp"

namespace rat
{
    class RatSound : public sf::Sound, public AudioEffect
    {

    public:

        RatSound()
            : AudioEffect(m_source) 
        {}

    };
}