#pragma once

#include <memory>

#include "dragonBonesSFML/SFMLFactory.h"

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Utility/Modules.hpp"

#include "Armature.hpp"

namespace rat {
    class DragonBones : public ModuleBase<Canvas> { using ModuleBase::ModuleBase;
    
    friend Armature;

    private:
        std::unique_ptr<dragonBones::SFMLFactory> _factory;

    public:
        void init();
        void update(float deltaTime);

        auto getFactory() { return _factory.get(); }
    };
}
