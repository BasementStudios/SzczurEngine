#include "CameraComponent.hpp"

#include "Szczur/Modules/Input/InputManager.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Modules/Script/Script.hpp"


namespace rat {
	CameraComponent::CameraComponent(Entity* parent) :
	Component { parent, fnv1a_64("CameraComponent"), "CameraComponent"} {

	}

	void CameraComponent::processEvents(InputManager& input) {
		float velocity = _velocity;
		auto* object = getEntity();
		if(input.isKept(Keyboard::LShift)) {
			velocity = 200.f;
		}
		auto rotation = object->getRotation();

		if(input.isKept(Keyboard::W)) {
			object->move({
				velocity * glm::sin(glm::radians(rotation.y)),
				0.f,
				-velocity * glm::cos(glm::radians(rotation.y))
			});
		}
		if(input.isKept(Keyboard::S))
			object->move({
				-velocity * glm::sin(glm::radians(rotation.y)),
				0.f,
				velocity * glm::cos(glm::radians(rotation.y))
			});
		if(!_stickToPlayer) {
			if(input.isKept(Keyboard::D)) {
				object->move(glm::vec3{
					velocity * glm::cos(glm::radians(rotation.y)),
					0.f,
					velocity * glm::sin(glm::radians(rotation.y))
				});
			}
			if(input.isKept(Keyboard::A)) {
				object->move(glm::vec3{
					-velocity * glm::cos(glm::radians(rotation.y)),
					0.f,
					-velocity * glm::sin(glm::radians(rotation.y))
				});
			}
		}
		if(input.isKept(Keyboard::Space))
			object->move({0.f, velocity, 0.f});
		if(input.isKept(Keyboard::LControl))
			object->move({0.f, -velocity, 0.f});
		if(_rotating) {
			auto mouse = input.getMousePosition();
			object->rotate({
				(mouse.y - _previousMouse.y)/10.f,
				(mouse.x - _previousMouse.x)/10.f,
				0.f
			});
			_previousMouse = mouse;
		}
		if(input.isPressed(Mouse::Right)) {
			_rotating = true;
			_previousMouse = input.getMousePosition();
		}
		if(input.isReleased(Mouse::Right)) {
			_rotating = false;
		}
	}

	std::unique_ptr<Component> CameraComponent::copy(Entity* newParent) const {
		auto ptr = std::make_unique<CameraComponent>(*this);

		ptr->setEntity(newParent);

		return ptr;
	}

	void CameraComponent::setVelocity(float velocity) {
		_velocity = velocity;
	}
	float CameraComponent::getVelocity() const {
		return _velocity;
	}

	void CameraComponent::setLock(bool lock) {
		_locked = lock;
	}
	bool CameraComponent::getLock() const {
		return _locked;
	}

	
	void CameraComponent::setStickToPlayer(bool value) {
		_stickToPlayer = value;
	}
	bool CameraComponent::getStickToPlayer() const {
		return _stickToPlayer;
	}

	void CameraComponent::loadFromConfig(Json& config) {
		Component::loadFromConfig(config);
		if(auto& var = config["velocity"]; !var.is_null()) _velocity = var;
		if(auto& var = config["locked"]; !var.is_null()) _locked = var;
		if(auto& var = config["stick_to_player"]; !var.is_null()) _stickToPlayer = var;
	}

	void CameraComponent::saveToConfig(Json& config) const {
		Component::saveToConfig(config);
		config["velocity"] = _velocity;
		config["locked"] = _locked;
		config["stick_to_player"] = _stickToPlayer;
	}

	void CameraComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Camera##camera_component")) {

			// Set velocity
			float velocity = getVelocity();
			ImGui::DragFloat("Velocity##camera_component", &velocity);
			setVelocity(velocity);

			// Set lock
			bool locked = getLock();
			ImGui::Checkbox("Locked##camera_component", &locked);
			setLock(locked);
			
			// Set lock on player
			bool stickToPlayer = getStickToPlayer();
			ImGui::Checkbox("Stick To Player##camera_component", &stickToPlayer);
			setStickToPlayer(stickToPlayer);
		}
	}

    void CameraComponent::update(ScenesManager& scenes, float deltaTime) {
		auto* player = getEntity()->getScene()->getPlayer();
		if(player == nullptr) return;

		if(getStickToPlayer()) {
			auto curPos = getEntity()->getPosition();
			curPos.x = player->getPosition().x;
			getEntity()->setPosition(curPos);
		}
    }

	void CameraComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<CameraComponent>("CameraComponent", "World");

		// Main
		object.set("setVelocity", &CameraComponent::setVelocity);
		object.set("getVelocity", &CameraComponent::getVelocity);
		object.set("setLock", &CameraComponent::setLock);
		object.set("getLock", &CameraComponent::getLock);
		object.set("setStickToPlayer", &CameraComponent::setStickToPlayer);
		object.set("getStickToPlayer", &CameraComponent::getStickToPlayer);
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

		// Entity
		entity.set("addCameraComponent", [&](Entity& e){return (CameraComponent*)e.addComponent<CameraComponent>();});
		entity.set("camera", &Entity::getComponentAs<CameraComponent>);

		object.init();

	}
}