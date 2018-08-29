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

world::WorldManager& World::getManager()
{
	return _worldManager;
}

const world::WorldManager& World::getManager() const
{
	return _worldManager;
}

}
