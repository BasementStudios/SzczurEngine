#pragma once

#include "SFML/Audio/Sound.hpp"

#include "Szczur/Modules/AudioEffects/AudioEffect.hpp"

#define SOUND_DATA_FILE_PATH "Assets/Sound/sounds.json"

namespace rat
{
    class RatSound : public sf::Sound, public AudioEffect
    {

    public:

        RatSound()
            : AudioEffect(m_source) 
        {
            
        }

    };
}