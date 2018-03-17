#pragma once

#include "AudioEffect.hpp"

namespace rat
{
    class Effect : public AudioEffect
    {

        unsigned int fakeSource; 

    public:

        Effect();

        static void initScript(Script& script);

        template <typename T>
        unsigned int getAuxiliaryEffect()
        {
            return getEffect<T>().getAuxiliaryEffect();
        }

        template <typename T>
        void sendAuxiliaryEffectsTo(T& ratAudio)
        {
            sendAuxIfCreated<Equalizer, T>(ratAudio);
            sendAuxIfCreated<Echo, T>(ratAudio);
            sendAuxIfCreated<Reverb, T>(ratAudio);
        }

    private:

        template <typename Effect_t, typename RatAudio_t>
        void sendAuxIfCreated(RatAudio_t& ratAudio)
        {
            if (getEffect<Effect_t>().created())
                ratAudio.template setEffect<Effect_t>(getAuxiliaryEffect<Effect_t>());
        }

    };
}