#include "ColliderComponent.hpp"

#include <ImGui/imgui.h>

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

void ColliderComponent::loadFromConfig(Json& config)
{
	Component::loadFromConfig(config);

	_isDynamic = config["dynamic"];

	if (config.find("circleRadius") != config.end())
	{
		_cicleCollider = true;
		_circleRadius = config["circleRadius"];
	}

	if (config.find("boxSize") != config.end())
	{
		_boxCollider = true;
		_boxSize.x = config["boxSize"]["x"];
		_boxSize.y = config["boxSize"]["y"];
	}
}

void ColliderComponent::saveToConfig(Json& config) const
{
	Component::saveToConfig(config);

	config["dynamic"] = _isDynamic;

	if (_cicleCollider)
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
			ImGui::DragFloat2("Size", &_boxSize[0]);
		}

		ImGui::Separator();

		ImGui::Checkbox("Circle collider", &_cicleCollider);

		if (_cicleCollider)
		{
			ImGui::DragFloat("Radius", &_circleRadius);
		}
	}
}

void ColliderComponent::initScript(ScriptClass<Entity>& entity, Script& script)
{
	auto object = script.newClass<ColliderComponent>("ColliderComponent", "World");

	object.set("move", &ColliderComponent::move);

	// Entity
	entity.set("addColidderComponent", [&] (Entity& e) {return (ColliderComponent*)e.addComponent<ColliderComponent>(); });
	entity.set("collider", &Entity::getComponentAs<ColliderComponent>);

	object.init();
}

void ColliderComponent::move(float x, float y, float z)
{
	glm::vec3 velocity = { x, y, z };

	auto thisPos = getEntity()->getPosition() + glm::vec3(x, 0, z);
	auto thisRectX = getRect(getEntity()->getPosition() + glm::vec3(x, 0, 0), _boxSize);
	auto thisRectZ = getRect(getEntity()->getPosition() + glm::vec3(0, 0, z), _boxSize);

	// scan every entity on path
	auto& entities = this->getEntity()->getScene()->getEntities("path");
	for (auto& entity : entities)
	{
		// if entity has collider
		if (entity->hasComponent<ColliderComponent>())
		{
			auto comp = entity->getComponentAs<ColliderComponent>();
			
			if (this->_boxCollider && comp->isBoxCollider())// if both have box collider
			{
				auto entityRect = getRect(entity->getPosition(), comp->getBoxSize());

				// if move to left/right will be collsion
				if (thisRectX.intersects(entityRect))
				{
					if (comp->isDynamic())
						entity->move({ velocity.x, 0, 0 }); // move the other
					else
						velocity.x = 0.f; // stop
				}

				// if move to up/down will be collsion
				if (thisRectZ.intersects(entityRect))
				{
					if (comp->isDynamic())
						entity->move({ 0, 0, velocity.z }); // move the other
					else
						velocity.z = 0.f; // stop
				}
			}
			else if (this->_cicleCollider && comp->isCircleCollider()) // if both have circle collider
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
	}

	getEntity()->move(velocity);
}

sf::FloatRect ColliderComponent::getRect(const glm::vec3& pos, const glm::vec2& size)
{
	return sf::FloatRect(pos.x - size.x, pos.z - size.y, size.x, size.y);
}

}
