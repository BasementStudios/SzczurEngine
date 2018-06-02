#pragma once

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/SpriteDisplayData.hpp"

namespace rat
{
class Entity;
class BaseComponent : public Component
{
public:

	///
	BaseComponent(Entity* parent);

	///
	BaseComponent(const BaseComponent&) = default;

	///
	BaseComponent& operator = (const BaseComponent&) = default;

	///
	BaseComponent(BaseComponent&&) = default;

	///
	BaseComponent& operator = (BaseComponent&&) = default;

	///
	~BaseComponent() = default;

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	///
	virtual void loadFromConfig(Json& config) override;

	///
	virtual void saveToConfig(Json& config) const override;

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

	///
	void positionOnly(bool flag);

private:

	bool _positionOnly = false;

};

}
