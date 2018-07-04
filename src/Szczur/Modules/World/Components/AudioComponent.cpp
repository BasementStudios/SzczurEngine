#include "InteractableComponent.hpp"

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/Input/Input.hpp>

namespace rat {
    AudioComponent::AudioComponent(Entity* parent) :
        Component { parent, fnv1a_64("AudioComponent"), "AudioComponent"}
    {
    }

    std::unique_ptr<Component> AudioComponent::copy(Entity* newParent) const
    {
        auto ptr = std::make_unique<AudioComponent>(*this);

        ptr->setEntity(newParent);

        return ptr;
    }

    void AudioComponent::loadFromConfig(Json& config)
    {
        Component::loadFromConfig(config);
    }

    void AudioComponent::saveToConfig(Json& config) const
    {
        Component::saveToConfig(config);

    }

    void AudioComponent::renderHeader(ScenesManager& scenes, Entity* object)
    {
        if (ImGui::CollapsingHeader("Audio##audio_component"))
        {
			;
        }
    }

    void AudioComponent::initScript(ScriptClass<Entity>& entity, Script& script)
    {
        auto object = script.newClass<AudioComponent>("AudioComponent", "World");

        object.init();
    }

    void AudioComponent::update(ScenesManager& scenes, float deltaTime)
    {
         ;
    }
}
