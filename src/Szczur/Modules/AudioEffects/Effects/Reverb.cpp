#include "Reverb.hpp"

namespace rat
{

    Reverb::Reverb(unsigned int& source)
        : EffectBase(source, AL_EFFECT_REVERB, 2)
    {

    }

    void Reverb::initScript(Script& script)
	{
		auto object = script.newClass<Reverb>("Reverb", "AudioEffects");
		SCRIPT_SET_CLASS(Reverb, density, diffusion, gain, gainHf, decayTime, decayHfRatio, reflectionsGain, reflectionsDelay,
                        lateReverbGain, lateReverbDelay, airAbsorptionGainHf, roomRolloffFactor, decayHfLimit);
		object.init();
	} 

    void Reverb::density(float density)
    {
        setEffect(AL_REVERB_DENSITY, density);
    }

    void Reverb::diffusion(float diffusion)
    {
        setEffect(AL_REVERB_DIFFUSION, diffusion);       
    }

    void Reverb::gain(float gain)
    {
        setEffect(AL_REVERB_GAIN, gain);
    }

    void Reverb::gainHf(float gainHF)
    {
        setEffect(AL_REVERB_GAINHF, gainHF);
    }

    void Reverb::decayTime(float decayTime)
    {
        setEffect(AL_REVERB_DECAY_TIME, decayTime);
    }

    void Reverb::decayHfRatio(float decayHfRatio)
    {
        setEffect(AL_REVERB_DECAY_HFRATIO, decayHfRatio);
    }

    void Reverb::reflectionsGain(float reflectionsGain)
    {
        setEffect(AL_REVERB_REFLECTIONS_GAIN, reflectionsGain);
    }

    void Reverb::reflectionsDelay(float reflectionsDelay)
    {
        setEffect(AL_REVERB_REFLECTIONS_DELAY, reflectionsDelay);
    }

    void Reverb::lateReverbGain(float lateReverbGain)
    {
        setEffect(AL_REVERB_LATE_REVERB_GAIN, lateReverbGain);
    }

    void Reverb::lateReverbDelay(float lateReverbDelay)
    {
        setEffect(AL_REVERB_LATE_REVERB_DELAY, lateReverbDelay);
    }

    void Reverb::airAbsorptionGainHf(float airAbsorptionGainHf)
    {
        setEffect(AL_REVERB_AIR_ABSORPTION_GAINHF, airAbsorptionGainHf);
    }

    void Reverb::roomRolloffFactor(float roomRolloffFactor)
    {
        setEffect(AL_REVERB_ROOM_ROLLOFF_FACTOR, roomRolloffFactor);
    }

    void Reverb::decayHfLimit(bool decayHfLimit)
    {
        setEffect(AL_REVERB_DECAY_HFLIMIT, decayHfLimit);
    } 

}