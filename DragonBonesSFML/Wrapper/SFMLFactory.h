#pragma once

#include <string>
#include <vector>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\Sprite.hpp>

#include "SFMLArmatureDisplay.h"
#include "SFMLSlot.h"
#include "SFMLTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLFactory : public BaseFactory
{
protected:
	static DragonBones*											_dragonBonesInstance;

	mutable std::vector<std::unique_ptr<SFMLSlot>>				_wrapperSlots;
	mutable std::vector<std::unique_ptr<SFMLTextureData>>		_wrapperTexturesData;

public:
	SFMLFactory();
	~SFMLFactory();

public:
	DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& name = "");
	TextureAtlasData* loadTextureAtlasData(const std::string& filePath, const std::string& name = "", float scale = 1.0f);
	SFMLArmatureDisplay* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "") const;
	sf::Texture* getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName = "") const;

	void update(float lastUpdate);

	bool loadTextures(DragonBonesData* dragonBonesData, const std::string& folderPath);

protected:
	TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
	Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const override;
	Slot* _buildSlot(const BuildArmaturePackage& dataPackage, SlotData* slotData, std::vector<DisplayData*>* displays, Armature& armature) const override;
};

DRAGONBONES_NAMESPACE_END
