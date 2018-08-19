#include "MoveAction.hpp"

#include <cmath>

#include "Szczur/Modules/World/Components/Trace/Timeline.hpp"
#include "Szczur/Modules/World/Entity.hpp"

#include "Szczur/Utility/Random.hpp"

namespace rat
{
MoveAction::MoveAction(Entity* entity)
	: Action(entity, Action::Type::Move)
{
}
void MoveAction::update(float deltaTime, Timeline* timeline)
{
	if (Teleport)
	{
		_entity->setPosition(_endPos);
		_finished = true;
	}
	else
	{
		_progress += _progressSpeed * timeline->SpeedMultiplier * deltaTime * 60.f;

		if (_progress >= 1.f)
		{
			_finished = true;
			_progress = 1.f;
		}

		auto velocity = (_startPos + _delta * _progress) - _entity->getPosition();

		// if entity has collider then move with it
		if (_entity->hasComponent<ColliderComponent>())
			_entity->getComponentAs<ColliderComponent>()->move(velocity.x, velocity.y, velocity.z);
		else
			_entity->move(velocity);
	}
}

void MoveAction::start()
{
	Action::start();

	// reset progress speed
	_progressSpeed = 0.f;

	// reset progress
	_progress = 0.f;

	// calc position for end and start
	calcPosition(Start, _entity->getPosition(), _startPos);
	calcPosition(End, _startPos, _endPos);

	// calc delta
	_delta = _endPos - _startPos;

	// calc distance
	float d = sqrt(pow(_delta.x, 2) + pow(_delta.y, 2) + pow(_delta.z, 2));

	// calc speed
	_progressSpeed = Speed / d;
}

void MoveAction::calcPosition(const MovePosition& position, const glm::vec3& relativeValue, glm::vec3& outPosition)
{
	if (position.Random)
	{
		Random random;

		// if start is random then set random value between RangeStart and RangeEnd
		outPosition.x = random.get(position.RangeStart.x, position.RangeEnd.x);
		outPosition.y = random.get(position.RangeStart.y, position.RangeEnd.y);
		outPosition.z = random.get(position.RangeStart.z, position.RangeEnd.z);
	}
	else
	{
		// if start isn't random then just set value
		outPosition = position.Value;
	}

	// if start is relative then add current position
	if (position.Relative)
	{
		outPosition += relativeValue;
	}
}

}
