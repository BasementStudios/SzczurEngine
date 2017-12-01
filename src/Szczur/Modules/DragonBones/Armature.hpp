#pragma once

#include <memory>

#include <dragonBones/SFMLArmatureDisplay.h>

namespace rat {
    class Armature {
        std::unique_ptr<dragonBones::SFMLArmatureDisplay> _armatureDisplay;

    public:
        Armature();
        ~Armature();

        void draw();
    };
}
