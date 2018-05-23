#pragma once

#include <vector>
#include <memory>

#include "Szczur/Utility/Modules/Module.hpp"

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

namespace rat
{

class Entity;
class Timeline;

class Trace : public Module<>
{
private:
	std::vector<std::unique_ptr<Timeline>> _timelines;

	Timeline *_currentTimeline = nullptr;

public:
	Trace();
	Trace(const Trace&) = delete;
	Trace& operator = (const Trace&) = delete;
	Trace(Trace&&) = delete;
	Trace& operator = (Trace&&) = delete;

	~Trace();

	void addTimeline(Timeline* timeline);
	void removeTimeline(Timeline* timeline);

	void setCurrentTimeline(Timeline* timeline);

	auto& getTimelines() { return _timelines; }

	void update(float deltaTime);

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;
};

}
