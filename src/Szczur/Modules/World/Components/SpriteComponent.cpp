#include "SpriteComponent.hpp"

#include <memory> // unique_ptr

#include <nlohmann/json.hpp>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Modules/World/Data/SpriteDisplayData.hpp"
#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Entity.hpp"

namespace rat
{

SpriteComponent::SpriteComponent(Entity* parent)
: 	Component { parent, fnv1a_64("SpriteComponent"), "SpriteComponent", Component::Drawable }
{
	;
}

///
std::unique_ptr<Component> SpriteComponent::copy(Entity* newParent) const
{
	auto ptr = std::make_unique<SpriteComponent>(*this);

	ptr->setEntity(newParent);

	return ptr;
}

///
void SpriteComponent::setSpriteDisplayData(SpriteDisplayData* spriteDisplayData)
{
	_spriteDisplayData = spriteDisplayData;
}

///
SpriteDisplayData* SpriteComponent::getSpriteDisplayData() const
{
	return _spriteDisplayData;
}

///
void* SpriteComponent::getFeature(Component::Feature_e feature)
{
	if (feature == Feature_e::Drawable)	return static_cast<sf3d::Drawable*>(this);

	return nullptr;
}

///
const void* SpriteComponent::getFeature(Component::Feature_e feature) const
{
	if (feature == Feature_e::Drawable) return static_cast<const sf3d::Drawable*>(this);

	return nullptr;
}

///
void SpriteComponent::loadFromConfig(nlohmann::json& config)
{
	Component::loadFromConfig(config);
}

///
void SpriteComponent::saveToConfig(nlohmann::json& config) const
{
	Component::saveToConfig(config);
}

///
void SpriteComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	if (_spriteDisplayData) {
		states.transform *= getEntity()->getTransform();
		target.draw(*_spriteDisplayData, states);
	}
}

}
