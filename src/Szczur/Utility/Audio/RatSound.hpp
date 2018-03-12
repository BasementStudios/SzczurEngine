#pragma once

#include "SFML/Audio/Sound.hpp"

#include "AudioEffects.hpp"

namespace rat
{
    class RatSound : public sf::Sound, public AudioEffects
    {

    public:

        RatSound()
            : AudioEffects(m_source) 
        {}

        

    };
}