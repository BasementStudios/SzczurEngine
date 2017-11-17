#include "SFMLFactory.h"

#include <fstream>

#include <SFML\Graphics.hpp>

#include "SFMLSlot.h"
#include "SFMLTextureData.h"
#include "SFMLTextureAtlasData.h"
#include "SFMLArmatureDisplay.h"
#include "SFMLDisplay.h"

#include "TextureMgr.h"
#include "Common.h"

DRAGONBONES_NAMESPACE_BEGIN

DragonBones* SFMLFactory::_dragonBonesInstance;

SFMLFactory::SFMLFactory()
{
	if (_dragonBonesInstance == nullptr)
	{
		const auto eventManager = new SFMLArmatureDisplay();
		_dragonBonesInstance = new DragonBones(eventManager);
		_dragonBonesInstance->yDown = false;
	}

	_dragonBones = _dragonBonesInstance;
}

SFMLFactory::~SFMLFactory()
{
	clear();

	if (_dragonBonesInstance)
	{
		delete _dragonBonesInstance;
	}
}

DragonBonesData* SFMLFactory::loadDragonBonesData(const std::string& filePath, const std::string& name)
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

TextureAtlasData* SFMLFactory::loadTextureAtlasData(const std::string& filePath, const std::string& name, float scale)
{
	_path = filePath;

	if (!isFileExist(filePath))
		return nullptr;

	std::string data;

	std::ifstream json(filePath);

	if (json.bad())
		return nullptr;

	json >> data;
	return static_cast<SFMLTextureAtlasData*>(BaseFactory::parseTextureAtlasData(data.c_str(), nullptr, name, scale));
}

SFMLArmatureDisplay* SFMLFactory::buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName, const std::string& textureAtlasName) const
{
	const auto armature = buildArmature(armatureName, dragonBonesName, skinName, textureAtlasName);

	if (armature != nullptr)
	{
		_dragonBones->getClock()->add(armature);

		return static_cast<SFMLArmatureDisplay*>(armature->getDisplay());
	}

	return nullptr;
}

sf::Sprite* SFMLFactory::getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName) const
{
	const auto textureData = static_cast<SFMLTextureData*>(_getTextureData(dragonBonesName, textureName));
	if (textureData != nullptr && textureData->Sprite != nullptr)
	{
		return textureData->Sprite.get();
	}

	return nullptr;
}

void SFMLFactory::update(float lastUpdate)
{
	_dragonBonesInstance->advanceTime(lastUpdate);
}

TextureAtlasData* SFMLFactory::_buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
	auto wrappperTextureAtlasData = static_cast<SFMLTextureAtlasData*>(textureAtlasData);

	if (textureAtlasData != nullptr)
	{
		if (textureAtlas != nullptr)
		{
			wrappperTextureAtlasData->setRenderTexture(static_cast<sf::Texture*>(textureAtlas));
		}
		else
		{
			auto texture = TextureMgr::Get()->GetTexture(textureAtlasData->imagePath);

			wrappperTextureAtlasData->setRenderTexture(texture);
		}
	}
	else
	{
		wrappperTextureAtlasData = BaseObject::borrowObject<SFMLTextureAtlasData>();
	}

	return wrappperTextureAtlasData;
}

Armature* SFMLFactory::_buildArmature(const BuildArmaturePackage& dataPackage) const
{
	const auto armature = BaseObject::borrowObject<Armature>();
	const auto armatureDisplay = new SFMLArmatureDisplay();

	armature->init(dataPackage.armature, armatureDisplay, armatureDisplay, _dragonBones);

	return armature;
}

Slot* SFMLFactory::_buildSlot(const BuildArmaturePackage& dataPackage, SlotData* slotData, std::vector<DisplayData*>* displays, Armature& armature) const
{
	auto slot = BaseObject::borrowObject<SFMLSlot>();
	auto wrapperDisplay = new SFMLDisplay();
	
	_wrapperSlots.push_back(std::unique_ptr<SFMLSlot>(slot));

	slot->init(slotData, displays, wrapperDisplay, wrapperDisplay);

	return slot;
}

DRAGONBONES_NAMESPACE_END
