#pragma once

#include <memory>

#include "Szczur/Utility/SFML3D/Drawable.hpp"

#include "../Component.hpp"

namespace rat
{
class Trace;
class Timeline;
class Script;

class TraceComponent : public Component, public sf3d::Drawable
{
public:

// Constructors

	///
	TraceComponent(Entity* parent);

// Getters

	///
	Trace* getTrace() { return _trace.get(); }

	///
	virtual void* getFeature(Component::Feature_e feature) override;

	///
	virtual const void* getFeature(Component::Feature_e feature) const override;

// Manipulations

	///
	void pause();

	///
	void resume();

// Config

	///
	virtual void loadFromConfig(const Json& config);

	///
	virtual void saveToConfig(Json& config) const;

// Main

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	///
	void update(float deltaTime);

	///
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;
	
	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

// Script

	///
	static void initScript(Script& script);

private:

	std::shared_ptr<Trace> _trace;

};

}
