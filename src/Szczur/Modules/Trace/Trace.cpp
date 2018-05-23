#include "Trace.hpp"

#include "Timeline.hpp"

namespace rat
{

Trace::Trace()
{

}

Trace::~Trace()
{

}

void Trace::addTimeline(Timeline* timeline)
{
	_timelines.push_back(std::unique_ptr<Timeline>(timeline));
}

void Trace::removeTimeline(Timeline* timeline)
{
	// TODO
}

void Trace::setCurrentTimeline(Timeline* timeline)
{
	_currentTimeline = timeline;

	if (_currentTimeline)
		_currentTimeline->start();
}

void Trace::update(float deltaTime)
{
	if (_currentTimeline)
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
