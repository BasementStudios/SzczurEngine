#include "MoveAction.hpp"

#include <cmath>

#include "Szczur/Modules/World/Components/Trace/Timeline.hpp"
#include "Szczur/Modules/World/Entity.hpp"

#include "Szczur/Utility/Random.hpp"

namespace rat
{
MoveAction::MoveAction()
	: Action(Action::Type::Move)
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

void MoveAction::init()
{
	Start.CalcResult();
	End.CalcResult();
}

void MoveAction::start()
{
	Action::start();

	// reset progress speed
	_progressSpeed = 0.f;

	// reset progress
	_progress = 0.f;

	// set positions
	_startPos = Start.Result;

	if (Start.Relative)
	{
		_startPos += _entity->getPosition();
	}

	_endPos = End.Result;

	if (End.Relative)
	{
		_endPos += _startPos;
	}

	// calc delta
	_delta = _endPos - _startPos;

	// calc distance
	float d = sqrt(pow(_delta.x, 2) + pow(_delta.y, 2) + pow(_delta.z, 2));

	// calc speed
	_progressSpeed = Speed / d;
}

void MovePosition::CalcResult()
{
	if (Random)
	{
		rat::Random random;

		// if start is random then set random value between RangeStart and RangeEnd
		Result.x = random.get(RangeStart.x, RangeEnd.x);
		Result.y = random.get(RangeStart.y, RangeEnd.y);
		Result.z = random.get(RangeStart.z, RangeEnd.z);
	}
	else
	{
		// if start isn't random then just set value
		Result = Value;
	}
}

}
