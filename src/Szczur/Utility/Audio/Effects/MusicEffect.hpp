#pragma once

#define AL_ALEXT_PROTOTYPES 
#include <AL/efx.h>

namespace rat
{
    class MusicEffect
    {

    private:

        unsigned int& _source;

        unsigned int _effect;
        unsigned int _effectSlot;

        int _effectType;

        bool _created = false;
        bool _supported = false;

        int _sourceSlot;

        inline static int _lastFreeSlot = 0; 

    public:

        void cleanEffect();

    protected:

        MusicEffect(unsigned int& source, int effectType);

        void init();

        void setEffect(int type, float value);


    };
}