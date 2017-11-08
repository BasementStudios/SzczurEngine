#include "WrapperFactory.h"

#include <fstream>

#include <SFML\Graphics.hpp>

#include "WrapperSlot.h"
#include "WrapperTextureData.h"
#include "WrapperTextureAtlasData.h"
#include "WrapperArmatureDisplay.h"
#include "Common.h"
#include "TextureMgr.h"
#include "WrapperSprite.h"
#include "WrapperMesh.h"
#include "WrapperDisplay.h"

dragonBones::DragonBones* WrapperFactory::_dragonBonesInstance;

WrapperFactory::WrapperFactory()
{
	if (_dragonBonesInstance == nullptr)
	{

		const auto eventManager = new WrapperArmatureDisplay();
		//eventManager->retain();

		_dragonBonesInstance = new dragonBones::DragonBones(eventManager);
		_dragonBonesInstance->yDown = false;
	}

	_dragonBones = _dragonBonesInstance;
}

WrapperFactory::~WrapperFactory()
{
	clear();
}

dragonBones::DragonBonesData* WrapperFactory::loadDragonBonesData(const std::string& filePath, const std::string& name)
{
	if (!name.empty())
	{
		const auto existedData = getDragonBonesData(name);
		if (existedData)
		{
			return existedData;
		}
	}

	if (!isFileExist(filePath))
		return nullptr;

	std::string data;

	std::ifstream json(filePath);

	if (json.bad())
		return nullptr;

	json >> data;

	if (data.empty())
		return nullptr;

	return parseDragonBonesData(data.c_str(), name, 1.0f);
}

dragonBones::TextureAtlasData* WrapperFactory::loadTextureAtlasData(const std::string& filePath, const std::string& name, float scale)
{
	_path = filePath;

	if (!isFileExist(filePath))
		return nullptr;

	std::string data;

	std::ifstream json(filePath);

	if (json.bad())
		return nullptr;

	json >> data;
	return static_cast<WrapperTextureAtlasData*>(BaseFactory::parseTextureAtlasData(data.c_str(), nullptr, name, scale));
}

WrapperArmatureDisplay* WrapperFactory::buildArmatureDisplay(const std::string & armatureName, const std::string & dragonBonesName, const std::string & skinName, const std::string & textureAtlasName) const
{
	const auto armature = buildArmature(armatureName, dragonBonesName, skinName, textureAtlasName);

	if (armature != nullptr)
	{
		_dragonBones->getClock()->add(armature);

		return static_cast<WrapperArmatureDisplay*>(armature->getDisplay());
	}

	return nullptr;
}

sf::Sprite* WrapperFactory::getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName) const
{
	const auto textureData = static_cast<WrapperTextureData*>(_getTextureData(dragonBonesName, textureName));
	if (textureData != nullptr && textureData->Sprite != nullptr)
	{
		return textureData->Sprite;
	}

	return nullptr;
}

void WrapperFactory::update(float lastUpdate)
{
	_dragonBonesInstance->advanceTime(lastUpdate);
}

void WrapperFactory::render(sf::RenderWindow &window)
{
}

dragonBones::TextureAtlasData* WrapperFactory::_buildTextureAtlasData(dragonBones::TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
	if (textureAtlasData != nullptr)
	{
		if (textureAtlas != nullptr)
		{
			static_cast<WrapperTextureAtlasData*>(textureAtlasData)->setRenderTexture(static_cast<sf::Texture*>(textureAtlas));
		}
		else
		{
			auto texture = TextureMgr::Get()->GetTexture(textureAtlasData->imagePath);

			static_cast<WrapperTextureAtlasData*>(textureAtlasData)->setRenderTexture(texture);
		}
	}
	else
	{
		textureAtlasData = dragonBones::BaseObject::borrowObject<WrapperTextureAtlasData>();
	}

	return textureAtlasData;
}

dragonBones::Armature* WrapperFactory::_buildArmature(const dragonBones::BuildArmaturePackage& dataPackage) const
{
	const auto armature = dragonBones::BaseObject::borrowObject<dragonBones::Armature>();
	const auto armatureDisplay = new WrapperArmatureDisplay();

	/*armatureDisplay->retain();
	armatureDisplay->setCascadeOpacityEnabled(true);
	armatureDisplay->setCascadeColorEnabled(true);*/

	armature->init(dataPackage.armature, armatureDisplay, armatureDisplay, _dragonBones);

	return armature;
}

dragonBones::Slot* WrapperFactory::_buildSlot(const dragonBones::BuildArmaturePackage& dataPackage, dragonBones::SlotData* slotData, std::vector<dragonBones::DisplayData*>* displays, dragonBones::Armature& armature) const
{
	const auto slot = dragonBones::BaseObject::borrowObject<WrapperSlot>();
	auto wrapperDisplay = new WrapperDisplay();

	/*rawDisplay->retain();
	rawDisplay->setCascadeOpacityEnabled(true);
	rawDisplay->setCascadeColorEnabled(true);
	rawDisplay->setAnchorPoint(cocos2d::Vec2::ZERO);
	rawDisplay->setLocalZOrder(slotData->zOrder);*/
	
	slot->init(slotData, displays, wrapperDisplay, wrapperDisplay);

	return slot;
}
