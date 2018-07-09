#pragma once

#include "../Component.hpp"

#include "Szczur/Modules/Sound/Sound.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

#include <unordered_map>
#include <string>

namespace rat {
class Script;
class ScenesManger;

class Script;
class Entity;
template<class T> class ScriptClass;

class AudioComponent : public Component 
{

    using Container_t = std::unordered_map<std::string, SoundBase>;

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

    ///
    SoundBase* get(const std::string& name);

    ///
    SoundBase* add(const std::string& name);

    ///
    void remove(const std::string& name);

    ///
    void play();

private:

    void load();

    bool _loadingSound {false};

    Sound* _sound;
    SoundAssets& _assets;

    Container_t _soundHolder;
    
};

}
