#pragma once

#include "../Component.hpp"

#include <sol.hpp>

namespace rat {
    class Entity;

    class ScriptableComponent : public Component {
    public:
        ScriptableComponent(Entity* parent);

        void update(float deltaTime);

        void loadFromFile(const std::string& path);

        void setCallbacks(const sol::function& updateCallback, const sol::function& initCallback);
    private:
        sol::function _updateCallback;
        std::string _scriptName;
    };
}