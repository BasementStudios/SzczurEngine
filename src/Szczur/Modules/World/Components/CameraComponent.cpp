#include "CameraComponent.hpp"

#include "Szczur/Modules/Input/InputManager.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Listener/Listener.hpp"

namespace rat {
	CameraComponent::CameraComponent(Entity* parent) :
	Component { parent, fnv1a_64("CameraComponent"), "CameraComponent"}, 
	_listener(detail::globalPtr<Listener>) {
		
	}

	void CameraComponent::processEvents(InputManager& input) {
        if(_noMove) return;

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
		if(!_stickTo || _stickToX) {
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
		_stickedToX = false;
	}

	void CameraComponent::stickTo(float x) {
		_stickTo = nullptr;
		_stickToX = x;
		_stickedToX = true;
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
		if(auto& var = config["type"]; !var.is_null()) _type = size_tToEnumType(var);
		if(auto& var = config["sticked_id"]; !var.is_null()) _stickedID = var;
        if(auto& var = config["no_move"]; !var.is_null()) _noMove = var;
		if(auto& var = config["smoothness"]; !var.is_null() && _type == Smooth) _smoothness = var;
		if(auto& var = config["linear"]; !var.is_null() && _type == Linear) _linear = var;


	}

	void CameraComponent::saveToConfig(Json& config) const {
		Component::saveToConfig(config);
		config["velocity"] = _velocity;
		config["locked"] = _locked;
		if(_type == Smooth)
			config["smoothness"] = _smoothness;
		else if(_type == Linear)
			config["linear"] = _linear;
		config["type"] = enumTypeToSize_t();
		config["limit"]["left"] = _limit.left;
		config["limit"]["right"] = _limit.right;
		config["limitedRange"] = _limitedRange;
		config["sticked_id"] = (_stickTo==nullptr ? 0 : _stickTo->getID());
        config["no_move"] = _noMove;
	}

	void CameraComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Camera##camera_component")) {

			// Set velocity
			float velocity = getVelocity();
			ImGui::DragFloat("Velocity##camera_component", &velocity);
			setVelocity(velocity);

			if(ImGui::BeginCombo("Type", enumTypeToString().c_str())) {
				if(ImGui::Selectable("None", _type == None))
					_type = None;
				if(ImGui::Selectable("Smooth", _type == Smooth))
					_type = Smooth;
				if(ImGui::Selectable("Linear", _type == Linear))
					_type = Linear;
				ImGui::EndCombo();
			}

			if(_type == Smooth)
				ImGui::DragFloat("Smoothness##camera_component", &_smoothness, 0.05f, 1.f, 50.f);
			else if(_type == Linear)
				ImGui::DragFloat("Linear##camera_component", &_linear, 0.05f, 1.f, 50.f);


			// Set lock
			bool locked = getLock();
			ImGui::Checkbox("Locked##camera_component", &locked);
			setLock(locked);
			ImGui::Checkbox("No move##camera_component", &_noMove);
			
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
			if(_stickTo != nullptr) {
                ImGui::SameLine();
                if(ImGui::Button("Unstick##camera_component")) {
                    stickTo(nullptr);
                }
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
		auto* entity = getEntity();
		// auto* player = getEntity()->getScene()->getPlayer();
        // if(player == nullptr) return;
 
        if(_stickedID != 0) {
            _stickTo = getEntity()->getScene()->getEntity(_stickedID);
            _stickedID = 0;
        }
        
		if(_stickTo) {
			auto curPos = getEntity()->getPosition();
			curPos.x = _stickTo->getPosition().x;
			getEntity()->setPosition(curPos);
		}
		else if(_stickedToX) {
			auto curPos = getEntity()->getPosition();
			curPos.x = _stickToX;
			getEntity()->setPosition(curPos);
		}
		if(_limitedRange) {
			auto position = entity->getPosition();
			if(position.x > _limit.right)
				position.x = _limit.right;
			else if(position.x < _limit.left)
				position.x = _limit.left;
			entity->setPosition(position);
		}

		auto curPos = entity->getPosition();
		_listener->setPosition(curPos.x, curPos.y, curPos.z);
    }

	void CameraComponent::setSmoothness(float smoothness) {
		_smoothness = smoothness;
	}

    float CameraComponent::getSmoothness() const {
		return _smoothness;
	}

	sf3d::View CameraComponent::getRecalculatedView(sf3d::View baseView) {
		auto* entity = getEntity();
		if(_type == None) {
			baseView.setCenter(entity->getPosition());
			baseView.setRotation(entity->getRotation());
		}
		else if(_type == Smooth) {
			if(_smoothness >= 1.f) {
				auto delta = entity->getPosition() - baseView.getCenter();
				auto deltaRotation = entity->getRotation() - baseView.getRotation();
				baseView.move(delta / _smoothness);
				baseView.rotate(deltaRotation / _smoothness);

			}
		}
		else if(_type == Linear) {
			auto position = entity->getPosition();
			auto direction = position - baseView.getCenter();
			float length = glm::sqrt(direction.x*direction.x + direction.y*direction.y);
			direction /= length;
			if(length <= _linear)
				baseView.setCenter(position);
			else
				baseView.move(direction*_linear);
			baseView.setRotation(entity->getRotation());
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
		object.set("stickTo", sol::overload(
			sol::resolve<void(Entity*)>(&CameraComponent::stickTo),
			sol::resolve<void(float)>(&CameraComponent::stickTo)
		));
		object.set("stickToPlayer", &CameraComponent::stickToPlayer);

		object.set("setLock", &CameraComponent::setLock);
		object.set("getLock", &CameraComponent::getLock);
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));
		

		// Entity
		entity.set("addCameraComponent", [&](Entity& e){return (CameraComponent*)e.addComponent<CameraComponent>();});
		entity.set("camera", &Entity::getComponentAs<CameraComponent>);

		object.init();

	}
	std::string CameraComponent::enumTypeToString() const {
		switch(_type) {
			case None: return "None";
			case Smooth: return "Smooth";
			case Linear: return "Linear";
		}
	}
	size_t CameraComponent::enumTypeToSize_t() const {
		return static_cast<size_t>(_type);
	}

	CameraComponent::Type CameraComponent::size_tToEnumType(size_t type) const {
		switch(type) {
			case static_cast<size_t>(Smooth) : return Smooth;
				case static_cast<size_t>(Linear) : return Linear;
		}
		return None;
	}
}