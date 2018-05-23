#include "EffectBase.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

    EffectBase::EffectBase(unsigned int& source, int effectType)
        : _source(source), _effectType(effectType)
    {

    }

    void EffectBase::cleanEffect()
    {
        alSource3i(_source, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, _sourceSlot, NULL);
        alAuxiliaryEffectSloti(_effectSlot, AL_EFFECTSLOT_NULL, 0);
        alGetError();

        _effectSlot = NULL;
        _effect = NULL;

        LOG_INFO("OpenAL: Effect cleaned");
        _created = false;
    }

    void EffectBase::init(unsigned int slot)
    {
        if (_created) return;
        else _created = true;

        _sourceSlot = slot;

        alGetError();

        alGenAuxiliaryEffectSlots(1, &_effectSlot);
        if (alGetError() != AL_NO_ERROR) {
            LOG_INFO("OpenAL Error: Problem with generating Auxiliary Effect Slot");
            return;
        }

        alGenEffects(1, &_effect);
        if (alGetError() != AL_NO_ERROR) {
            LOG_INFO("OpenAL Error: Problem with generating Effect");
            return;
        }

        alGetError();
        if (alIsEffect(_effect)) {
            alEffecti(_effect, AL_EFFECT_TYPE, _effectType);
            if (alGetError() != AL_NO_ERROR) {
                LOG_INFO("OpenAL Error: Effect is not supported!");
                return; 
            }
            _supported = true;
            LOG_INFO("OpenAL: Effect correctly initialized");
        }   
    }

    void EffectBase::setEffect(int type, float value)
    {
        if (_supported) {
            alEffectf(_effect, type, value);
        }
        else {
            LOG_INFO("OpenAL Error: Effect is not supported!");
            return;
        }
        
        alSource3i(_source, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, _sourceSlot, NULL);
        alAuxiliaryEffectSloti(_effectSlot, AL_EFFECTSLOT_NULL, 0);
        alGetError();
    
        alGetError();
        alAuxiliaryEffectSloti(_effectSlot, AL_EFFECTSLOT_EFFECT, _effect);
		if (alGetError() != AL_NO_ERROR) {
			LOG_INFO("OpenAL Error: Problem with loading Effect into Auxiliary Effect Slot");
            return;
        }

        alGetError();
        alSource3i(_source, AL_AUXILIARY_SEND_FILTER, _effectSlot, _sourceSlot, NULL);
		if (alGetError() != AL_NO_ERROR && _source != 0) {
			LOG_INFO("OpenAL Error: Problem with loading Auxiliary Effect Slot into Source");
            return;
        }

        LOG_INFO("OpenAL: Audio effect correctly loaded");
    }

    void EffectBase::setAuxiliaryEffect(unsigned int aux)
    {
        _effectSlot = aux;

        alGetError();
        alSource3i(_source, AL_AUXILIARY_SEND_FILTER, _effectSlot, _sourceSlot, NULL);
        if (alGetError() != AL_NO_ERROR) {
			LOG_INFO("OpenAL Error: Problem with loading Auxiliary Effect Slot into Source");
            return;
        }
    }

    unsigned int EffectBase::getAuxiliaryEffect() const
    {
        return _effectSlot;
    }

    bool EffectBase::created() const
    {
        return _created;
    }

    unsigned int EffectBase::getSlot() const 
    {
        return _sourceSlot;
    }

}