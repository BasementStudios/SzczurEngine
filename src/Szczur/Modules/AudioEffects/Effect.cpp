#include "Effect.hpp"

#include "RatSound.hpp"
#include "Szczur/Modules/Music/RatMusic.hpp"

namespace rat
{
    Effect::Effect()
        : AudioEffect(fakeSource)
    {

    }

    bool Effect::exist() 
    {
        return ( 
            get<Equalizer>().created() ||
            get<Reverb>().created()    ||
            get<Echo>().created()
        );
    }

    void Effect::initScript(Script& script)
    {
        auto object = script.newClass<Effect>("Effect", "AudioEffects");

        object.set("getEqualizer", &AudioEffect::getEffect<Equalizer>);
		object.set("getReverb", &Effect::AudioEffect::getEffect<Reverb>);
		object.set("getEcho", &Effect::AudioEffect::getEffect<Echo>);

		object.set("cleanEqualizer", &Effect::AudioEffect::cleanEffect<Equalizer>);
		object.set("cleanReverb", &Effect::AudioEffect::cleanEffect<Reverb>);
		object.set("cleanEcho", &Effect::AudioEffect::cleanEffect<Echo>);

        object.set("sendToMusic", &Effect::sendAuxiliaryEffectsTo<RatMusic>);
        object.set("sendToSound", &Effect::sendAuxiliaryEffectsTo<RatSound>);

        object.makeInstance();

		object.init();
    }
}