#pragma once

#include <SFML/System/Clock.hpp>

#include "../Action.hpp"

#include "Szczur/Utility/Random.hpp"

namespace rat
{

class WaitAction : public Action
{
public:
	bool Random;

	union
	{
		// if Random is false
		float TimeToWait = 0.f;

		// if Random is true
		struct
		{
			float RangeStart;
			float RangeEnd;
		};
	};
private:
	float _timeToWait = 0.f;
	sf::Clock _clock;

public:
	WaitAction()
		: Action(Action::Type::Wait)
	{

	}
	virtual ~WaitAction()
	{

	}

	virtual void update(float deltaTime, Timeline* timeline) override
	{
		if (_clock.getElapsedTime().asSeconds() > TimeToWait)
		{
			_finished = true;
		}
	}

	virtual void start() override
	{
		Action::start();

		rat::Random random;

		if (Random)
		{
			_timeToWait = random.get(RangeStart, RangeEnd);
		}
		else
		{
			_timeToWait = TimeToWait;
		}

		_clock.restart();
	}
};

}
