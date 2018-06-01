#include "Trace.hpp"

#include "Timeline.hpp"

#include "Actions/AnimAction.hpp"
#include "Actions/MoveAction.hpp"
#include "Actions/WaitAction.hpp"

namespace rat
{

Trace::Trace()
{

}

Trace::~Trace()
{

}

void Trace::addTimeline()
{
	_timelines.emplace_back(std::make_unique<Timeline>(++_lastId));
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

	if (_currentTimeline)
		_currentTimeline->start();
}

void Trace::pause()
{
	_pause = true;
}

void Trace::resume()
{
	_pause = false;
}

void Trace::loadFromConfig(Json& config, Entity* entity)
{
	Json::array_t jsonTimelines = config["timelines"];

	_lastId = config["lastId"];

	for (auto& jsonTimeline : jsonTimelines)
	{
		auto timeline = std::make_unique<Timeline>(jsonTimeline["id"].get<int>());
		timeline->Loop = jsonTimeline["loop"];

		Json::array_t jsonActions = jsonTimeline["actions"];

		for (auto& jsonAction : jsonActions)
		{
			switch (jsonAction["type"].get<Action::Type>())
			{
				case Action::Move:
				{
					auto moveAction = new MoveAction(entity);

					moveAction->UseCurrentPosition = jsonAction["useCurrentPosition"];
					moveAction->Start.x = jsonAction["start"]["x"];
					moveAction->Start.y = jsonAction["start"]["y"];
					moveAction->Start.z = jsonAction["start"]["z"];

					moveAction->EndRelativeToStart = jsonAction["endRelativeToStart"];
					moveAction->End.x = jsonAction["end"]["x"];
					moveAction->End.y = jsonAction["end"]["y"];
					moveAction->End.z = jsonAction["end"]["z"];

					moveAction->Speed = jsonAction["speed"];
					moveAction->Teleport = jsonAction["teleport"];

					timeline->addAction(moveAction);
				} break;
				case Action::Anim:
				{
					auto animAction = new AnimAction(entity);

					animAction->AnimationName = jsonAction["animationName"].get<std::string>();
					animAction->FadeInTime = jsonAction["FadeInTime"];
					animAction->PlayOnce = jsonAction["playOnce"];
					animAction->FlipX = jsonAction["FlipX"];

					timeline->addAction(animAction);
				} break;
				case Action::Wait:
				{
					auto waitAction = new WaitAction(entity);

					waitAction->TimeToWait = jsonAction["timeToWait"];

					timeline->addAction(waitAction);
				} break;
			}
		}

		_timelines.push_back(std::move(timeline));
	}
}

void Trace::saveToConfig(Json& config) const
{
	if (_currentTimeline)
		config["currentTimeline"] = _currentTimeline->getId();

	config["lastId"] = _lastId;

	auto jsonTimelines = Json::array();

	for (auto& timeline : _timelines)
	{
		auto jsonTimeline = Json::object();

		jsonTimeline["id"] = timeline->getId();
		jsonTimeline["loop"] = timeline->Loop;

		auto jsonActions = Json::array();

		for (auto& action : timeline->getActions())
		{
			auto jsonAction = Json::object();
			
			jsonAction["type"] = action->getType();
			switch (action->getType())
			{
				case Action::Move:
				{
					auto moveAction = static_cast<MoveAction*>(action.get());
					
					jsonAction["useCurrentPosition"] = moveAction->UseCurrentPosition;
					jsonAction["start"]["x"] = moveAction->Start.x;
					jsonAction["start"]["y"] = moveAction->Start.y;
					jsonAction["start"]["z"] = moveAction->Start.z;

					jsonAction["endRelativeToStart"] = moveAction->EndRelativeToStart;
					jsonAction["end"]["x"] = moveAction->End.x;
					jsonAction["end"]["y"] = moveAction->End.y;
					jsonAction["end"]["z"] = moveAction->End.z;

					jsonAction["speed"] = moveAction->Speed;
					jsonAction["teleport"] = moveAction->Teleport;
				} break;
				case Action::Anim:
				{
					auto animAction = static_cast<AnimAction*>(action.get());

					jsonAction["animationName"] = animAction->AnimationName;
					jsonAction["FadeInTime"] = animAction->FadeInTime;
					jsonAction["playOnce"] = animAction->PlayOnce;
					jsonAction["FlipX"] = animAction->FlipX;
				} break;
				case Action::Wait:
				{
					auto waitAction = static_cast<WaitAction*>(action.get());

					jsonAction["timeToWait"] = waitAction->TimeToWait;
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
	if (_currentTimeline && !_pause)
	{
		_currentTimeline->update(deltaTime);
	}
}

void Trace::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	for (auto& timeline : _timelines)
	{
		timeline->draw(target, states);
	}
}

}
