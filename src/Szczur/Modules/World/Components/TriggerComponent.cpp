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
}