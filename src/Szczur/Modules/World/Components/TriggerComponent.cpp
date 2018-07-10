#include "TriggerComponent.hpp"

#include <SFML/Graphics/Rect.hpp>

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Utility/ImGuiTweaks.hpp"
#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Window/Window.hpp"

namespace rat {
	TriggerComponent::TriggerComponent(Entity* parent) :
    	Component{parent, fnv1a_64("TriggerComponent"), "TriggerComponent"},
        _input(detail::globalPtr<Input>->getManager()) {
	}

	std::unique_ptr<Component> TriggerComponent::copy(Entity* newParent) const {
		auto ptr = std::make_unique<TriggerComponent>(*this);

		ptr->setEntity(newParent);

		return ptr;
	}

	bool TriggerComponent::checkForTrigger(const glm::vec3& position) const {
		auto entityPos = getEntity()->getPosition();

		if (_triggerShape == Shape::Circle) {
			auto delta = position - entityPos;
			if (delta.x*delta.x + delta.z*delta.z <= _radius * _radius) {
				return true;
			}
			return false;
		}
		else if (_triggerShape == Shape::Rectangle) {
			sf::FloatRect rect(entityPos.x - _rectSize.x / 2.f, entityPos.z - _rectSize.y / 2.f, _rectSize.x, _rectSize.y);

			if (rect.contains({ position.x, position.z }))
				return true;

			return false;
		}

		return false;
	}

	void TriggerComponent::setRadius(float radius) {
		_radius = radius;
	}  

	float TriggerComponent::getRadius() const {
		return _radius;
	}

	void TriggerComponent::setRectSize(float x, float y)
	{
		_rectSize = { x, y };
	}

	const glm::vec2& TriggerComponent::getRectSize() const {
		return _rectSize;
	}

	void TriggerComponent::setType(Type type) {
		if (type < 0 || type >= Type::TypesCount)
			type = Type::None;
		else
			this->type = type;
	}

	TriggerComponent::Type TriggerComponent::getType() {
		return type;
	}

	void TriggerComponent::setScene(const std::string& name)
	{
		if (auto scene = getEntity()->getScene()->getScenes()->getScene(name); scene != nullptr) {
			sceneId = scene->getID();
		}
	}

	std::string TriggerComponent::getScene()
	{
		if (auto scene = getEntity()->getScene()->getScenes()->getScene(sceneId); scene != nullptr) {
			return scene->getName();
		}

		return std::string();
	}

	void TriggerComponent::setEntrance(const std::string& name)
	{
		if (auto scene = getEntity()->getScene()->getScenes()->getScene(sceneId); scene != nullptr) {
			auto& entries = scene->getEntities("entries");

			auto entry = std::find_if(entries.begin(), entries.end(), 
				[&] (std::unique_ptr<Entity>& entity) { 
					return name == entity->getName(); 
				}
			);

			if (entry != entries.end()) {
				entranceId = entry->get()->getID();
			}
		}
	}

	std::string TriggerComponent::getEntrance()
	{
		if (auto scene = getEntity()->getScene()->getScenes()->getScene(sceneId); scene != nullptr) {
			auto entry = scene->getEntity(entranceId);

			if (entry) {
				return entry->getName();
			}
		}

		return std::string();
	}

	std::string TriggerComponent::enumToString(size_t en)
	{
		static std::string names[]{ "None", "Change scene", "Overlaping" };
		return names[en];
	}

	TriggerComponent::Type TriggerComponent::uintToEnum(size_t s) {
		switch(s) {
			case 0: return None;
			case 1: return ChangeScene;
			case 2: return Overlaping;
		}
		return None;
	}

	void TriggerComponent::loadFromConfig(Json& config) {
		Component::loadFromConfig(config);
		
		if (config.find("shapeType") != config.end()) {
			_triggerShape = static_cast<Shape>(config["shapeType"]);
		}

		if (_triggerShape == Shape::Circle) {
			_radius = config["radius"];
		}
		else if (_triggerShape == Shape::Rectangle) {
			_rectSize.x = config["rectSize"]["x"];
			_rectSize.y = config["rectSize"]["y"];
		}

		type = uintToEnum(config["type"].get<size_t>());

		if (type == ChangeScene) {
			sceneId = config["sceneId"];
			entranceId = config["enranceId"];

			if (config.find("fadeTime") != config.end()) {
				_fadeTime = config["fadeTime"];
				_changingSceneWithFade = true;
			}
		}
	}

	void TriggerComponent::saveToConfig(Json& config) const {
		Component::saveToConfig(config);

		config["shapeType"] = static_cast<size_t>(_triggerShape);

		if (_triggerShape == Shape::Circle) {
			config["radius"] = _radius;
		}
		else if (_triggerShape == Shape::Rectangle){
			config["rectSize"]["x"] = _rectSize.x;
			config["rectSize"]["y"] = _rectSize.y;
		}

		config["type"] = static_cast<size_t>(type);

		if(type == ChangeScene) {
			config["sceneId"] = sceneId;
			config["enranceId"] = entranceId;

			if (_changingSceneWithFade) {
				config["fadeTime"] = _fadeTime;
			}
		}
	}

	void TriggerComponent::setShapeType(Shape shape) {
		_triggerShape = shape;
	}

	const TriggerComponent::Shape& TriggerComponent::getShapeType() {
		return _triggerShape;
	}

    void TriggerComponent::update(ScenesManager& scenes, float deltaTime) {
    	
    	if(!scenes.isGameRunning()) return; 

        auto* player = getEntity()->getScene()->getPlayer();
        if(player == nullptr) 
			return;

		if (type == TriggerComponent::Overlaping) {
			if (checkForTrigger(player->getPosition())) {
				if (!_isPlayerInside) {
					if (_enterCallback.valid())
						_enterCallback(getEntity());

					_isPlayerInside = true;
				}

				if (_insideCallback.valid())
					_insideCallback(getEntity());
			}
			else {
				if (_isPlayerInside) {
					if (_leaveCallback.valid())
						_leaveCallback(getEntity());

					_isPlayerInside = false;
				}
			}
		}
        // Active trigger after [Space]
        else if(_input.isPressed(Keyboard::Space)) {
            // Is player inside trigger
            if(checkForTrigger(player->getPosition())) {
                // Action for ChangeScene trigger
                if(type == TriggerComponent::ChangeScene) {
                    // Change scene after teleport

					if (_changingSceneWithFade)
						detail::globalPtr<World>->fadeIntoScene(sceneId, _fadeTime);
					else
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

    void TriggerComponent::initScript(ScriptClass<Entity>& entity, Script& script) {
        auto object = script.newClass<TriggerComponent>("TriggerComponent", "World");

		// Main
		object.set("setType", &TriggerComponent::setType);
		object.set("getType", &TriggerComponent::getType);


		// change scene
		object.set("setScene", &TriggerComponent::setScene);
		object.set("getScene", &TriggerComponent::getScene);

		object.set("setEntrance", &TriggerComponent::setEntrance);
		object.set("getEntrance", &TriggerComponent::getEntrance);


		// shape
		object.set("setShapeType", &TriggerComponent::setShapeType);
		object.set("getShapeType", &TriggerComponent::getShapeType);
		
		object.set("setRadius", &TriggerComponent::setRadius);
		object.set("getRadius", &TriggerComponent::getRadius);

		object.set("setRectSize", &TriggerComponent::setRectSize);
		object.set("setRectSize", &TriggerComponent::getRectSize);

		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

		// Entity
		entity.set("trigger", &Entity::getComponentAs<TriggerComponent>);
		entity.set("addTriggerComponent", [&] (Entity& e) {return (TriggerComponent*)e.addComponent<TriggerComponent>(); });

		// overlapping

		// enter callback
		entity.setProperty("onEnter", [](){}, [] (Entity &obj, sol::function func) {
			obj.getComponentAs<TriggerComponent>()->_enterCallback = func;
		});

		// inside callback
		entity.setProperty("onInside", [](){}, [] (Entity &obj, sol::function func) {
			obj.getComponentAs<TriggerComponent>()->_insideCallback = func;
		});

		// leave callback
		entity.setProperty("onLeave", [](){}, [] (Entity &obj, sol::function func) {
			obj.getComponentAs<TriggerComponent>()->_leaveCallback = func;
		});

        object.init();
    }

    void TriggerComponent::renderHeader(ScenesManager& scenes, Entity* object) {
    	if (ImGui::CollapsingHeader("Trigger##trigger_component"))
        {
			if (ImGui::RadioButton("Circle", _triggerShape == Shape::Circle))
			{
				_triggerShape = Shape::Circle;
			}
			
			ImGui::SameLine();

			if (ImGui::RadioButton("Rectangle", _triggerShape == Shape::Rectangle))
			{
				_triggerShape = Shape::Rectangle;
			}

			if (_triggerShape == Shape::Circle)
			{
				// Set radius
				ImGui::DragFloat<ImGui::CopyPaste>("Radius", _radius);
			}
			else if (_triggerShape == Shape::Rectangle)
			{
				// set rect size
				ImGui::DragVec2<ImGui::CopyPaste>("Size", _rectSize);
			}

			ImGui::Separator();

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
				if (ImGui::Selectable(enumToString(Type::Overlaping).c_str(), type == Type::Overlaping))
				{
					type = Type::Overlaping;
					_isPlayerInside = false;
				}
				ImGui::EndCombo();
			}

            // Options for "Change scene" type
            if (type == Type::ChangeScene)
            {
				ImGui::Checkbox("Change scene with fade", &_changingSceneWithFade);
				
				if (_changingSceneWithFade)
				{
					ImGui::SliderFloat("Fade time", &_fadeTime, 0.1f, 2.f, "%.2f sec");
				}

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
