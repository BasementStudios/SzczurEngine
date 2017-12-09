#include "Armature.hpp"

#include <dragonBones\SFMLFactory.h>
#include <dragonBones\SFMLArmatureDisplay.h>

namespace rat
{
	Armature::Armature(std::string_view name)
	{
		create(name);
	}

	void Armature::create(std::string_view name)
	{
		auto factory = dragonBones::SFMLFactory::get();

		if (!factory)
			return;

		std::string path = name.data();

		auto dbData = factory->loadDragonBonesData(path + "/skeleton.json");

		if (dbData == nullptr)
			return;

		auto texturesData = factory->getTexturesData(dbData, path + "/textures");

		for (auto& textureData : texturesData) {
			auto tex = new sf::Texture();
			tex->loadFromFile(textureData->path);
			textureData->setTexture(tex);
		}

		auto textureAtlasData = factory->createTextureAtlasData(texturesData, dbData);

		if (textureAtlasData == nullptr) {

			for (auto& textureData : texturesData) {
				if (textureData)
					delete textureData;
			}

			return;
		}

		_armatureDisplay = std::unique_ptr<dragonBones::SFMLArmatureDisplay>(factory->buildArmatureDisplay(name.data()));
	}

	void Armature::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_armatureDisplay)
			target.draw(*_armatureDisplay, states);
	}
}
