#pragma once

#define AL_ALEXT_PROTOTYPES 
#include <OpenAL/efx.h>

namespace rat
{
    class EffectBase
    {

    private:

        unsigned int& _source;

        unsigned int _effect;
        unsigned int _effectSlot;

        int _effectType;

        bool _created = false;
        bool _supported = false;

        unsigned int _sourceSlot;

    public:

        void cleanEffect();

        void setAuxiliaryEffect(unsigned int aux);

        unsigned int getAuxiliaryEffect() const;

        bool created() const;

        unsigned int getSlot() const;

        void init(unsigned int slot);

    protected:

        EffectBase(unsigned int& source, int effectType);

        void setEffect(int type, float value);

    };
}