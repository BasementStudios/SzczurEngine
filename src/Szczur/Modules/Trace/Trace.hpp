#pragma once

#include <vector>
#include <memory>

#include <Json/json.hpp>

#include "Szczur/Utility/Modules/Module.hpp"

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

using Json = nlohmann::json;

namespace rat
{

class Entity;
class Timeline;

class Trace : public Module<>
{
private:
	std::vector<std::unique_ptr<Timeline>> _timelines;

	int _lastId = -1;

	Timeline* _currentTimeline = nullptr;

public:
	Trace();
	Trace(const Trace&) = delete;
	Trace& operator = (const Trace&) = delete;
	Trace(Trace&&) = delete;
	Trace& operator = (Trace&&) = delete;

	~Trace();

	void addTimeline();
	void removeTimeline(Timeline* timeline);

	void setCurrentTimeline(Timeline* timeline);

	auto& getTimelines() { return _timelines; }

	void loadFromConfig(const Json& config, Entity* entity);
	void saveToConfig(Json& config) const;

	void update(float deltaTime);

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;
};

}
