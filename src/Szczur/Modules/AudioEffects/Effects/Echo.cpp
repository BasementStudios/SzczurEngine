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
		object.set("delay", &Echo::delay);
		object.set("lrDelay", &Echo::lrDelay);
		object.set("damping", &Echo::damping);
		object.set("feedback", &Echo::feedback);
		object.set("spread", &Echo::spread);
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
