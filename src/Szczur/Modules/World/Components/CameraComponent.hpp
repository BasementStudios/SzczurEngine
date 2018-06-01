#pragma once

#include <SFML/Graphics.hpp>

#include "../Component.hpp"


namespace rat {

class InputManager;
class Entity;

class CameraComponent : public Component {
public:

    ///
    CameraComponent(Entity* parent);

    ///
    void processEvents(InputManager& input);

    ///
    virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

    ///
    virtual void loadFromConfig(const Json& config) override;

    ///
    virtual void saveToConfig(Json& config) const override;

    ///
    void setVelocity(float velocity);

    ///
    float getVelocity() const;

    ///
    void setLock(bool lock);

    ///
    bool getLock() const;

    ///
    void setStickToPlayer(bool value);

    ///
    bool getStickToPlayer() const;

    ///
    virtual void renderHeader(ScenesManager& scenes, Entity* object) override;
    
    ///
    void update(ScenesManager& scenes, float deltaTime);

private:
    bool _rotating{false};
    bool _locked{false};
    bool _stickToPlayer{false};
    float _velocity{50.f};
    sf::Vector2i _previousMouse;
};

}