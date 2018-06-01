#pragma once

#include <glm/glm.hpp>
#include <SFML/System/Clock.hpp>

#include "../Action.hpp"

namespace rat
{

class WaitAction : public Action
{
public:
	float TimeToWait = 0.f;

private:
	sf::Clock _clock;

public:
	WaitAction(Entity* entity)
		: Action(entity, Action::Type::Wait)
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

		_clock.restart();
	}
};

}
