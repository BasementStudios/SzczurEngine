#pragma once

#include <memory> // unique_ptr

#include <nlohmann/json_fwd.hpp>

namespace sf3d {
	class RenderStates;
}
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/SpriteDisplayData.hpp"

namespace rat
{

class SpriteComponent : public sf3d::Drawable, public Component
{
public:

	///
	SpriteComponent(Entity* parent);

	///
	SpriteComponent(const SpriteComponent&) = default;

	///
	SpriteComponent& operator = (const SpriteComponent&) = default;

	///
	SpriteComponent(SpriteComponent&&) = default;

	///
	SpriteComponent& operator = (SpriteComponent&&) = default;

	///
	~SpriteComponent() = default;

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	///
	void setSpriteDisplayData(SpriteDisplayData* spriteDisplayData);

	///
	SpriteDisplayData* getSpriteDisplayData() const;

	///
	virtual void* getFeature(Component::Feature_e feature) override;

	///
	virtual const void* getFeature(Component::Feature_e feature) const override;

	///
	virtual void loadFromConfig(nlohmann::json& config) override;

	///
	virtual void saveToConfig(nlohmann::json& config) const override;

	///
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states = sf3d::RenderStates::Default) const override;

private:

	SpriteDisplayData* _spriteDisplayData = nullptr;

};

}
