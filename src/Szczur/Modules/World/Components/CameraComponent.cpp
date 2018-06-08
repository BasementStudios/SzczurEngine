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
		if(!_stickTo) {
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

	void CameraComponent::stickTo(Entity* entity) {
		_stickTo = entity;
	}

	
	void CameraComponent::stickToPlayer() {
		_stickTo = getEntity()->getScene()->getPlayer();
	}

	void CameraComponent::loadFromConfig(Json& config) {
		Component::loadFromConfig(config);
		if(auto& var = config["velocity"]; !var.is_null()) _velocity = var;
		if(auto& var = config["locked"]; !var.is_null()) _locked = var;
		if(auto& var = config["smoothness"]; !var.is_null()) _smoothness = var;

		if(auto& var = config["limit"]["left"]; !var.is_null()) _limit.left = var;
		if(auto& var = config["limit"]["right"]; !var.is_null()) _limit.right = var;
		if(auto& var = config["limitedRange"]; !var.is_null()) _limitedRange = var;
	}

	void CameraComponent::saveToConfig(Json& config) const {
		Component::saveToConfig(config);
		config["velocity"] = _velocity;
		config["locked"] = _locked;
		config["smoothness"] = _smoothness;
		config["limit"]["left"] = _limit.left;
		config["limit"]["right"] = _limit.right;
		config["limitedRange"] = _limitedRange;
	}

	void CameraComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Camera##camera_component")) {

			// Set velocity
			float velocity = getVelocity();
			ImGui::DragFloat("Velocity##camera_component", &velocity);
			setVelocity(velocity);

			ImGui::DragFloat("Smoothness##camera_component", &_smoothness, 0.05f, 1.f, 50.f);

			// Set lock
			bool locked = getLock();
			ImGui::Checkbox("Locked##camera_component", &locked);
			setLock(locked);
			
			// Set lock on player
			ImGui::Text(
				(
					std::string{"Stick To: "} + 
					((_stickTo)?_stickTo->getName():std::string{"None"})
				).c_str()
			);

			if(ImGui::Button("Stick To Player##camera_component")) {
				stickToPlayer();
			}

			ImGui::Checkbox("Limited Range##camera_component", &_limitedRange);
			if(_limitedRange) {
				ImGui::DragFloat2("Limit", (float*)&_limit);
				if(_limit.right < _limit.left)
					_limit.left = _limit.right;
			}
		}
	}

    void CameraComponent::update(ScenesManager& scenes, float deltaTime) {
		auto* player = getEntity()->getScene()->getPlayer();
		if(player == nullptr) return;

		if(_stickTo) {
			auto curPos = getEntity()->getPosition();
			curPos.x = _stickTo->getPosition().x;
			getEntity()->setPosition(curPos);
		}

		
    }

	void CameraComponent::setSmoothness(float smoothness) {
		_smoothness = smoothness;
	}

    float CameraComponent::getSmoothness() const {
		return _smoothness;
	}

	sf3d::View CameraComponent::getRecalculatedView(sf3d::View baseView) {
		if(_smoothness >= 1.f) {
			auto* entity = getEntity();

			if(_limitedRange) {
				float newX = entity->getPosition().x;
				if(newX > _limit.right)
					newX = _limit.right;
				else if(newX < _limit.left)
					newX = _limit.left;
				entity->setPosition({
					newX,
					entity->getPosition().y,
					entity->getPosition().z
				});
			}

			auto delta = entity->getPosition() - baseView.getCenter();
			auto deltaRotation = entity->getRotation() - baseView.getRotation();
			baseView.move(delta/_smoothness);
			baseView.rotate(deltaRotation/_smoothness);

		}
		return baseView;
	}

	void CameraComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<CameraComponent>("CameraComponent", "World");

		// Main
		object.set("setVelocity", &CameraComponent::setVelocity);
		object.set("getVelocity", &CameraComponent::getVelocity);
		object.set("setSmoothness", &CameraComponent::setSmoothness);
		object.set("getSmoothness", &CameraComponent::getSmoothness);
		object.set("stickTo", &CameraComponent::stickTo);

		object.set("setLock", &CameraComponent::setLock);
		object.set("getLock", &CameraComponent::getLock);
		object.set("strickToPlayer", &CameraComponent::stickToPlayer);
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));
		

		// Entity
		entity.set("addCameraComponent", [&](Entity& e){return (CameraComponent*)e.addComponent<CameraComponent>();});
		entity.set("camera", &Entity::getComponentAs<CameraComponent>);

		object.init();

	}
}