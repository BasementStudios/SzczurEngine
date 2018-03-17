#pragma once

#include <memory>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Debug.hpp"

#include "Effect.hpp"

namespace rat 
{   
    class AudioEffects : public Module<Script>
    {

        std::unique_ptr<Effect> _globalEffects = nullptr;

    public:

        AudioEffects();

        void initScript();

        bool globalEffects();
        Effect& getGlobalEffects();
        void cleanGlobalEffects();

    };
}