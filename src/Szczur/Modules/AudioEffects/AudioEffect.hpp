#pragma once

#include "Effects/Equalizer.hpp"
#include "Effects/Echo.hpp"
#include "Effects/Reverb.hpp"
#include <tuple>

#define MAX_AUX_FOR_SOURCE 2

namespace rat
{
    class AudioEffect
    {
        std::array<bool, MAX_AUX_FOR_SOURCE> _freeSlots { false }; 

        std::tuple<Equalizer, Echo, Reverb> _effects;

    public:
    
        /**********************/
        /****Effects data******/
        float LC = 200.0;
        float LG = 1.0;
        float HC = 6000.0;
        float HG = 1.0;
     	float M1C = 500.0;
        float M1W = 1.0;
        float M1G = 1.0;
        float M2C = 3000.0;
        float M2W = 1.0;
        float M2G = 1.0;
        float DL = 0.1;
        float LDL = 0.1;
        float DG = 0.5;
        float FB = 0.5;
        float SP = -1.0;
        float DEN = 1.0;
        float DIFF = 1.0;
        float G = 0.32;
        float RGH = 0.89;
        float DT = 1.49;
        float DHF = 0.83;
        float RG = 0.05;
        float RD = 0.007;
        float LRG = 1.26;
        float LRD = 0.011;
        float AAGH = 0.994;
        float RRF = 0.0;
        int DHL = 1;
        /**********************/ 

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