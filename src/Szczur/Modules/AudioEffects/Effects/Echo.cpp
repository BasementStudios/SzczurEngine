#include "Echo.hpp"

namespace rat
{

	Echo::Echo(unsigned int& source)
		: EffectBase(source, AL_EFFECT_ECHO)
	{

	}

	void Echo::initScript(Script& script)
	{
		auto object = script.newClass<Echo>("Echo", "AudioEffects");
		SCRIPT_SET_CLASS(Echo, delay, lrDelay, damping, feedback, spread);
		object.init();
	} 

	void Echo::delay(float delay)
	{
		setEffect(AL_ECHO_DELAY, delay);
	}

	void Echo::lrDelay(float lrDelay)
	{
		setEffect(AL_ECHO_LRDELAY, lrDelay);
	}

	void Echo::damping(float damping)
	{
		setEffect(AL_ECHO_DAMPING, damping);
	}

	void Echo::feedback(float feedback)
	{
		setEffect(AL_ECHO_FEEDBACK, feedback);
	}

	void Echo::spread(float spread)
	{
		setEffect(AL_ECHO_SPREAD, spread);
	}


}