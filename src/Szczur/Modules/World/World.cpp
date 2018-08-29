#include "World.hpp"

namespace rat
{

World::World()
{
	LOG_INFO("Initializing World module");
	LOG_INFO("Module World initialized");
}

World::~World()
{
	LOG_INFO("Module World destructed");
}

WorldManager& World::getManager()
{
	return _worldManager;
}

const WorldManager& World::getManager() const
{
	return _worldManager;
}

}
