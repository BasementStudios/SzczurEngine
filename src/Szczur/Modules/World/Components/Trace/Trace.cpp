#include "Trace.hpp"

#include <nlohmann/json.hpp>

#include "Timeline.hpp"

#include "Actions/AnimAction.hpp"
#include "Actions/MoveAction.hpp"
#include "Actions/WaitAction.hpp"
#include "Actions/ScriptAction.hpp"

#include "Szczur/Modules/World/World.hpp"

namespace rat
{

Trace::Trace(Entity* _entity)
	: _entity(_entity)
{

}

Trace::~Trace()
{

}

Timeline* Trace::addTimeline()
{
	return _timelines.emplace_back(std::make_unique<Timeline>(++_lastId, _entity)).get();
}

void Trace::removeTimeline(Timeline* timeline)
{
	if (timeline == _currentTimeline)
	{
		_currentTimeline = nullptr;
	}

	_timelines.erase(std::remove_if(_timelines.begin(), _timelines.end(), [timeline] (auto& it) { return timeline == it.get(); }));
}

void Trace::setCurrentTimeline(Timeline* timeline)
{
	_currentTimeline = timeline;
}

void Trace::loadFromConfig(nlohmann::json& config)
{
	auto loadMoveActionPos = [&] (nlohmann::json::object_t jsonAction, Timeline* timeline) {
		MovePosition pos;

		// if json don't have this then don't have others
		if (auto it = jsonAction.find("relative"); it == jsonAction.end())
			return pos;

		pos.Relative = jsonAction["relative"];

		pos.Random = jsonAction["random"];

		if (pos.Random)
		{
			pos.RangeStart.x = jsonAction["rangeStart"]["x"];
			pos.RangeStart.y = jsonAction["rangeStart"]["y"];
			pos.RangeStart.z = jsonAction["rangeStart"]["z"];

			pos.RangeEnd.x = jsonAction["rangeEnd"]["x"];
			pos.RangeEnd.y = jsonAction["rangeEnd"]["y"];
			pos.RangeEnd.z = jsonAction["rangeEnd"]["z"];

			timeline->changeVertexArraySize(timeline->getVertexArraySize() + 2);
		}
		else
		{
			pos.Value.x = jsonAction["value"]["x"];
			pos.Value.y = jsonAction["value"]["y"];
			pos.Value.z = jsonAction["value"]["z"];
		}

		return pos;
	};

	nlohmann::json::array_t jsonTimelines = config["timelines"];

	_lastId = config["lastId"];

	for (auto& jsonTimeline : jsonTimelines)
	{
		auto timeline = std::make_unique<Timeline>(jsonTimeline["id"].get<int>(), _entity);
		timeline->Loop = jsonTimeline["loop"];

		if (auto it = jsonTimeline.find("speedMultiplier"); it != jsonTimeline.end())
		{
			timeline->SpeedMultiplier = it.value();
		}

		if (auto it = jsonTimeline.find("showLines"); it != jsonTimeline.end())
		{
			timeline->ShowLines = it.value();
		}

		nlohmann::json::array_t jsonActions = jsonTimeline["actions"];

		for (auto& jsonAction : jsonActions)
		{
			switch (jsonAction["type"].get<Action::Type>())
			{
				case Action::Move:
				{
					auto moveAction = new MoveAction();

					if (auto start = jsonAction.find("start"); start != jsonAction.end())
						moveAction->Start = loadMoveActionPos(start.value(), timeline.get());
					
					if (auto end = jsonAction.find("end"); end != jsonAction.end())
						moveAction->End = loadMoveActionPos(end.value(), timeline.get());

					moveAction->Speed = jsonAction["speed"];
					moveAction->Teleport = jsonAction["teleport"];

					timeline->addAction(moveAction);
				} break;
				case Action::Anim:
				{
					auto animAction = new AnimAction();

					animAction->AnimationName = jsonAction["animationName"].get<std::string>();
					animAction->FadeInTime = jsonAction["FadeInTime"];
					animAction->PlayOnce = jsonAction["playOnce"];
					animAction->FlipX = jsonAction["FlipX"];

					timeline->addAction(animAction);
				} break;
				case Action::Wait:
				{
					auto waitAction = new WaitAction();

					waitAction->TimeToWait = jsonAction["timeToWait"];

					timeline->addAction(waitAction);
				} break;
				case Action::Script:
				{
					auto scriptAction = new ScriptAction();

					scriptAction->ScriptFilePath = jsonAction["filePath"].get<std::string>();

					timeline->addAction(scriptAction);
				} break;
			}
		}

		_timelines.push_back(std::move(timeline));
	}

	if (auto it = config.find("currentTimeline"); it != config.end())
	{
		int currentTimelineId = it.value();

		auto timeline = std::find_if(_timelines.begin(), _timelines.end(), [&] (auto& timeline) { return currentTimelineId == timeline->getId(); });

		if (timeline != _timelines.end())
		{
			detail::globalPtr<World>->getScenes().isGameRunning();

			_currentTimeline = timeline->get();
			//_currentTimeline->start();
		}
	}
}

void Trace::saveToConfig(nlohmann::json& config) const
{
	auto saveMoveActionPos = [&](const MovePosition& pos) {
		auto jsonAction = nlohmann::json::object();

		jsonAction["relative"] = pos.Relative;

		jsonAction["random"] = pos.Random;

		if (pos.Random)
		{
			jsonAction["rangeStart"]["x"] = pos.RangeStart.x;
			jsonAction["rangeStart"]["y"] = pos.RangeStart.y;
			jsonAction["rangeStart"]["z"] = pos.RangeStart.z;

			jsonAction["rangeEnd"]["x"] = pos.RangeEnd.x;
			jsonAction["rangeEnd"]["y"] = pos.RangeEnd.y;
			jsonAction["rangeEnd"]["z"] = pos.RangeEnd.z;
		}
		else
		{
			jsonAction["value"]["x"] = pos.Value.x;
			jsonAction["value"]["y"] = pos.Value.y;
			jsonAction["value"]["z"] = pos.Value.z;
		}
		
		return jsonAction;
	};

	if (_currentTimeline)
		config["currentTimeline"] = _currentTimeline->getId();

	config["lastId"] = _lastId;

	auto jsonTimelines = nlohmann::json::array();

	for (auto& timeline : _timelines)
	{
		auto jsonTimeline = nlohmann::json::object();

		jsonTimeline["id"]              = timeline->getId();
		jsonTimeline["loop"]            = timeline->Loop;
		jsonTimeline["speedMultiplier"] = timeline->SpeedMultiplier;
		jsonTimeline["showLines"]       = timeline->ShowLines;

		auto jsonActions = nlohmann::json::array();

		for (auto& action : timeline->getActions())
		{
			auto jsonAction = nlohmann::json::object();
			
			jsonAction["type"] = action->getType();
			switch (action->getType())
			{
				case Action::Move:
				{
					auto moveAction = static_cast<MoveAction*>(action.get());

					jsonAction["start"] = saveMoveActionPos(moveAction->Start);
					jsonAction["end"]   = saveMoveActionPos(moveAction->End);

					jsonAction["speed"]    = moveAction->Speed;
					jsonAction["teleport"] = moveAction->Teleport;
				} break;
				case Action::Anim:
				{
					auto animAction = static_cast<AnimAction*>(action.get());

					jsonAction["animationName"] = animAction->AnimationName;
					jsonAction["FadeInTime"]    = animAction->FadeInTime;
					jsonAction["playOnce"]      = animAction->PlayOnce;
					jsonAction["FlipX"]         = animAction->FlipX;
				} break;
				case Action::Wait:
				{
					auto waitAction = static_cast<WaitAction*>(action.get());

					jsonAction["timeToWait"] = waitAction->TimeToWait;
				} break;
				case Action::Script:
				{
					auto scriptAction = static_cast<ScriptAction*>(action.get());

					jsonAction["filePath"] = scriptAction->ScriptFilePath;
				} break;
			}

			jsonActions.push_back(jsonAction);
		}

		jsonTimeline["actions"] = jsonActions;
		jsonTimelines.push_back(jsonTimeline);
	}

	config["timelines"] = jsonTimelines;
}

void Trace::update(float deltaTime)
{
	if (_currentTimeline)
	{
		_currentTimeline->update(deltaTime);
	}

	for (auto& timeline : _timelines)
	{
		if (timeline->ShowLines)
			timeline->updateVertexArray();
	}
}

void Trace::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	for (auto& timeline : _timelines)
	{
		if (timeline->ShowLines)
			timeline->draw(target, states);
	}
}

}
