#include "TriggerComponent.hpp"

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Modules/Input/Input.hpp"

namespace rat {
	TriggerComponent::TriggerComponent(Entity* parent) :
    	Component{parent, fnv1a_64("TriggerComponent"), "TriggerComponent"},
        _input(detail::globalPtr<Input>->getManager()) {

	}

	std::unique_ptr<Component> TriggerComponent::copy(Entity* newParent) const
	{
		auto ptr = std::make_unique<TriggerComponent>(*this);

		ptr->setEntity(newParent);

		return ptr;
	}

	bool TriggerComponent::checkForTrigger(const glm::vec3& position) const {
		auto delta = position - getEntity()->getPosition();
		if(delta.x*delta.x + delta.z*delta.z <= _radius*_radius) {
			return true;
        }
		return false;
	}

	void TriggerComponent::setRadius(float radius) {
		_radius = radius;
	}  

	float TriggerComponent::getRadius() const {
		return _radius;
	}

	std::string TriggerComponent::enumToString(size_t en) {
		static std::string names[] {"None", "Change scene"};
		return names[en];
	}

	TriggerComponent::Type TriggerComponent::uintToEnum(size_t s) {
		switch(s) {
			case 0: return None;
			case 1: return ChangeScene;
		}
	}

	void TriggerComponent::loadFromConfig(Json& config) {
		Component::loadFromConfig(config);
		_radius = config["radius"];
		std::cout << "ID: " << config["type"].get<size_t>() << '\n';
		std::cout << "asdasdasd\n";
		type = uintToEnum(config["type"].get<size_t>());
		if(type == ChangeScene) {
			std::cout << "Passed\n";
			sceneId = config["sceneId"];
			entranceId = config["enranceId"];
		}
	}

	void TriggerComponent::saveToConfig(Json& config) const {
		Component::saveToConfig(config);
		config["type"] = static_cast<size_t>(type);
		config["radius"] = _radius;
		if(type == ChangeScene) {
			config["sceneId"] = sceneId;
			config["enranceId"] = entranceId;
		}
	}

    void TriggerComponent::update(ScenesManager& scenes, float deltaTime) {
        auto* player = getEntity()->getScene()->getPlayer();
        if(player == nullptr) return;

        // Active trigger after [LShift]
        if(_input.isPressed(Keyboard::LShift)) {
            // Is player inside trigger
            if(checkForTrigger(player->getPosition())) {
                // Action for ChangeScene trigger
                if(type == TriggerComponent::ChangeScene) {
                    // Change scene after teleport
                    scenes.setCurrentScene(sceneId);
                    // Set player position equal entry
                    auto* scene = scenes.getCurrentScene();
                    if(auto* entry = scene->getEntity("entries", entranceId)) {
                        scene->getPlayer()->setPosition(entry->getPosition());                        
                    }
                }
            }
        }
    }

    void TriggerComponent::renderHeader(ScenesManager& scenes, Entity* object) {
    	if (ImGui::CollapsingHeader("Trigger##trigger_component"))
        {
        	// Combo box with available types
            if (ImGui::BeginCombo("Types", enumToString(type).c_str()))
            {
                if (ImGui::Selectable(enumToString(Type::None).c_str(), type == Type::None))
                {
                    type = Type::None;
                }
                if (ImGui::Selectable(enumToString(Type::ChangeScene).c_str(), type == Type::ChangeScene))
                {
                    type = Type::ChangeScene;
                    sceneId = 0u;
                    entranceId = 0u;
                }
                ImGui::EndCombo();
            }

            // Set radius
            float radius = getRadius();
            ImGui::DragFloat("Radius", &radius);
            setRadius(radius);

            // Options for "Chagne scene" type
            if (type == Type::ChangeScene)
            {
                // Selected
                auto* scene = scenes.getScene(sceneId);

                // List of scenes
                if (ImGui::BeginCombo("Scene", scene ? scene->getName().c_str() : "None"))
                {
                    for (auto& it : scenes.getScenes())
                    {
                        if (ImGui::Selectable(it->getName().c_str(), sceneId == it->getID()))
                        {
                            sceneId = it->getID();
                        }
                    }
                    ImGui::EndCombo();
                }

                // If scene is selected
                if (sceneId)
                {                    
                    // Find name of selected entry
                    scene = scenes.getScene(sceneId);
                    std::string name = "None";
                    auto& entries = scene->getEntities("entries");
                    if(auto* entry = scene->getEntity("entries", entranceId)) {
                        name = entry->getName();
                    }
                    
                    // Combo box with available entries in selected scene
                    if (ImGui::BeginCombo("Entry", name.c_str()))
                    {
                        for (auto& entry : entries)
                        {
                            if (ImGui::Selectable(entry->getName().c_str(), entry->getID() == entranceId))
                            {
                                entranceId = entry->getID();
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
            }
        }
    }
}