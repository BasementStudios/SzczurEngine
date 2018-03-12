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

    protected:

        MusicEffect(unsigned int& source, int effectType);

        void init();

        void setEffect(int type, float value);

        float checkRange(float value, float min, float max) const;

    };
}