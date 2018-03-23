#pragma once

#include "Szczur/Utility/Modules/Module.hpp"

#include "Effect.hpp"

namespace rat 
{   
    class AudioEffects : public Module<Script>
    {

        std::unique_ptr<Effect> _globalEffects = nullptr;

    public:

        AudioEffects();
        ~AudioEffects();

        void initScript();

        bool globalEffects();
        Effect& getGlobalEffects();
        void cleanGlobalEffects();

    };
}