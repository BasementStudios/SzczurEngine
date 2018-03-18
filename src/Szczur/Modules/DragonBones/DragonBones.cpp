#include "DragonBones.hpp"

/** @file DragonBones.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <dragonBones/SFMLFactory.h>

#include "Szczur/Debug.hpp"
#include "UglyArmature.hpp"

namespace rat 
{

/* Properties */
/// Factory
DragonBones::Factory_t& DragonBones::getFactory()
{
	return this->factory;
}
const DragonBones::Factory_t& DragonBones::getFactory() const
{
	return this->factory;
}


/* Methods */
/// init
void DragonBones::init()
{
	this->initScript();
}
/// init Script
void DragonBones::initScript()
{
	Script& script = getModule<Script>();
	auto module = script.newModule("DragonBones");
	
	module.set_function("createArmature", &DragonBones::createArmature, this);
	
	script.initClasses<UglyArmature>();
}
/// update
void DragonBones::update(float deltaTime)
{
	this->factory.update(deltaTime);
}

/// createArmature
UglyArmature* DragonBones::createArmature(const std::string& name)
{
	auto skeletonData = this->factory.loadDragonBonesData("Assets/Objects/" + name + "/skeleton.json");

	if (!skeletonData) {
		LOG_ERROR("skeletdat fail");
		return nullptr;
	}

	auto texturesData = this->factory.getTexturesData(skeletonData, "Assets/Objects/" + name + "/textures");

	for (auto& textureData : texturesData) {
		auto tex = new sf::Texture();
		if (!tex->loadFromFile(textureData->path)) {
			LOG_ERROR("brak tex ", textureData->path);
			return nullptr;
		}
		textureData->setTexture(tex);
	}

	auto textureAtlasData = this->factory.createTextureAtlasData(texturesData, skeletonData);

	if (textureAtlasData == nullptr) {
		LOG_ERROR("texatlasdat fail");
		return nullptr;
	}

	auto sfmldisplay = this->factory.buildArmatureDisplay(name);

	if (!sfmldisplay) {
		LOG_ERROR("sfmldisp fail");
		return nullptr;
	}

	auto arm = new UglyArmature(sfmldisplay);

	return arm;
}

}
