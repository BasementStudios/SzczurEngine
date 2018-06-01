#pragma once

#include <string>

#include "Szczur/Modules/DragonBones/SF3DArmatureDisplay.hpp"

#include "../Action.hpp"

#include "Szczur/Modules/World/Entity.hpp"

namespace rat
{

class AnimAction : public Action
{
public:
	std::string AnimationName;

	float FadeInTime = 0.5f;

	bool PlayOnce = false;
	bool WaitToEnd = false;

	bool FlipX = false;

public:
	AnimAction(Entity* entity)
		: Action(entity, Action::Type::Anim)
	{

	}

	virtual ~AnimAction()
	{

	}

	void update(float deltaTime, Timeline* timeline) override
	{
		if (_entity->hasComponent<ArmatureComponent>())
		{
			auto armature = _entity->getComponentAs<ArmatureComponent>()->getArmature();

			armature->getArmature()->setFlipX(FlipX);

			if (WaitToEnd && armature->getAnimation()->isCompleted())
			{
				_finished = true;
			}
		}
		else
		{
			_finished = true;
		}
	}

	void start() override
	{
		Action::start();

		_entity->getComponentAs<ArmatureComponent>()->fadeIn(AnimationName, FadeInTime, PlayOnce ? 1 : 0);

		if (!WaitToEnd)
			_finished = true;
	}
};

}
