#include "SpriteComponent.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"

namespace rat {
    SpriteComponent::SpriteComponent(Entity* parent)
    : Component { parent, fnv1a_64("SpriteComponent"), "SpriteComponent", Component::Drawable }
	{

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
	void SpriteComponent::loadFromConfig(const Json& config)
	{
		Component::loadFromConfig(config);
		auto& spriteDisplayDataHolder = getEntity()->getScene()->getSpriteDisplayDataHolder();
		auto name = config["spriteDisplayData"].get<std::string>();
		if(name != "") {
			bool found{false};
			for(auto& it : spriteDisplayDataHolder) {
				if(name == it.getName()) {
					setSpriteDisplayData(&it);
					found = true;
				}
			}
			if(!found) {
				try {
					setSpriteDisplayData(&(spriteDisplayDataHolder.emplace_back(name)));
				}
				catch(const std::exception& exc) {

				}
			}
		}
	}

	///
	void SpriteComponent::saveToConfig(Json& config) const
	{
		Component::saveToConfig(config);
		config["spriteDisplayData"] = _spriteDisplayData ? _spriteDisplayData->getName() : "";
	}

	///
	void SpriteComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
	{
		if(_spriteDisplayData) {
            states.transform *= getEntity()->getTransform();
            target.draw(*_spriteDisplayData, states);
        }
	}
}
