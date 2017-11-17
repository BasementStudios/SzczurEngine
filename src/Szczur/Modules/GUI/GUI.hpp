#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Utility/Modules.hpp"

#include "Widget.hpp"

namespace rat {
    class GUI : public ModuleBase<Canvas> { using ModuleBase::ModuleBase;
    public:
        //GUI() = default;

        void init();
        void update(float deltaTime);
        void render();
    private:

        Widget _root;
    };
}