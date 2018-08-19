#include "ColliderComponent.hpp"

#include <imgui.h>

#include "Szczur/Utility/ImGuiTweaks.hpp"
#include "Szczur/Modules/World/Entity.hpp"
#include "Szczur/Modules/World/Scene.hpp"

namespace rat
{

ColliderComponent::ColliderComponent(Entity* parent)
	: Component{ parent, fnv1a_64("ColliderComponent"), "ColliderComponent" }
{
}

std::unique_ptr<Component> ColliderComponent::copy(Entity* newParent) const
{
	auto ptr = std::make_unique<ColliderComponent>(*this);
	ptr->setEntity(newParent);
	return ptr;
}

void ColliderComponent::loadFromConfig(nlohmann::json& config)
{
	Component::loadFromConfig(config);

	_isDynamic = config["dynamic"];

	if (auto it = config.find("circleRadius"); it != config.end())
	{
		_circleCollider = true;
		_circleRadius = it.value();
	}

	if (auto it = config.find("boxSize"); it != config.end())
	{
		_boxCollider = true;
		_boxSize.x = it.value()["x"];
		_boxSize.y = it.value()["y"];
	}
}

void ColliderComponent::saveToConfig(nlohmann::json& config) const
{
	Component::saveToConfig(config);

	config["dynamic"] = _isDynamic;

	if (_circleCollider)
	{
		config["circleRadius"] = _circleRadius;
	}

	if (_boxCollider)
	{
		config["boxSize"]["x"] = _boxSize.x;
		config["boxSize"]["y"] = _boxSize.y;
	}
}

void ColliderComponent::renderHeader(ScenesManager& scenes, Entity* object)
{
	if (ImGui::CollapsingHeader("Collider##collider_component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Dynamic", &_isDynamic);

		ImGui::Separator();

		ImGui::Checkbox("Box collider", &_boxCollider);
		
		if (_boxCollider)
		{
			ImGui::DragVec2<ImGui::CopyPaste>("Size", _boxSize);
		}

		ImGui::Separator();

		ImGui::Checkbox("Circle collider", &_circleCollider);

		if (_circleCollider)
		{
			ImGui::DragFloat<ImGui::CopyPaste>("Radius", _circleRadius);
		}
	}
}

void ColliderComponent::initScript(ScriptClass<Entity>& entity, Script& script)
{
	auto object = script.newClass<ColliderComponent>("ColliderComponent", "World");

	// Main
	object.set("move", &ColliderComponent::move);

	object.set("setCircleCollider", &ColliderComponent::setCircleCollider);
	object.set("isCircleCollider", &ColliderComponent::isCircleCollider);
	object.set("setCircleRadius", &ColliderComponent::setCircleRadius);
	object.set("getCircleRadius", &ColliderComponent::getCircleRadius);

	object.set("setBoxCollider", &ColliderComponent::setBoxCollider);
	object.set("isBoxCollider", &ColliderComponent::isBoxCollider);
	object.set("setBoxSize", &ColliderComponent::setBoxSize);
	object.set("getBoxSize", &ColliderComponent::getBoxSize);

	object.set("isDynamic", &ColliderComponent::isDynamic);
	object.set("setDynamic", &ColliderComponent::setDynamic);

	object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

	// Entity
	entity.set("addColidderComponent", [&] (Entity& e) {return (ColliderComponent*)e.addComponent<ColliderComponent>(); });
	entity.set("collider", &Entity::getComponentAs<ColliderComponent>);

	object.init();
}

void ColliderComponent::move(float x, float y, float z)
{
	glm::vec3 velocity = { x, y, z };

	auto thisPos = getEntity()->getPosition() + glm::vec3(x, 0, z);
	auto thisAABB = getAABB(getEntity()->getPosition() + glm::vec3(x, 0, z), _boxSize);

	auto collisionCheck = [&] (Entity* entity) {

		if (entity == getEntity())
			return;

		if (!entity->isActive())
			return;

		// if entity has collider
		if (entity->hasComponent<ColliderComponent>())
		{
			auto comp = entity->getComponentAs<ColliderComponent>();

			if (this->_boxCollider && comp->isBoxCollider())// if both have box collider
			{
				auto entityRect = getAABB(entity->getPosition(), comp->getBoxSize());

				AABB md;

				// calc Minkowiski's difference
				entityRect.minkowskiDifference(thisAABB, md);

				// check if is collision
				if (md.isCollision())
				{
					// get closest point on `other`
					auto newPos = md.closestPointOnBoundsToPoint(glm::vec2());

					velocity.x += newPos.x;
					velocity.z += newPos.y;
				}
			}
			else if (this->_circleCollider && comp->isCircleCollider()) // if both have circle collider
			{
				// calc delta between objects
				auto delta = thisPos - entity->getPosition();

				// calc distance
				auto distance = sqrt(pow(delta.x, 2) + pow(delta.z, 2));

				// add radius both circles
				auto radius = _circleRadius + comp->getCircleRadius();

				// collision check
				if (distance < radius)
				{
					// calc angle
					auto angle = atan2(delta.z, delta.x);

					if (comp->isDynamic())
					{
						// move the other
						entity->move({ -cos(angle) * (radius - distance), 0.f, -sin(angle) * (radius - distance) });
					}
					else
					{
						// stop itself
						velocity.x += cos(angle) * (radius - distance);
						velocity.z += sin(angle) * (radius - distance);
					}
				}
			}
		}
	};

	// scan every entity on path
	auto& entities = this->getEntity()->getScene()->getEntities("path");
	for (auto& entity : entities)
	{
		collisionCheck(entity.get());
	}

	collisionCheck(getEntity()->getScene()->getPlayer());

	getEntity()->move(velocity);
}

AABB ColliderComponent::getAABB(const glm::vec3& pos, const glm::vec2& size)
{
	return AABB(glm::vec2(pos.x, pos.z), glm::vec2(size.x, size.y) / 2.f);
}

}
