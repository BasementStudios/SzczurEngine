#pragma once

#include "../Component.hpp"

#include <sol.hpp>

#include <glm/glm.hpp>

namespace rat {
class Entity;
class Script;

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
    virtual void loadFromConfig(const Json& config) override;

    ///
    virtual void saveToConfig(Json& config) const override;

    ///
    static void initScript(Script& script);

    ///
    virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

private:

    sol::function _interactionCallback;
    float _distance{50.f};
    float _height{0.f};
};

}
