#pragma once

#include <vector>
#include <memory> // unique_ptr

#include <nlohmann/json_fwd.hpp>

namespace sf3d {
	class RenderTarget;
	class RenderStates;
}
namespace rat {
	class Entity;
	class Timeline;
}

namespace rat
{

class Trace
{
private:
	Entity* _entity = nullptr;

	std::vector<std::unique_ptr<Timeline>> _timelines;

	int _lastId = -1;

	Timeline* _currentTimeline = nullptr;

public:
	Trace(Entity* entity);
	~Trace();

	Timeline* addTimeline();
	void removeTimeline(Timeline* timeline);

	void setCurrentTimeline(Timeline* timeline);
	auto getCurrentTimeline() { return _currentTimeline; }

	auto& getTimelines() { return _timelines; }

	void loadFromConfig(nlohmann::json& config);
	void saveToConfig(nlohmann::json& config) const;

	void update(float deltaTime);

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;
};

}
