#include "MusicEffect.hpp"

#include "Szczur/Debug.hpp"

namespace rat
{

    MusicEffect::MusicEffect(unsigned int& source, int effectType)
        : _source(source), _effectType(effectType)
    {

    }

    void MusicEffect::cleanEffect()
    {
        alSource3i(_source, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, _sourceSlot, NULL);
        alAuxiliaryEffectSloti(_effectSlot, AL_EFFECTSLOT_NULL, 0);
        alGetError();
    }

    void MusicEffect::init()
    {
        if (_created) return;
        else _created = true;

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
            _sourceSlot = _lastFreeSlot;
            ++_lastFreeSlot;
            LOG_INFO("OpenAL: Effect correctly initialized");
        }   
    }

    void MusicEffect::setEffect(int type, float value)
    {
        init();

        if (_supported) {
            alEffectf(_effect, type, value);
        }
        else {
            LOG_INFO("OpenAL Error: Effect is not supported!");
            return;
        }
        
       cleanEffect();
    
        alGetError();
        alAuxiliaryEffectSloti(_effectSlot, AL_EFFECTSLOT_EFFECT, _effect);
		if (alGetError() != AL_NO_ERROR) {
			LOG_INFO("OpenAL Error: Problem with loading Effect into Auxiliary Effect Slot");
            return;
        }

        alGetError();
        alSource3i(_source, AL_AUXILIARY_SEND_FILTER, _effectSlot, _sourceSlot, NULL);
		if (alGetError() != AL_NO_ERROR){
			LOG_INFO("OpenAL Error: Problem with loading Auxiliary Effect Slot into Source");
            return;
        }

        LOG_INFO("OpenAL: Audio effect correctly loaded");
    }

}