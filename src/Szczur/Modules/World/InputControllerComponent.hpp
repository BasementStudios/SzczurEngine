#pragma once
#include "Component.hpp"
#include <Szczur/Modules/Input/InputManager.hpp>
#include <glm/glm.hpp>

namespace rat {
    class InputControllerComponent : public Component {
    public:
        InputControllerComponent();

        glm::vec3 update(const InputManager& input);

        virtual std::unique_ptr<Component> copy() const override;

        virtual void loadFromConfig(const Json& config) override {}

        ///
        virtual void saveToConfig(Json& config) const override {}
    };
}