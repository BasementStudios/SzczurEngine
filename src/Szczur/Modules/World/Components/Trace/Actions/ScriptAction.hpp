#pragma once

#include "../Action.hpp"

#include "Szczur/Modules/World/Components/ScriptableComponent.hpp"
#include "Szczur/Modules/World/Entity.hpp"

namespace rat
{

class ScriptAction : public Action
{
public:
	std::string ScriptFilePath;

public:
	ScriptAction(Entity* entity)
		: Action(entity, Action::Type::Script)
	{
	}

	virtual ~ScriptAction()
	{
	}

	virtual void update(float deltaTime, Timeline* timeline) override
	{
	}

	void start() override
	{
		Action::start();

		auto script = _entity->getComponentAs<ScriptableComponent>();

		if (script)
		{
			try
			{
				script->runScript(ScriptFilePath);
			}
			catch (sol::error ex)
			{
				LOG_EXCEPTION(ex);
			}
			
			_finished = true;
		}
	}
};

}
