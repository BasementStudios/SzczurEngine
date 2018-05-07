#pragma once

#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Modules/World/Entity.hpp"
#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/SpriteDisplayData.hpp"

namespace rat
{

class SpriteComponent : public sf3d::Drawable, public Component
{
public:

	///
	SpriteComponent(Entity* parent)
		: Component { parent, fnv1a_64("SpriteComponent"), "SpriteComponent", Component::Drawable }
	{

	}

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
	virtual std::unique_ptr<Component> copy() const override
	{
		return std::make_unique<SpriteComponent>(*this);
	}

	///
	void setSpriteDisplayData(SpriteDisplayData* spriteDisplayData)
	{
		_spriteDisplayData = spriteDisplayData;
	}

	///
	SpriteDisplayData* getSpriteDisplayData() const
	{
		return _spriteDisplayData;
	}

	///
	virtual void* getFeature(Component::Feature_e feature) override
	{
		if (feature == Feature_e::Drawable)	return static_cast<sf3d::Drawable*>(this);

		return nullptr;
	}

	///
	virtual const void* getFeature(Component::Feature_e feature) const override
	{
		if (feature == Feature_e::Drawable) return static_cast<const sf3d::Drawable*>(this);

		return nullptr;
	}

	///
	virtual void loadFromConfig(const Json& config) override
	{
		Component::loadFromConfig(config);
	}

	///
	virtual void saveToConfig(Json& config) const override
	{
		Component::saveToConfig(config);
	}

	///
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override
	{
		if(_spriteDisplayData) {
			target.draw(*_spriteDisplayData, states);
		}
	}

private:

	SpriteDisplayData* _spriteDisplayData = nullptr;

};

}
