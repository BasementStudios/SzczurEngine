#pragma once

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

	glm::vec3 _progress;
	glm::vec3 _progressSpeed;

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

			auto pos = Start + _delta * _progress;

			_entity->setPosition(pos);

			if (_progress.x >= 1.f && _progress.y >= 1.f && _progress.z >= 1.f)
			{
				_finished = true;
			}
		}
	}

	virtual void start() override
	{
		Action::start();

		// reset progress
		_progress = glm::vec3();

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

		// calc progress speed
		_delta = _endPos - Start;

		if (_delta.x != 0.f)
			_progressSpeed.x = std::abs(Speed / _delta.x);
		else
			_progress.x = 1.f;

		if (_delta.y != 0.f)
			_progressSpeed.y = std::abs(Speed / _delta.y);
		else
			_progress.y = 1.f;

		if (_delta.z != 0.f)
			_progressSpeed.z = std::abs(Speed / _delta.z);
		else
			_progress.z = 1.f;
	}
};

}
