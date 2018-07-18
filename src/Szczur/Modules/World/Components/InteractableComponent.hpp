#pragma once

#include <memory> // unique_ptr

#include <glm/vec3.hpp>
#include <sol.hpp>
#include <nlohmann/json_fwd.hpp>

namespace rat {
    class ScenesManger;
    class InputManager;
    class Entity;
    class Script;
    template<class T> class ScriptClass;
}
#include "../Component.hpp"

namespace rat
{

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
    virtual void loadFromConfig(nlohmann::json& config) override;

    ///
    virtual void saveToConfig(nlohmann::json& config) const override;

    ///
    static void initScript(ScriptClass<Entity>& entity, Script& script);

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
