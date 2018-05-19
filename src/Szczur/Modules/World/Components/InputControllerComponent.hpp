#pragma once

#include "../Component.hpp"

#include <sol.hpp>

namespace rat {
    class InputManager;
    class Entity;
    class Script;
    class InputControllerComponent : public Component {
    public:
        InputControllerComponent(Entity* parent);

        void update(InputManager& input, float deltaTime);

        virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

        virtual void loadFromConfig(const Json& config) override{}

	    virtual void saveToConfig(Json& config) const override{}

        static void initScript(Script& script);
    private:
        sol::function _updateCallback;

    };
}