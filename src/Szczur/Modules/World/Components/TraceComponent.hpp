#pragma once

#include <memory>

#include "Szczur/Utility/SFML3D/Drawable.hpp"

#include "Szczur/Modules/Trace/Trace.hpp"

#include "../Component.hpp"

namespace rat
{
class Trace;
class Timeline;

class TraceComponent : public Component, public sf3d::Drawable
{
private:
	std::shared_ptr<Trace> _trace;

public:
	TraceComponent(Entity* parent);

	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	Trace* getTrace() { return _trace.get(); }

	virtual void* getFeature(Component::Feature_e feature) override;

	virtual const void* getFeature(Component::Feature_e feature) const override;

	void update(float deltaTime);

	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;
};

}
