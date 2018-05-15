#include "ScriptableComponent.hpp"

#include "../Entity.hpp"

namespace rat {
    ScriptableComponent::ScriptableComponent(Entity* parent) :
    Component { parent, fnv1a_64("ScriptableComponent"), "ScriptableComponent"} {

    }

    void ScriptableComponent::update(float deltaTime) {
        if(_updateCallback.valid())
            _updateCallback(deltaTime);
    }

    void ScriptableComponent::loadFromFile(const std::string& path) {

    }

    void ScriptableComponent::setCallbacks(const sol::function& updateCallback, const sol::function& initCallback) {
        if(initCallback.valid())
            initCallback();
        if(updateCallback.valid())
            _updateCallback = updateCallback;
    }
}