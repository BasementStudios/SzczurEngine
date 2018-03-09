#pragma once

#include <memory>

#include <dragonBones/SFMLFactory.h>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"

#include "Armature.hpp"

namespace rat 
{

class DragonBones : Module<Assets>
{ 
private:
	dragonBones::SFMLFactory _factory;

public:
	template <typename Tuple>
	DragonBones(Tuple&& tuple);

	void update(float deltaTime);

	Armature* createArmature(const std::string& actorName);

	void addSoundEvent(const std::function<void(dragonBones::EventObject*)>& listener) { _factory.addSoundEventListener(listener); }
};

template <typename Tuple>
DragonBones::DragonBones(Tuple&& tuple) :
	Module(tuple)
{

}

}
