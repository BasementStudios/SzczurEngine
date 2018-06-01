#pragma once

#include <cmath>
#include <glm/glm.hpp>

#include "../Action.hpp"

#include "Szczur/Modules/World/Entity.hpp"

namespace rat
{

class MoveAction : public Action
{
public:
	bool UseCurrentPosition = true;
	glm::vec3 Start;

	bool EndRelativeToStart = true;
	glm::vec3 End;

	float Speed = 1.f;

	bool Teleport = false;

private:
	glm::vec3 _endPos;

	float _progress;
	float _progressSpeed;

	glm::vec3 _delta;

public:
	MoveAction(Entity* entity)
		: Action(entity, Action::Type::Move)
	{

	}
	virtual ~MoveAction()
	{

	}

	virtual void update(float deltaTime) override
	{
		if (Teleport)
		{
			_entity->setPosition(_endPos);
			_finished = true;
		}
		else
		{
			_progress += _progressSpeed * deltaTime * 100.f;

			if (_progress >= 1.f)
			{
				_finished = true;
				_progress = 1.f;
			}

			auto pos = Start + _delta * _progress;

			_entity->setPosition(pos);
		}
	}

	virtual void start() override
	{
		Action::start();

		// reset progress speed
		_progressSpeed = 0.f;

		// reset progress
		_progress = 0.f;

		// calc start pos
		if (UseCurrentPosition)
		{
			Start = _entity->getPosition();
		}

		// calc end pos
		if (EndRelativeToStart)
		{
			_endPos = Start + End;
		}
		else
		{
			_endPos = End;
		}

		// calc delta
		_delta = _endPos - Start;

		// calc distance
		float d = sqrt(pow(_delta.x, 2) + pow(_delta.y, 2) + pow(_delta.z, 2));

		// calc speed
		_progressSpeed = Speed / d;
	}
};

}
