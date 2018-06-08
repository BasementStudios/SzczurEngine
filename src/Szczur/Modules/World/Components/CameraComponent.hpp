#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/SFML3D/View.hpp"

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

    void stickTo(Entity* entity);

    ///
    void setLock(bool lock);

    ///
    bool getLock() const;

    ///
    virtual void renderHeader(ScenesManager& scenes, Entity* object) override;
    
    void stickToPlayer();

    ///
    void update(ScenesManager& scenes, float deltaTime);

    sf3d::View getRecalculatedView(sf3d::View baseView);;

    ///
    static void initScript(ScriptClass<Entity>& entity, Script& script);

private:
    bool _rotating{false};
    bool _locked{false};

    bool _limitedRange{false};
    struct {
        float left{0.f};
        float right{0.f};
    } _limit;

    float _velocity{50.f};
    float _smoothness{1.f};
    sf::Vector2i _previousMouse;

    Entity* _stickTo{nullptr};

};

}