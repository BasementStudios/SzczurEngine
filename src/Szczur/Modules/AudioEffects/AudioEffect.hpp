#pragma once

#include "Effects/Equalizer.hpp"
#include "Effects/Echo.hpp"
#include "Effects/Reverb.hpp"

#include "EffectsData.hpp"

#include <tuple>

#define MAX_AUX_FOR_SOURCE 2

namespace rat
{
    class AudioEffect
    {
        std::array<bool, MAX_AUX_FOR_SOURCE> _freeSlots { false }; 

        std::tuple<Equalizer, Echo, Reverb> _effects;

    public:

        EqualizerData eqData;
        ReverbData    reverbData;
        EchoData      echoData;

        enum class EffectType {
            None, Reverb, Echo, Equalizer
        };

        std::array<EffectType, MAX_AUX_FOR_SOURCE> effectsTypes {EffectType::None};

        AudioEffect(unsigned int& source);

        template <typename T>
        T& getEffect();

        template <typename T>
        void setEffect(unsigned int aux);
        
        template <typename T>
        void cleanEffect();

        void cleanAllEffects();

        int lastFreeSlot();

    protected:

        template <typename T>
        T& get();

    private:

        unsigned int getLastFreeSlot();

    };
}     

#include "AudioEffect.tpp"  