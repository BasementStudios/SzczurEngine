/** @file SF3DFactory.cpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#include "SF3DFactory.hpp"

#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "SF3DSlot.hpp"
#include "SF3DTextureData.hpp"
#include "SF3DTextureAtlasData.hpp"
#include "SF3DArmatureProxy.hpp"
#include "SF3DDisplay.hpp"
#include "SF3DEventDispatcher.hpp"

DRAGONBONES_NAMESPACE_BEGIN

DragonBones* SF3DFactory::_dragonBonesInstance;
SF3DFactory* SF3DFactory::_factory;

SF3DFactory::SF3DFactory()
{
	_factory = this;

	if (_dragonBonesInstance == nullptr)
	{
		_soundEventDispatcher = std::make_unique<SF3DEventDispatcher>();

		_dragonBonesInstance = new DragonBones(nullptr);
		_dragonBonesInstance->yDown = false;
	}

	_dragonBones = _dragonBonesInstance;
}

SF3DFactory::~SF3DFactory()
{
	clear();

	if (_dragonBonesInstance)
	{
		delete _dragonBonesInstance;
	}
}

DragonBonesData* SF3DFactory::loadDragonBonesData(const std::string& filePath, const std::string& name)
{
	if (!name.empty())
	{
		const auto existedData = getDragonBonesData(name);

		if (existedData)
			return existedData;
	}

	std::stringstream data;

	std::ifstream json(filePath);

	if (json.bad())
		return nullptr;

	data << json.rdbuf();

	if (data.str().empty())
		return nullptr;

	return parseDragonBonesData(data.str().c_str(), name, 1.0f);
}

TextureAtlasData* SF3DFactory::loadTextureAtlasData(const std::string& filePath, sf3d::Texture* atlasTexture, const std::string& name, float scale)
{
	std::stringstream data;

	std::ifstream json(filePath);

	if (json.bad())
		return nullptr;

	data << json.rdbuf();

	if (data.str().empty())
		return nullptr;

	return static_cast<SF3DTextureAtlasData*>(BaseFactory::parseTextureAtlasData(data.str().c_str(), atlasTexture, name, scale));
}

TextureAtlasData* SF3DFactory::createTextureAtlasData(std::vector<SF3DTextureData*>& texturesData, DragonBonesData* dragonBonesData)
{
	if (dragonBonesData == nullptr)
		return nullptr;

	auto textureAtlasData = dragonBones::BaseObject::borrowObject<dragonBones::SF3DTextureAtlasData>();
	
	textureAtlasData->name = dragonBonesData->name;
	
	for (auto& textureData : texturesData)
	{
		textureData->parent = textureAtlasData;
		textureAtlasData->addTexture(textureData);
	}

	addTextureAtlasData(textureAtlasData);

	return textureAtlasData;
}

SF3DArmatureProxy* SF3DFactory::buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName, const std::string& textureAtlasName) const
{
	const auto armature = buildArmature(armatureName, dragonBonesName, skinName, textureAtlasName);

	if (armature != nullptr)
	{
		_dragonBones->getClock()->add(armature);

		return static_cast<SF3DArmatureProxy*>(armature->getDisplay());
	}

	return nullptr;
}

sf3d::Texture* SF3DFactory::getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName) const
{
	const auto textureData = static_cast<SF3DTextureData*>(_getTextureData(dragonBonesName, textureName));

	if (textureData != nullptr && textureData->texture != nullptr)
	{
		return textureData->texture;
	}

	return nullptr;
}

void SF3DFactory::update(float deltaTime)
{
	_dragonBonesInstance->advanceTime(deltaTime);
}

TextureAtlasData* SF3DFactory::_buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
	auto textureAtlasData_ = static_cast<SF3DTextureAtlasData*>(textureAtlasData);

	if (textureAtlasData != nullptr)
	{
		if (textureAtlas != nullptr)
		{
			textureAtlasData_->setRenderTexture(static_cast<sf3d::Texture*>(textureAtlas));
		}
		else
		{
			throw std::runtime_error("No atlas texture"); 
		}
	}
	else
	{
		textureAtlasData_ = BaseObject::borrowObject<SF3DTextureAtlasData>();
	}

	return textureAtlasData_;
}

Armature* SF3DFactory::_buildArmature(const BuildArmaturePackage& dataPackage) const
{
	const auto armature = BaseObject::borrowObject<Armature>();
	const auto armatureDisplay = new SF3DArmatureProxy();

	armature->init(dataPackage.armature, armatureDisplay, armatureDisplay, _dragonBones);

	return armature;
}

Slot* SF3DFactory::_buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const
{
	auto slot = BaseObject::borrowObject<SF3DSlot>();
	auto wrapperDisplay = new SF3DDisplay();

	slot->init(slotData, armature, wrapperDisplay, wrapperDisplay);

	return slot;
}

DRAGONBONES_NAMESPACE_END
