#include "SFMLFactory.h"

#include <fstream>
#include <sstream>

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
		_dragonBonesInstance = new DragonBones(nullptr);
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
			return existedData;
	}

	if (!isFileExist(filePath))
		return nullptr;

	std::stringstream data;

	std::ifstream json(filePath);

	if (json.bad())
		return nullptr;

	data << json.rdbuf();

	if (data.str().empty())
		return nullptr;

	return parseDragonBonesData(data.str().c_str(), name, 1.0f);
}

TextureAtlasData* SFMLFactory::loadTextureAtlasData(const std::string& filePath, const std::string& name, float scale)
{
	if (!isFileExist(filePath))
		return nullptr;

	std::stringstream data;

	std::ifstream json(filePath);

	if (json.bad())
		return nullptr;

	data << json.rdbuf();

	if (data.str().empty())
		return nullptr;

	return static_cast<SFMLTextureAtlasData*>(BaseFactory::parseTextureAtlasData(data.str().c_str(), nullptr, name, scale));
}

void SFMLFactory::loadTextures(DragonBonesData *dragonBonesData, const std::string& folderPath)
{
	SFMLTextureAtlasData* textureAtlasData = BaseObject::borrowObject<SFMLTextureAtlasData>();
	textureAtlasData->name = dragonBonesData->name;

	for (auto& armName : dragonBonesData->armatureNames) 
	{
		auto arm = dragonBonesData->armatures[armName];

		for (auto& skin : arm->skins)
		{
			for (auto& displays : skin.second->displays) 
			{
				for (auto& display : displays.second)
				{
					std::string fileTex = folderPath + "/" + display->path + ".png";

					auto tex = TextureMgr::Get()->GetTexture(fileTex);

					if (!tex)
						continue;

					auto size = tex->getSize();

					const auto textureData = static_cast<SFMLTextureData*>(textureAtlasData->createTexture());
					textureData->rotated = false;
					textureData->name = display->name;
					textureData->region.x = 0.f;
					textureData->region.y = 0.f;
					textureData->region.width = size.x;
					textureData->region.height = size.y;

					textureData->Texture = tex;
					textureData->Rect = { 0, 0, (int)size.x, (int)size.y };

					textureAtlasData->addTexture(textureData);
				}
			}
		}
	}

	addTextureAtlasData(textureAtlasData);
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

sf::Texture* SFMLFactory::getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName) const
{
	const auto textureData = static_cast<SFMLTextureData*>(_getTextureData(dragonBonesName, textureName));
	if (textureData != nullptr && textureData->Texture != nullptr)
	{
		return textureData->Texture;
	}

	return nullptr;
}

void SFMLFactory::update(float lastUpdate)
{
	_dragonBonesInstance->advanceTime(lastUpdate);
}

TextureAtlasData* SFMLFactory::_buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
	auto textureAtlasData_ = static_cast<SFMLTextureAtlasData*>(textureAtlasData);

	if (textureAtlasData != nullptr)
	{
		if (textureAtlas != nullptr)
		{
			textureAtlasData_->setRenderTexture(static_cast<sf::Texture*>(textureAtlas));
		}
		else
		{
			auto texture = TextureMgr::Get()->GetTexture(textureAtlasData->imagePath);

			textureAtlasData_->setRenderTexture(texture);
		}
	}
	else
	{
		textureAtlasData_ = BaseObject::borrowObject<SFMLTextureAtlasData>();
	}

	return textureAtlasData_;
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
