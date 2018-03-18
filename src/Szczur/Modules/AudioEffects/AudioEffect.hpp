#pragma once

#include "Effects/Equalizer.hpp"
#include "Effects/Echo.hpp"
#include "Effects/Reverb.hpp"
#include <tuple>

namespace rat
{
    class AudioEffect
    {
        std::tuple<Equalizer, Echo, Reverb> _effects;

    public:

        AudioEffect(unsigned int& source)
            : _effects(source, source, source)
        {}

        template <typename T>
        T& getEffect()
        {
            return std::get<T>(_effects);
        }

        template <typename T>
        void setEffect(unsigned int aux)
        {
            std::get<T>(_effects).setAuxiliaryEffect(aux);
        }
        
        template <typename T>
        void cleanEffect()
        {
            std::get<T>(_effects).cleanEffect();
        }

        void cleanAllEffects()
        {
            cleanEffect<Equalizer>();
            cleanEffect<Reverb>();
            cleanEffect<Echo>();
        }

    };
}       