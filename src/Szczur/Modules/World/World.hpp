#pragma once

#include "WorldManager.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class World : public Module<>
{
public:

	///
	World();

	///
	World(const World&) = delete;

	///
	World& operator = (const World&) = delete;

	///
	World(World&&) = delete;

	///
	World& operator = (World&&) = delete;

	///
	~World();

	///
	world::WorldManager& getManager();

	///
	const world::WorldManager& getManager() const;

private:

	world::WorldManager _worldManager;

};

}
