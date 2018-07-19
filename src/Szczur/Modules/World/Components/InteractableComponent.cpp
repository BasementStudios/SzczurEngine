#include "InteractableComponent.hpp"

#include <nlohmann/json.hpp>

#include "Szczur/Utility/ImGuiTweaks.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "../Scene.hpp"
#include "../Entity.hpp"
#include "../ScenesManager.hpp"

namespace rat {
	InteractableComponent::InteractableComponent(Entity* parent) :
		Component { parent, fnv1a_64("InteractableComponent"), "InteractableComponent"}, 
		_input(detail::globalPtr<Input>->getManager()) {
	}

	void InteractableComponent::callback()
	{
		try
		{
			if (_interactionCallback.valid())
				_interactionCallback(getEntity());
		}
		catch (sol::error err)
		{
			LOG_EXCEPTION(err);
		}
	}

	void InteractableComponent::setDistance(float distance) {
		_distance = distance;
	}
	float InteractableComponent::getDistance() const {
		return _distance;
	}

	void InteractableComponent::setHeight(float height) {
		_height = height;
	}
	float InteractableComponent::getHeight() const {
		return _height;
	}

	bool InteractableComponent::checkForInteraction(const glm::vec3& position) const {
		auto* entity = getEntity();
		auto delta = position - entity->getPosition();
		if(delta.x*delta.x + delta.z*delta.z <= _distance*_distance)
			return true;
		return false;
	}

	std::unique_ptr<Component> InteractableComponent::copy(Entity* newParent) const {
		auto ptr = std::make_unique<InteractableComponent>(*this);

		ptr->setEntity(newParent);

		return ptr;
	}

	void InteractableComponent::loadFromConfig(nlohmann::json& config) {
		Component::loadFromConfig(config);
		_distance = config["distance"];
	}

	void InteractableComponent::saveToConfig(nlohmann::json& config) const {
		Component::saveToConfig(config);
		config["distance"] = _distance;
	}


	void InteractableComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<InteractableComponent>("InteractableComponent", "World");

		// Main
		object.set("setDistance", &InteractableComponent::setDistance);
		object.set("getDistance", &InteractableComponent::getDistance);
		object.set("setHeight", &InteractableComponent::setHeight);
		object.set("getHeight", &InteractableComponent::getHeight);
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

		// Entity
		entity.set("interactable", &Entity::getComponentAs<InteractableComponent>);
		entity.set("addInteractableComponent", [&](Entity& e){return (InteractableComponent*)e.addComponent<InteractableComponent>();});
		entity.setProperty("onInteraction", [](){}, [](Entity &obj, sol::function func) {
			obj.getComponentAs<InteractableComponent>()->_interactionCallback = func;
		});

		object.init();
	}

	void InteractableComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Interactable")) {

			// Set radius for circle interaction
			ImGui::DragFloat<ImGui::CopyPaste>("Radius", _distance);

			// Set height of circle interaction 
			ImGui::DragFloat<ImGui::CopyPaste>("Height", _height);
		}
	}

	void InteractableComponent::update(ScenesManager& scenes, float deltaTime) {
		auto* player = getEntity()->getScene()->getPlayer();
		if(player == nullptr) return;

		if(_input.isReleased(Keyboard::LShift)) {
			if(checkForInteraction(player->getPosition())) {
				callback();
			}
		}
	}
}
