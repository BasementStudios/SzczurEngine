#include "Timeline.hpp"

#include "Actions/AnimAction.hpp"
#include "Actions/MoveAction.hpp"

#include "Szczur/Utility/Random.hpp"

namespace rat
{

Timeline::Timeline(int id, Entity* entity)
	: _id(id), _entity(entity)
{
	_vertexArray.setPrimitiveType(sf3d::PrimitiveType::Lines);
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

	Random random;

	float min = 10.f, max = 150.f;

	action->ButtonColor = ImVec4(random.get(min, max) / 255.f, random.get(min, max) / 255.f, random.get(min, max) / 255.f, 1.f);

	_actions.push_back(std::unique_ptr<Action>(action));
}

void Timeline::removeAction(Action* action)
{
	if (action->getType() == Action::Move)
	{
		_vertexArray.resize(_vertexArray.getSize() - 2);
	}

	_actions.erase(std::remove_if(_actions.begin(), _actions.end(), [action] (auto& it) { return action == it.get(); }));
	_currentActionIndex = 0;
}

void Timeline::update(float deltaTime)
{
	if (_status == Status::Playing && _currentActionIndex < _actions.size())
	{
		auto& currentAction = _actions[_currentActionIndex];
		currentAction->update(deltaTime, this);

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
					_status = Status::Stopped;
				}
			}
		}
	}
}

void Timeline::updateVertexArray()
{
	int i = 0;

	glm::vec3 lastPosition = _entity->getPosition();

	auto setVertex = [&](int idx, const glm::vec3& pos, const glm::vec4& color) {
		sf3d::Vertex& vert = _vertexArray.getVertex(idx);
		vert.position = pos;
		vert.color = color;
	};

	auto renderRange = [&] (MovePosition& pos, const glm::vec3& offset) {
		setVertex(i++, pos.RangeStart + offset, glm::vec4(0.36f, 0.42f, 0.75f, 1.0f));
		setVertex(i++, pos.RangeEnd + offset, glm::vec4(0.36f, 0.42f, 0.75f, 1.0f));
	};

	for (auto& action : _actions)
	{
		if (action->getType() == Action::Move)
		{
			auto moveAction = static_cast<MoveAction*>(action.get());

			glm::vec3 start;

			if (moveAction->Start.Random)
			{
				auto& startPos = moveAction->Start;

				renderRange(startPos, lastPosition);
				start = (startPos.RangeStart + startPos.RangeEnd) / 2.f;
			}
			else
			{
				start = moveAction->Start.Value;
			}

			if (moveAction->Start.Relative)
				start += lastPosition;

			setVertex(i++, start, glm::vec4(action->Color, 1.f));


			glm::vec3 end;

			if (moveAction->End.Random)
			{
				end = (moveAction->End.RangeStart + moveAction->End.RangeEnd) / 2.f;
			}
			else
			{
				end = moveAction->End.Value;
			}

			if (moveAction->End.Relative)
				end += start;

			setVertex(i++, end, glm::vec4(action->Color, 1.f));

			lastPosition = end;

			if (moveAction->End.Random)
				renderRange(moveAction->End, start);
		}
	}
}

void Timeline::start()
{
	if (_actions.size() == 0)
		return;

	_status = Playing;
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
