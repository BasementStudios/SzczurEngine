#pragma once

#include <vector>
#include <memory>

#include <Json/json.hpp>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

using Json = nlohmann::json;

namespace rat
{

class Entity;
class Timeline;

class Trace
{
private:
	std::vector<std::unique_ptr<Timeline>> _timelines;

	int _lastId = -1;

	bool _pause = false;

	Timeline* _currentTimeline = nullptr;

public:
	Trace();
	~Trace();

	void addTimeline();
	void removeTimeline(Timeline* timeline);

	void setCurrentTimeline(Timeline* timeline);
	auto getCurrentTimeline() { return _currentTimeline; }

	auto& getTimelines() { return _timelines; }

	void pause();
	void resume();

	bool isPlaying() { return !_pause; }

	void loadFromConfig(const Json& config, Entity* entity);
	void saveToConfig(Json& config) const;

	void update(float deltaTime);

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;
};

}
