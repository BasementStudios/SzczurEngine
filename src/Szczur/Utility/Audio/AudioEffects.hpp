#pragma once

#include "Effects/Equalizer.hpp"
#include "Effects/Echo.hpp"
#include "Effects/Reverb.hpp"
#include <tuple>

namespace rat
{
    class AudioEffects
    {
        std::tuple<Equalizer, Echo, Reverb> _effects;

    public:

        AudioEffects(unsigned int& source)
            : _effects(source, source, source)
        {}

        template <typename T>
        T& getEffect()
        {
            return std::get<T>(_effects);
        }
        
    };
}