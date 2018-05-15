#include "CameraComponent.hpp"

#include "Szczur/Modules/Input/InputManager.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"


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

	void CameraComponent::loadFromConfig(const Json& config) {
		Component::loadFromConfig(config);
		_velocity = config["velocity"];
		_locked = config["locked"];
	}

	void CameraComponent::saveToConfig(Json& config) const {
		Component::saveToConfig(config);
		config["velocity"] = _velocity;
		config["locked"] = _locked;
	}
}