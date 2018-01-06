#pragma once

#include <memory>

#include <dragonBones/SFMLFactory.h>

#include "Szczur/Utility/Modules.hpp"
#include "Szczur\Modules\Assets\Assets.hpp"

#include "Armature.hpp"

namespace rat 
{
	class DragonBones : Module<Assets>, Module<>::Updatable
	{ 
		using Module::Module;

	private:
		std::unique_ptr<dragonBones::SFMLFactory> _factory;

	public:
		void init();

		void update(float deltaTime);

		Armature* createArmature(const std::string& actorName);

		void addSoundEvent(const std::function<void(dragonBones::EventObject*)>& listener) { _factory->addSoundEventListener(listener); }
	};
}
