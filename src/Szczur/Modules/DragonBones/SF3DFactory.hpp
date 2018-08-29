/** @file SF3DFactory.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#pragma once

#include <string>
#include <vector>

#include <dragonBones/DragonBonesHeaders.h>

#include "Szczur/Utility/SFML3D/Texture.hpp"

#include "SF3DEventDispatcher.hpp"

DRAGONBONES_NAMESPACE_BEGIN

class SF3DArmatureProxy;
class SF3DTextureData;

class SF3DFactory : public BaseFactory
{
protected:
	static DragonBones*						_dragonBonesInstance;
	static SF3DFactory*						_factory;

	std::unique_ptr<SF3DEventDispatcher>	_soundEventDispatcher;

public:
	SF3DFactory();
	~SF3DFactory();

public:
	DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& name = "");
	TextureAtlasData* loadTextureAtlasData(const std::string& filePath, sf3d::Texture *atlasTexture, const std::string& name = "", float scale = 1.0f);
	SF3DArmatureProxy* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "") const;
	sf3d::Texture* getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName = "") const;

	void addSoundEventListener(const std::function<void(EventObject*)>& listener)
	{
		_soundEventDispatcher->addDBEventListener(EventObject::SOUND_EVENT, listener);
	}

	void update(float deltaTime);

	TextureAtlasData* createTextureAtlasData(std::vector<SF3DTextureData*>& texturesData, DragonBonesData* dragonBonesData);

	static SF3DFactory* get() { return _factory; }

protected:
	TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
	Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const override;
	Slot* _buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const override;
};

DRAGONBONES_NAMESPACE_END
