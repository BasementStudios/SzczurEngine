#include "TriggerComponent.hpp"
#include "../Entity.hpp"

namespace rat {
    TriggerComponent::TriggerComponent(Entity* parent) :
    Component{parent, fnv1a_64("TriggerComponent"), "TriggerComponent"} {

    }

    std::unique_ptr<Component> TriggerComponent::copy(Entity* newParent) const
	{
		auto ptr = std::make_unique<TriggerComponent>(*this);

        ptr->setEntity(newParent);

        return ptr;
	}

    bool TriggerComponent::checkForTrigger(const glm::vec3& position) const {
        auto* entity = getEntity();
        auto delta = position - entity->getPosition();
        if(delta.x*delta.x + delta.z*delta.z <= _radius*_radius)
            return true;
        return false;
    }

    void TriggerComponent::setRadius(float radius) {
        _radius = radius;
    }  

    float TriggerComponent::getRadius() const {
        return _radius;
    }

    std::string TriggerComponent::enumToString(size_t en) {
        switch(en) {
            case None: return "None";
            case ChangeScene: return "Change Scene";
        }
    }
    TriggerComponent::Type TriggerComponent::uintToEnum(size_t s) {
        switch(s) {
            case 0: return None;
            case 1: return ChangeScene;
        }
    }

    void TriggerComponent::loadFromConfig(const Json& config) {
        Component::loadFromConfig(config);
        _radius = config["radius"];
        std::cout << "ID: " << config["type"].get<size_t>() << '\n';
        std::cout << "asdasdasd\n";
        type = uintToEnum(config["type"].get<size_t>());
        if(type == ChangeScene) {
            std::cout << "Passed\n";
            sceneId = config["sceneId"];
            entranceId = config["enranceId"];
        }
    }

    void TriggerComponent::saveToConfig(Json& config) const {
        Component::saveToConfig(config);
        config["type"] = static_cast<size_t>(type);
        config["radius"] = _radius;
        if(type == ChangeScene) {
            config["sceneId"] = sceneId;
            config["enranceId"] = entranceId;
        }
    }
}