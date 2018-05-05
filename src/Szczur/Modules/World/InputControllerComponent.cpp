#include "InputControllerComponent.hpp"

namespace rat {
    InputControllerComponent::InputControllerComponent() :
    Component("InputControllerComponent", typeID<InputControllerComponent>()) {

    }

    glm::vec3 InputControllerComponent::update(const InputManager& input) {
        glm::vec3 offset{0.f, 0.f, 0.f};
        if(input.isKept(Keyboard::Right))
            offset.x += 20.f;
        if(input.isKept(Keyboard::Left))
            offset.x -= 20.f;
        if(input.isKept(Keyboard::Up))
            offset.z -= 20.f;
        if(input.isKept(Keyboard::Down))
            offset.z += 20.f;
        return offset;
    }

    
	std::unique_ptr<Component> InputControllerComponent::copy() const {
        return std::make_unique<InputControllerComponent>(*this);
    }
}