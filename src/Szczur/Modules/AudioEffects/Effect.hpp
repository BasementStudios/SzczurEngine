#pragma once

#include "AudioEffect.hpp"

namespace rat
{
    class Effect : public AudioEffect
    {

        unsigned int fakeSource; 

    public:

        Effect();

        bool exist();

        static void initScript(Script& script);

        template <typename T>
        unsigned int getAuxiliaryEffect();

        template <typename T>
        void sendAuxiliaryEffectsTo(T* ratAudio);

    private:

        template <typename Effect_t, typename RatAudio_t>
        void sendAuxIfCreated(RatAudio_t* ratAudio);

    };
}

#include "Effect.tpp"