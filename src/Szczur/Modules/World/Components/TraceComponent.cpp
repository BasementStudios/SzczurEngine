#include "TraceComponent.hpp"

#include "Szczur/Modules/Trace/Trace.hpp"
#include "Szczur/Modules/Trace/Timeline.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{

TraceComponent::TraceComponent(Entity* parent)
  : Component { parent, fnv1a_64("TraceComponent"), "TraceComponent", Component::Drawable}
{
	_trace = std::make_shared<Trace>();
}

void* TraceComponent::getFeature(Component::Feature_e feature)
{
	if (feature == Component::Drawable) 
		return static_cast<sf3d::Drawable*>(this);

	return nullptr;
}

const void* TraceComponent::getFeature(Component::Feature_e feature) const
{
	if (feature == Component::Drawable) 
		return static_cast<const sf3d::Drawable*>(this);

	return nullptr;
}

void TraceComponent::pause()
{
	_trace->pause();
}

void TraceComponent::resume()
{
	_trace->resume();
}

void TraceComponent::setTimeline(int id)
{
	auto& timelines = _trace->getTimelines();

	auto timeline = std::find_if(timelines.begin(), timelines.end(), [id] (auto& timeline) { return id == timeline->getId(); });

	if (*timeline)
		_trace->setCurrentTimeline((*timeline).get());
}

void TraceComponent::stop()
{
	_trace->setCurrentTimeline(nullptr);
}

void TraceComponent::loadFromConfig(const Json& config)
{
	Component::loadFromConfig(config);
	
	_trace->loadFromConfig(config, this->getEntity());
}

void TraceComponent::saveToConfig(Json& config) const
{
	Component::saveToConfig(config);

	_trace->saveToConfig(config);
}

void TraceComponent::update(float deltaTime)
{
	if (_trace)
	{
		_trace->update(deltaTime);
	}
}

void TraceComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	_trace->draw(target, states);
}

void TraceComponent::initScript(Script& script)
{
	auto object = script.newClass<TraceComponent>("TraceComponent", "World");
	object.set("pause", &TraceComponent::pause);
	object.set("resume", &TraceComponent::resume);
	object.set("setTimeline", &TraceComponent::setTimeline);
	object.set("stop", &TraceComponent::stop);
	object.init();
}

std::unique_ptr<Component> TraceComponent::copy(Entity* newParent) const
{
	auto ptr = std::make_unique<TraceComponent>(*this);

	ptr->setEntity(newParent);

	return ptr;
}

}
