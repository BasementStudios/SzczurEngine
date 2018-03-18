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
    LOG_INFO("Assets/Objects/" + name + "/skeleton.json");
	auto skeletonData = factory.loadDragonBonesData("Assets/Objects/" + name + "/skeleton.json");

    LOG_INFO("Assets/Objects/" + name + "/texture.png");
	auto tex1 = new sf::Texture();
    if (!tex1->loadFromFile("Assets/Objects/" + name + "/texture.png"))
    {
        LOG_ERROR("CAnnot!");
    }

	LOG_INFO("Assets/Objects/" + name + "/atlas.json");
    auto tex = factory.loadTextureAtlasData("Assets/Objects/" + name + "/atlas.json", tex1);

    if (!tex)
        LOG_INFO("Err");

    auto armatureDisplay = factory.buildArmatureDisplay(name);

    LOG_INFO("Loaded!");

    return new UglyArmature(armatureDisplay);
}

}
