#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "SFML/OpenGL.hpp"
#include <GL/glu.h>

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/DragonBones/Armature/ArmatureObject.hpp"
#include "Szczur/Modules/DragonBones/Armature/ArmatureObjectType.hpp"
#include "Szczur/Modules/World/Sprite/SpriteObject.hpp"
#include "Szczur/Modules/World/Sprite/SpriteObjectType.hpp"

namespace rat
{

/* Operators */
/// Constructor
World::World()
{
	LOG_INFO(this, ": World module initializing"); 
	this->init();
	LOG_INFO(this, ": World module initialized!"); 
}
/// Destructor
World::~World()
{
    LOG_INFO(this, ": World module destructed"); 
}

static sf::Texture stoneTex;

/* Methods*/
/// init
void World::init()
{
	this->currentMap = &(this->maps.emplace_back());

	// @info Testing propouses
	//auto cedmin = new ArmatureObjectType("Cedmin", getModule<DragonBones>().getFactory());
	//this->types.push_back(cedmin);
	//auto dragonType = new ArmatureObjectType("Dragon", getModule<DragonBones>().getFactory());
	//this->types.push_back(dragonType);
	//auto stoneType = new SpriteObjectType("Stone");
	//this->types.push_back(stoneType);

	//this->currentMap->sceneObjects.push_back(
	//	new ArmatureObject(cedmin, "Player", {800.f, 600.f})
	//);
	//this->currentMap->sceneObjects.push_back(
	//	new ArmatureObject(dragonType, "Smok", {100.f, 400.f}, "Dragon")
	//);
	//this->currentMap->sceneObjects.push_back(
	//	new SpriteObject(stoneType, "MyStone", {400.f, 300.f})
	//);




	if (stoneTex.loadFromFile("Assets/Objects/Stone/texture.png")) {
		LOG_ERROR("WTF");
	}

	sf::RenderWindow& window = getModule<Window>().getWindow();
	window.pushGLStates();
}

/// update
void World::update(float deltaTime)
{
	currentMap->update(deltaTime);
}

/// render
void World::render()
{
	getModule<Window>().draw(*currentMap);

	sf::RenderWindow& window = getModule<Window>().getWindow();

	window.popGLStates();
	window.setActive(true);

	// glClearDepth(1.f);
    // glEnable(GL_DEPTH_TEST);
    // glDepthMask(GL_TRUE);

    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluPerspective(90.f, 1.f, 1.f, 300.0f);
	

	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// glTranslatef(1.5f, 0.0f, -7.0f);
	
	// glEnable(GL_TEXTURE_2D);
	// sf::Texture::bind(&stoneTex);

	// glBegin(GL_QUADS);
	// glTexCoord2f(0, 0); glVertex3f( 1.0f,  1.0f, 1.0f); 
	// glTexCoord2f(0, 1); glVertex3f(-1.0f,  1.0f, 1.0f);
	// glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
	// glTexCoord2f(1, 0); glVertex3f( 1.0f, -1.0f, 1.0f);
	// glEnd(); 

	window.setActive(false);
	window.pushGLStates();
}

}
