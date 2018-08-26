#include "Timeline.hpp"

#include "Actions/MoveAction.hpp"
#include "Actions/WaitAction.hpp"
#include "Actions/ScriptAction.hpp"
#include "Actions/AnimAction.hpp"

#include "Szczur/Utility/Random.hpp"

namespace rat
{

Timeline::Timeline(int id, const std::string& name, Entity* entity)
	: _id(id), _name(name), _entity(entity)
{
	_vertexArray.setPrimitiveType(sf3d::PrimitiveType::Lines);

	{
		auto moveAction = new MoveAction();
		moveAction->Start.Relative = false;
		addAction(moveAction);
	}
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

	action->_entity = _entity;

	Random random;

	float min = 10.f, max = 150.f;

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

				auto& currentAction = _actions[_currentActionIndex];
				currentAction->start();

				// update line
				// updateVertexArray();
			}
			else
			{
				// update line
				// updateVertexArray();

				if (Loop)
				{
					start();
				}
				else
				{
					stop();
				}
			}
		}
	}
}

void Timeline::updateVertexArray()
{
	glm::vec3 lastPosition = _entity->getPosition();

	auto setVertex = [&] (int idx, const MovePosition& movePos, const glm::vec4& color) {
		auto pos = movePos.Result;

		if (movePos.Relative)
			pos += lastPosition;

		lastPosition = pos;

		sf3d::Vertex& vert = _vertexArray.getVertex(idx);
		vert.position = pos;
		vert.color = color;
	};

	int i = 0;

	for (auto& action : _actions)
	{
		if (action->getType() == Action::Move)
		{
			auto moveAction = static_cast<MoveAction*>(action.get());

			setVertex(i++, moveAction->Start, glm::vec4(action->Color, 1.f));
			setVertex(i++, moveAction->End, glm::vec4(action->Color, 1.f));
		}
	}
}

void Timeline::start()
{
	if (_actions.size() == 0)
		return;

	// call init
	for (auto& action : _actions)
	{
		action->init();
	}

	// create line which represents path which entity will be going
	updateVertexArray();

	_status = Status::Playing;

	if (_onPlayCallback.valid())
		_onPlayCallback();

	_currentActionIndex = 0;
	auto& currentAction = _actions[_currentActionIndex];
	currentAction->start();
}

void Timeline::pause()
{
	if (_onPauseCallback.valid())
		_onPauseCallback();

	_status = Status::Paused;
}

void Timeline::resume()
{
	if (_onResumeCallback.valid())
		_onResumeCallback();

	_status = Status::Playing;
}

void Timeline::stop()
{
	if (_onStopCallback.valid())
		_onStopCallback();

	_status = Status::Stopped;
}

void Timeline::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	if (ShowLines)
	{
		glLineWidth(2.f);
		target.draw(_vertexArray, states);
	}
}

void Timeline::addMoveAction(const glm::vec3& pos, float speed, bool relative, bool teleport)
{
	if (speed <= 0.f) 
		speed = 1.f;

	auto action = new MoveAction();
	action->End.Value = pos;
	action->End.Relative = relative;
	action->Speed = speed;
	action->Teleport = teleport;
	if (teleport)
	{
		// blue
		action->Color = glm::vec3(0.12f, 0.53f, 0.9f);
	}

	addAction(action);
}

void Timeline::addMoveAction(const glm::vec3& rangeStart, const glm::vec3& rangeEnd, float speed, bool relative, bool teleport)
{
	if (speed <= 0.f) 
		speed = 1.f;

	auto action = new MoveAction();
	action->End.Random = true;
	action->End.RangeStart = rangeStart;
	action->End.RangeEnd = rangeEnd;
	action->End.Relative = relative;
	action->Speed = speed;
	action->Teleport = teleport;
	if (teleport)
	{
		// blue
		action->Color = glm::vec3(0.12f, 0.53f, 0.9f);
	}

	addAction(action);
}

void Timeline::initScript(Script& script)
{
	auto object = script.newClass<Timeline>("Timeline", "World");

	object.set("relativeStart", [&] (Timeline* thisa, bool relativeStart) {
		static_cast<MoveAction*>(thisa->getActions().front().get())->Start.Relative = relativeStart;
	});

	object.set("setLoop", [&] (Timeline* thisa, bool loop) {
		thisa->Loop = loop;
	});

	object.set("setSpeedMultipler", [&] (Timeline* thisa, float speed) {
		thisa->SpeedMultiplier = speed;
	});

	object.set("move", sol::overload(
		[&] (Timeline* thisa, float x, float y, float z) { // move with offset
		thisa->addMoveAction({ x, y, z }, 1.f, true);
		},
		[&](Timeline* thisa, float x, float y, float z, float speed = 1.f) { // move with offset with speed
			thisa->addMoveAction({ x, y, z }, speed, true);
		},
		[&] (Timeline* thisa, float startX, float startY, float startZ, float endX, float endY, float endZ) { // move with random offset
			thisa->addMoveAction({ startX, startY, startZ }, { endX, endY, endZ }, 1.f, true);
		},
		[&] (Timeline* thisa, float startX, float startY, float startZ, float endX, float endY, float endZ, float speed) { // move with random offset with speed
			thisa->addMoveAction({ startX, startY, startZ }, { endX, endY, endZ }, speed, true);
		}
	));

	object.set("moveTo", sol::overload(
		[&] (Timeline* thisa, float x, float y, float z) { // move to point
		thisa->addMoveAction({ x, y, z }, 1.f, false);
		},
		[&] (Timeline* thisa, float x, float y, float z, float speed) { // move to point with speed
			thisa->addMoveAction({ x, y, z }, speed, false);
		},
		[&] (Timeline* thisa, float startX, float startY, float startZ, float endX, float endY, float endZ) { // move to random point
			thisa->addMoveAction({ startX, startY, startZ }, { endX, endY, endZ }, 1.f, false);
		},
		[&] (Timeline* thisa, float startX, float startY, float startZ, float endX, float endY, float endZ, float speed) { // move to random point with speed
			thisa->addMoveAction({ startX, startY, startZ }, { endX, endY, endZ }, speed, false);
		}
	));

	object.set("teleport", sol::overload(
		[&] (Timeline* thisa, float x, float y, float z) { // teleport with offset
			thisa->addMoveAction({ x, y, z }, 0.f, true, true);
		},
		[&] (Timeline* thisa, float startX, float startY, float startZ, float endX, float endY, float endZ) { // teleport with random offset
			thisa->addMoveAction({ startX, startY, startZ }, { endX, endY, endZ }, 0.f, true, true);
		}
	));

	object.set("teleportTo", sol::overload(
		[&] (Timeline* thisa, float x, float y, float z) { // teleport to point
			thisa->addMoveAction({ x, y, z }, 0.f, false, true);
		},
		[&] (Timeline* thisa, float startX, float startY, float startZ, float endX, float endY, float endZ) { // teleport to random point
			thisa->addMoveAction({ startX, startY, startZ }, { endX, endY, endZ }, 0.f, false, true);
		}
	));

	object.set("wait", sol::overload(
		[&] (Timeline* thisa, float timeToWait) {
			auto waitAction = new WaitAction();
			waitAction->TimeToWait = timeToWait;
			thisa->addAction(waitAction);
		},
		[&] (Timeline* thisa, float start, float end) {
			auto waitAction = new WaitAction();
			waitAction->RangeStart = start;
			waitAction->RangeEnd = end;
			thisa->addAction(waitAction);
		}
	));

	object.set("call", sol::overload(
		[&] (Timeline* thisa, sol::function func) {
			auto scriptAction = new ScriptAction();
			scriptAction->FunctionToCall = func;
			thisa->addAction(scriptAction);
		},
		[&] (Timeline* thisa, const std::string& scriptFile) {
			auto scriptAction = new ScriptAction();
			scriptAction->ScriptFilePath = scriptFile;
			thisa->addAction(scriptAction);
		}
	));

	object.set("playAnim", [&] (Timeline* thisa, const std::string& animationName, float fadeInTime) {
		auto animAction = new AnimAction();
		animAction->AnimationName = animationName;
		animAction->FadeInTime = fadeInTime;
		thisa->addAction(animAction);
	});

	object.set("playOnce", [&] (Timeline* thisa, const std::string& animationName, float fadeInTime, bool waitToEnd) {
		auto animAction = new AnimAction();
		animAction->AnimationName = animationName;
		animAction->FadeInTime = fadeInTime;
		animAction->PlayOnce = true;
		animAction->WaitToEnd = waitToEnd;
		thisa->addAction(animAction);
	});

	// calllback
	object.setProperty("onPlay", []{}, [&] (Timeline& thisa, sol::function callback) {
		thisa._onPlayCallback = callback;
	});

	object.setProperty("onPause", []{}, [&] (Timeline& thisa, sol::function callback) {
		thisa._onPauseCallback = callback;
	});

	object.setProperty("onResume", []{}, [&] (Timeline& thisa, sol::function callback) {
		thisa._onResumeCallback = callback;
	});

	object.setProperty("onStop", [] {}, [&] (Timeline& thisa, sol::function callback) {
		thisa._onStopCallback = callback;
	});

	object.init();
}

}
