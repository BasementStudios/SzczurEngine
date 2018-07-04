#pragma once

#include <SFML/Audio/Sound.hpp>

#include "Szczur/Modules/AudioEffects/AudioEffect.hpp"

namespace rat
{
    class RatSound : public sf::Sound, public AudioEffect
    {

    public:

        RatSound()
            : AudioEffect(m_source) 
        {
            setRelativeToListener(true);
            setAttenuation(0);
        }

    };
}