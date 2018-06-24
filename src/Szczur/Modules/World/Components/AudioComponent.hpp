#pragma once

#include "../Component.hpp"

namespace rat {
class Script;
class ScenesManger;

class Script;
class Entity;
template<class T> class ScriptClass;

class AudioComponent : public Component {
public:

    ///
    AudioComponent(Entity* parent);

    ///
    virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

    ///
    virtual void loadFromConfig(Json& config) override;

    ///
    virtual void saveToConfig(Json& config) const override;

    ///
    virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

    ///
    static void initScript(ScriptClass<Entity>& entity, Script& script);

    ///
    void update(ScenesManager& scenes, float deltaTime);

private:

    
};

}
