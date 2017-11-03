#pragma once

#include <string>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include "WrapperArmatureDisplay.h"

class WrapperFactory : public dragonBones::BaseFactory
{
protected:
	static dragonBones::DragonBones* _dragonBonesInstance;

	std::string _path;

public:
	WrapperFactory();
	~WrapperFactory();

public:
	dragonBones::DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& name = "");
	dragonBones::TextureAtlasData* loadTextureAtlasData(const std::string& filePath, const std::string& name = "", float scale = 1.0f);
	WrapperArmatureDisplay* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "") const;
	sf::Sprite* getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName = "") const;

	void update(float lastUpdate);
	void render(sf::RenderWindow &window);

protected:
	dragonBones::TextureAtlasData * _buildTextureAtlasData(dragonBones::TextureAtlasData * textureAtlasData, void * textureAtlas) const override;
	dragonBones::Armature * _buildArmature(const dragonBones::BuildArmaturePackage & dataPackage) const override;
	dragonBones::Slot * _buildSlot(const dragonBones::BuildArmaturePackage & dataPackage, dragonBones::SlotData * slotData, std::vector<dragonBones::DisplayData*>* displays, dragonBones::Armature & armature) const override;

};

