#pragma once

#include <glm/glm.hpp>

namespace rat
{

class Entity;

class Action
{
public:
	enum Type
	{
		Move,
		Anim,
		Wait
	};

public:
	glm::vec3 Color = { 1.f, 1.f, 1.f };

protected:
	Type _type;
	Entity* _entity = nullptr;

	bool _finished = false;

public:
	Action(Entity* entity, Type type)
		: _entity(entity), _type(type)
	{

	}
	virtual ~Action() = default;

	virtual void update(float deltaTime) = 0;

	virtual void start()
	{
		_finished = false;
	}

	virtual bool isFinished() { return _finished; }

	auto getType() { return _type; }
};

}
