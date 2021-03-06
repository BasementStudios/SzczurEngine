#pragma once

#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"

#include <glm/glm.hpp>

namespace rat
{

class ColliderComponent : public Component
{
public:
	///
	ColliderComponent(Entity* parent);

	///
	ColliderComponent(const ColliderComponent&) = default;

	///
	ColliderComponent& operator = (const ColliderComponent&) = default;

	///
	ColliderComponent(ColliderComponent&&) = default;

	///
	ColliderComponent& operator = (ColliderComponent&&) = default;

	///
	~ColliderComponent() = default;

	///
	std::unique_ptr<Component> copy(Entity* newParent) const override;

	///
	void loadFromConfig(Json& config) override;

	///
	void saveToConfig(Json& config) const override;

	///
	void renderHeader(ScenesManager& scenes, Entity* object) override;

	///
	static void initScript(ScriptClass<Entity>& entity, Script& script);

	// Scripts
	void move(float x, float y, float z);

	///
	bool isCircleCollider() const { return _circleCollider; }

	///
	void setCircleCollider(bool circleCollider) { _circleCollider = circleCollider; }

	///
	void setCircleRadius(float radius) { _circleRadius = radius; }

	///
	float getCircleRadius() const { return _circleRadius; }

	///
	void setBoxCollider(bool boxCollider) { _boxCollider = boxCollider; }

	///
	bool isBoxCollider() const { return _boxCollider; }

	///
	void setBoxSize(float x, float y) { _boxSize = { x, y }; }

	///
	const auto& getBoxSize() const { return _boxSize; }

	///
	bool isDynamic() { return _isDynamic; }

	///
	void setDynamic(bool dynamic) { _isDynamic = dynamic; }

private:
	///
	static sf::FloatRect _getRect(const glm::vec3& pos, const glm::vec2& size);

private:
	bool _isDynamic = false;

	bool _circleCollider = false;
	float _circleRadius = 100.f;

	bool _boxCollider = false;
	glm::vec2 _boxSize = { 200.f, 200.f };
};

}
