#pragma once

#include "Szczur/Modules/World/Componable.hpp"
#include "Szczur/Modules/World/BaseObject.hpp"

#include "Szczur/Modules/Input/InputManager.hpp"

namespace rat {
    class InputController : public Componable::Component<BaseObject> {
    public:
        InputController();

        void update(const InputManager& input);
    };
}