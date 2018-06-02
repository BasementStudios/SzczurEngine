#pragma once

#include "../Component.hpp"

#include <sol.hpp>

#include <glm/glm.hpp>

namespace rat {
class Entity;
class Script;
class ScenesManger;
class InputManager;

class InteractableComponent : public Component {
public:

    ///
    InteractableComponent(Entity* parent);

    ///
    virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

    ///
    void callback();

    ///
    bool checkForInteraction(const glm::vec3& position) const;

    ///
    void setDistance(float distance);

    ///
    float getDistance() const;

    ///
    void setHeight(float height);

    ///
    float getHeight() const;

    ///
    virtual void loadFromConfig(Json& config) override;

    ///
    virtual void saveToConfig(Json& config) const override;

    ///
    static void initScript(Script& script);

    ///
    virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

    ///
    void update(ScenesManager& scenes, float deltaTime);

private:

    sol::function _interactionCallback;
    InputManager& _input;
    float _distance{50.f};
    float _height{0.f};
};

}
