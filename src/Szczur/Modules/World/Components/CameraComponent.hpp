#pragma once

#include <SFML/Graphics.hpp>

#include "../Component.hpp"


namespace rat {

class InputManager;
class Entity;
class Script;
template<class T> class ScriptClass;

class CameraComponent : public Component {
public:

    ///
    CameraComponent(Entity* parent);

    ///
    void processEvents(InputManager& input);

    ///
    virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

    ///
    virtual void loadFromConfig(Json& config) override;

    ///
    virtual void saveToConfig(Json& config) const override;

    ///
    void setVelocity(float velocity);

    ///
    float getVelocity() const;

    void setSmoothness(float smoothness);

    float getSmoothness() const;

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

    ///
    static void initScript(ScriptClass<Entity>& entity, Script& script);

private:
    bool _rotating{false};
    bool _locked{false};
    bool _stickToPlayer{false};
    float _velocity{50.f};
    float _smoothness{1.f};
    sf::Vector2i _previousMouse;
};

}