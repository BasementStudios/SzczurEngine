#include "CameraComponent.hpp"

#include <nlohmann/json.hpp>

#include "Szczur/Modules/Input/InputManager.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Utility/ImGuiTweaks.hpp"
#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Listener/Listener.hpp"

namespace rat {
	CameraComponent::CameraComponent(Entity* parent) :
	Component { parent, fnv1a_64("CameraComponent"), "CameraComponent"}, 
	_listener(detail::globalPtr<Listener>) {
		sf3d::Camera::move({1000.f,500.f,2000.f}); // @todo . On ratio change there should be update to the camera
		sf3d::Camera::setRenderDistance(300.f);
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

    bool CameraComponent::isNoMove() const {
    	return _noMove;
    }

    void CameraComponent::setNoMove(bool flag) {
    	_noMove = flag;
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

	void CameraComponent::loadFromConfig(nlohmann::json& config) {
		Component::loadFromConfig(config);
		if(auto& var = config["velocity"]; !var.is_null()) _velocity = var;
		if(auto& var = config["locked"]; !var.is_null()) _locked = var;
		if(auto& var = config["smoothness"]; !var.is_null()) _smoothness = var;

		if(auto& var = config["limit"]["left"]; !var.is_null()) _limit.left = var;
		if(auto& var = config["limit"]["right"]; !var.is_null()) _limit.right = var;
		if(auto& var = config["limitedRange"]; !var.is_null()) _limitedRange = var;
		if(auto& var = config["type"]; !var.is_null()) _moveType = static_cast<MoveType>(var);
        if(auto& var = config["no_move"]; !var.is_null()) _noMove = var;
		if(auto& var = config["smoothness"]; !var.is_null() && _moveType == Smooth) _smoothness = var;
		if(auto& var = config["linear"]; !var.is_null() && _moveType == Linear) _linear = var;

		if(auto& var = config["targetingType"]; !var.is_null()) _targetingType = static_cast<TargetingType>(var);
		if(_targetingType == Forwarded)
			if(auto& var = config["forwarded"]; !var.is_null()) _forwarded = var;

		if(auto& var = config["stickedToX"]; !var.is_null()) {
			_stickToX = var;
			_stickedToX = true;
		}
		else if(auto& var = config["stickedToID"]; !var.is_null()) {
			_stickedID = var;
		}


	}

	void CameraComponent::saveToConfig(nlohmann::json& config) const {
		Component::saveToConfig(config);
		config["velocity"] = _velocity;
		config["locked"] = _locked;
		if(_moveType == Smooth)
			config["smoothness"] = _smoothness;
		else if(_moveType == Linear)
			config["linear"] = _linear;
		config["type"] = static_cast<size_t>(_moveType);
		config["limit"]["left"] = _limit.left;
		config["limit"]["right"] = _limit.right;
		config["limitedRange"] = _limitedRange;
        config["no_move"] = _noMove;

		config["targetingType"] = static_cast<size_t>(_targetingType);
		if(_targetingType == Forwarded)
			config["forwarded"] = _forwarded;

		if(_stickedToX)
			config["stickedToX"] = _stickToX;
		else if(_stickTo)
			config["stickedToID"] = _stickTo->getID();
	}

	void CameraComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Camera##camera_component")) {

			// Set velocity
			ImGui::DragFloat<ImGui::CopyPaste>("Velocity##camera_component", _velocity);

			ImGui::Spacing();

			if(ImGui::BeginCombo("MoveType", enumToString(_moveType).c_str())) {
				if(ImGui::Selectable("None", _moveType == None))
					_moveType = None;
				if(ImGui::Selectable("Smooth", _moveType == Smooth))
					_moveType = Smooth;
				if(ImGui::Selectable("Linear", _moveType == Linear))
					_moveType = Linear;
				ImGui::EndCombo();
			}

			if(_moveType == Smooth)
				ImGui::DragFloat<ImGui::CopyPaste>("Smoothness##camera_component", _smoothness, 0.05f, 1.f, 50.f);
			else if(_moveType == Linear)
				ImGui::DragFloat<ImGui::CopyPaste>("Linear##camera_component", _linear, 0.05f, 1.f, 50.f);

			ImGui::Spacing();

			if(ImGui::BeginCombo("Targeting Type", enumToString(_targetingType).c_str())) {
				if(ImGui::Selectable("Precise", _targetingType == Precise))
					_targetingType = Precise;
				if(ImGui::Selectable("Forwarded", _targetingType == Forwarded))
					_targetingType = Forwarded;
				ImGui::EndCombo();
			}

			if(_targetingType == Forwarded) {
				ImGui::DragFloat<ImGui::CopyPaste>("Forwarded##camera_component", _forwarded);
			}

			ImGui::Spacing();

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

	void CameraComponent::updateCamera() {
		auto* entity = getEntity();
		if (_moveType == None) {
			_virtualPosition = entity->getPosition();
			this->setRotation(entity->getRotation());
		}
		else if (_moveType == Smooth) {
			if(_smoothness >= 1.f) {
				auto delta = entity->getPosition() - _virtualPosition;
				auto deltaRotation = entity->getRotation() - this->getRotation();

				_virtualPosition += delta / _smoothness;
				this->rotate(deltaRotation / _smoothness);
			}
		}
		else if (_moveType == Linear) {
			auto position = entity->getPosition();
			auto direction = position - _virtualPosition;
			float length = glm::sqrt(direction.x * direction.x + direction.y * direction.y);
			direction /= length;
			if(length <= _linear)
				_virtualPosition = position;
			else
				_virtualPosition += direction * _linear;
			this->setRotation(entity->getRotation());
		}

		if(_targetingType == Precise) {
			this->setPosition(_virtualPosition);
		}
		if(_targetingType == Forwarded) {
			auto delta = entity->getPosition() - _virtualPosition;

			auto position = entity->getPosition() + delta * _forwarded;
			if(_limitedRange) {
				if(position.x > _limit.right)
					position.x = _limit.right;
				else if(position.x < _limit.left)
					position.x = _limit.left;
			}

			this->setPosition(position);
		}
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
	std::string CameraComponent::enumToString(MoveType type) {
		switch (type) {
			case Smooth: return "Smooth";
			case Linear: return "Linear";
			default: return "None";
		}
	}

	std::string CameraComponent::enumToString(TargetingType type) {
		switch (type) {
			case Forwarded: return "Forwarded";
			default: return "Precise";
		}
	}
}
