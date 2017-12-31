#include "DragonBones.hpp"

namespace rat 
{
	void DragonBones::init()
	{
		_factory = std::make_unique<dragonBones::SFMLFactory>();
	}

	void DragonBones::update(float deltaTime)
	{
		_factory->update(deltaTime);
	}

	void DragonBones::render()
	{

	}

	void DragonBones::input(sf::Event& e)
	{
		(void)e;
	}
	Armature* DragonBones::createArmaturee(const std::string& actorName)
	{
		if (_factory == nullptr)
			return nullptr;

		std::string path = actorName.data();

		auto dbData = _factory->loadDragonBonesData(path + "/skeleton.json");

		if (!dbData)
			return nullptr;

		auto texturesData = _factory->getTexturesData(dbData, path + "/textures");

		for (auto& textureData : texturesData) {
			auto tex = new sf::Texture();
			tex->loadFromFile(textureData->path);
			textureData->setTexture(tex);

			// @todo: load textures with assets manager
		}

		auto textureAtlasData = _factory->createTextureAtlasData(texturesData, dbData);

		if (textureAtlasData == nullptr) {
			for (auto& textureData : texturesData) {
				if (textureData)
					delete textureData;
			}

			return nullptr;
		}

		auto armatureDisplay = _factory->buildArmatureDisplay(actorName.data());

		if (!armatureDisplay)
			return nullptr;

		auto armature = new Armature(armatureDisplay);

		return armature;
	}
}
