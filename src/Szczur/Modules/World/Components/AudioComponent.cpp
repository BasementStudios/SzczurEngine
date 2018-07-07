#include "InteractableComponent.hpp"

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/Input/Input.hpp>

namespace rat {
	AudioComponent::AudioComponent(Entity* parent) :
		Component { parent, fnv1a_64("AudioComponent"), "AudioComponent"}, 
        _sound(detail::globalPtr<Sound>),
        _assets(_sound->getAssetsManager())
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

        auto& soundsList = config["SoundsList"];

        for (auto& it : soundsList) {
            add(it);
        }

	}

	void AudioComponent::saveToConfig(Json& config) const 
    {
		Component::saveToConfig(config);
		
        for (auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
            config["SoundsList"].push_back(it->first);
        }
	}

    void AudioComponent::renderHeader(ScenesManager& scenes, Entity* object) 
    {
        if(_loadingSound)
            load();

    	if (ImGui::CollapsingHeader("Audio##audio_component"))
        {
            if (ImGui::Button("Load##audio_component")) {
                _loadingSound = true;
            }

            ImGui::Separator();

            if (ImGui::TreeNode("Sounds List")) { 
                for(auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
                    auto name = it->second.getName();
                    ImGui::Text((name).c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("  -  ")) {
                        remove(name);
                        break;
                    }
                }
                ImGui::TreePop();   
            }
        }

    }

	void AudioComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<AudioComponent>("AudioComponent", "World");

        object.set("get", &AudioComponent::get);
        object.set("add", &AudioComponent::add);
        object.set("remove", &AudioComponent::remove);

        // Entity
		entity.set("addAudioComponent", [&](Entity& e){return (AudioComponent*)e.addComponent<AudioComponent>();});
		entity.set("audio", &Entity::getComponentAs<AudioComponent>);

		object.init();
	}

	void AudioComponent::update(ScenesManager& scenes, float deltaTime) 
    {
        auto pos = getEntity()->getPosition();
		for (auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
            it->second.setPosition(pos.x, pos.y, pos.z);
        }
	}

    SoundBase* AudioComponent::get(const std::string& name)
    {
        if (_soundHolder.count(name))
            return &_soundHolder[name];
        return nullptr;
    }

    
    SoundBase* AudioComponent::add(const std::string& name)
    {
        _soundHolder.try_emplace(name, SoundBase(name));
        _soundHolder[name].load();

        auto fileName = _soundHolder[name].getFileName();

        if(fileName.empty()) {
            _soundHolder.erase(name);
        } 
        else {
            _sound->addSound(&_soundHolder[name]);
            _assets.load(fileName);
            _soundHolder[name].setBuffer(_assets.get(fileName));
            _soundHolder[name].init();
        }

        return get(name);
    }
    
    void AudioComponent::remove(const std::string& name)
    {
        _sound->removeSound(&_soundHolder[name]);
        _soundHolder.erase(name);
    }

    void AudioComponent::play()
    {
        for (auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
            //it->second.play();
        }
    }

    void AudioComponent::load()
    {
        static std::string loadingSoundName = "";

        ImGui::Begin("Load Sound", NULL);

            ImGui::Text("Name: "); 
            ImGui::SameLine();

            size_t size = loadingSoundName.length() + 100;
            char *newText = new char[size] {};
            strncpy(newText, loadingSoundName.c_str(), size);

            ImGui::PushItemWidth(300);
                if (ImGui::InputText("##LoadingSoundNameInput", newText, size)) {
                    loadingSoundName = newText;
                }
                if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
                    ImGui::SetKeyboardFocusHere(0);
                }
            ImGui::PopItemWidth();

            delete[] newText;
            
            ImGui::SetCursorPosX(260);

            if (ImGui::Button("CANCEL##LoadSound")) {
                loadingSoundName = "";
                _loadingSound = false;
            }

            ImGui::SameLine(); 

            if (ImGui::Button(" OK ##LoadSound")) {
                add(loadingSoundName);
                loadingSoundName = "";
                _loadingSound = false;
            }

        ImGui::End();
    }
}
