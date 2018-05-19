#include "InputControllerComponent.hpp"

#include "../Entity.hpp"

#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/Input/InputManager.hpp>

namespace rat {
    InputControllerComponent::InputControllerComponent(Entity* parent) :
    Component { parent, fnv1a_64("InputControllerComponent"), "InputControllerComponent"} {

    }

    void InputControllerComponent::update(InputManager& input, float deltaTime) {
        if(_updateCallback.valid())
            _updateCallback(*this, input, deltaTime);
    }

    std::unique_ptr<Component> InputControllerComponent::copy(Entity* newParent) const {
		auto ptr = std::make_unique<InputControllerComponent>(*this);

        ptr->setEntity(newParent);

        return ptr;
	}

    void InputControllerComponent::initScript(Script& script) {
        auto object = script.newClass<InputControllerComponent>("InputControllerComponent", "World");
		object.set("onUpdate", &InputControllerComponent::_updateCallback);
		object.init();
    }
}