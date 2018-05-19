#include "InteractableComponent.hpp"

#include "../Entity.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {
    InteractableComponent::InteractableComponent(Entity* parent) :
    Component { parent, fnv1a_64("InteractableComponent"), "InteractableComponent"} {

    }

    void InteractableComponent::callback() {
        if(_interactionCallback.valid())
            _interactionCallback();
    }

    void InteractableComponent::setDistance(float distance) {
        _distance = distance;
    }
    float InteractableComponent::getDistance() const {
        return _distance;
    }

    bool InteractableComponent::checkForInteraction(const glm::vec3& position) const {
        auto* entity = getEntity();
        auto delta = position - entity->getPosition();
        if(delta.x*delta.x + delta.z*delta.z <= _distance*_distance)
            return true;
        return false;
    }

    std::unique_ptr<Component> InteractableComponent::copy(Entity* newParent) const {
		auto ptr = std::make_unique<InteractableComponent>(*this);

        ptr->setEntity(newParent);

        return ptr;
	}

    void InteractableComponent::loadFromConfig(const Json& config) {
        Component::loadFromConfig(config);
        _distance = config["distance"];
    }

	void InteractableComponent::saveToConfig(Json& config) const {
        Component::saveToConfig(config);
		config["distance"] = _distance;
    }

    void InteractableComponent::initScript(Script& script) {
        auto object = script.newClass<InteractableComponent>("InteractableComponent", "World");
		object.set("onInteraction", &InteractableComponent::_interactionCallback);
		object.init();
    }
}