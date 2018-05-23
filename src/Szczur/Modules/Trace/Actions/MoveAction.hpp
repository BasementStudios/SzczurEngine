#pragma once

#include <glm/glm.hpp>

#include "../Action.hpp"

#include "Szczur/Modules/World/Entity.hpp"

namespace rat
{

class MoveAction : public Action
{
public:
	glm::vec3 Start;
	glm::vec3 End;

	float Speed = 1.f;

	bool UseCurrentPosition = true;

	bool Teleport = false;

private:
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
			_entity->setPosition(End);
			_finished = true;
		}
		else
		{
			_delta = End - Start;

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

		_progress = glm::vec3();

		if (UseCurrentPosition)
		{
			Start = _entity->getPosition();
		}
	}
};

}
