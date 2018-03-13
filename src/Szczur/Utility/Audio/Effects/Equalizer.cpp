#include "Equalizer.hpp"

namespace rat
{

    Equalizer::Equalizer(unsigned int& source)
        : MusicEffect(source, AL_EFFECT_EQUALIZER)
    {

    }

    void Equalizer::initScript(Script& script)
	{
		auto object = script.newClass<Equalizer>("Equalizer", "Music");
		SCRIPT_SET_CLASS(Equalizer, low, lowMid, highMid, high, lowCutoff, lowGain, lowMidCenter, lowMidWidth, lowMidGain, 
                        highMidCenter, highMidWidth, highCutoff, highGain);
		object.init();
	} 

    void Equalizer::low(float gain, float cutoff)
    {
        if (cutoff != 0) lowCutoff(cutoff);
        lowGain(gain);
    }

    void Equalizer::lowMid(float gain, float center, float width)
    {
        if (center != 0) lowMidCenter(center);
        if (width != 0)  lowMidWidth(width);
        lowMidGain(gain);
    }

    void Equalizer::highMid(float gain, float center, float width)
    {
        if (center != 0) highMidCenter(center);
        if (width != 0)  highMidWidth(width);
        highMidGain(gain);
    }

    void Equalizer::high(float gain, float cutoff)
    {
        if (cutoff != 0) highCutoff(cutoff);
        highGain(gain);
    }

    void Equalizer::lowCutoff(float cutoff)
    {
        setEffect(AL_EQUALIZER_LOW_CUTOFF, cutoff);
    }

    void Equalizer::lowGain(float gain)
    {
        setEffect(AL_EQUALIZER_LOW_GAIN, gain);
    }

    void Equalizer::lowMidCenter(float center)
    {
        setEffect(AL_EQUALIZER_MID1_CENTER, center);
    }

    void Equalizer::lowMidWidth(float width)
    {
        setEffect(AL_EQUALIZER_MID1_WIDTH, width);
    }

    void Equalizer::lowMidGain(float gain)
    {
        setEffect(AL_EQUALIZER_MID1_GAIN, gain);
    }

    void Equalizer::highMidCenter(float center)
    {
        setEffect(AL_EQUALIZER_MID2_CENTER, center);
    }

    void Equalizer::highMidWidth(float width)
    {
        setEffect(AL_EQUALIZER_MID2_WIDTH, width);
    }

    void Equalizer::highMidGain(float gain)
    {
        setEffect(AL_EQUALIZER_MID2_GAIN, gain);
    }

    void Equalizer::highCutoff(float cutoff)
    {
        setEffect(AL_EQUALIZER_HIGH_CUTOFF, cutoff);
    }

    void Equalizer::highGain(float gain)
    {
        setEffect(AL_EQUALIZER_HIGH_GAIN, gain);
    }

}