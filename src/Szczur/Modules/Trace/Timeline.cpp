#include "Timeline.hpp"

#include "Actions/AnimAction.hpp"
#include "Actions/MoveAction.hpp"

namespace rat
{

Timeline::Timeline()
{
	_vertexArray.setPrimitveType(GL_LINE_STRIP);
}

Timeline::~Timeline()
{
}

void Timeline::addAction(Action* action)
{
	if (action->getType() == Action::Move)
	{
		_vertexArray.resize(_vertexArray.getSize() + 2);
	}

	_actions.push_back(std::unique_ptr<Action>(action));

}

void Timeline::removeAction(Action* action)
{
	// TOOD
}

void Timeline::update(float deltaTime)
{
	if (!_finished && _currentActionIndex < _actions.size())
	{
		auto& currentAction = _actions[_currentActionIndex];
		currentAction->update(deltaTime);

		if (currentAction->isFinished())
		{
			if (_currentActionIndex < _actions.size() - 1)
			{
				_currentActionIndex++;

				//LOG_INFO("Current action index: ", _currentActionIndex);

				auto& currentAction = _actions[_currentActionIndex];
				currentAction->start();
			}
			else
			{
				if (Loop)
				{
					start();
				}
				else
				{
					_finished = true;
				}
			}
		}
	}

	if (ShowLines)
	{
		int i = 0;

		for (auto& action : _actions)
		{
			if (action->getType() == Action::Move)
			{
				auto moveAction = static_cast<MoveAction*>(action.get());

				sf3d::Vertex vertex;
				vertex.position = moveAction->Start;
				_vertexArray.set(i, vertex);

				i++;

				vertex.position = moveAction->End;
				_vertexArray.set(i, vertex);

				i++;
			}
		}
	}
}

void Timeline::start()
{
	_finished = false;
	_currentActionIndex = 0;
	auto& currentAction = _actions[_currentActionIndex];
	currentAction->start();

	//LOG_INFO("Current action index: ", _currentActionIndex);
}

void Timeline::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	if (ShowLines)
	{
		glLineWidth(2.f);
		target.draw(_vertexArray, states);
	}
}

}
