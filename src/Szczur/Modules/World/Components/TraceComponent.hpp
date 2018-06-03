#pragma once

#include <memory>

#include "Szczur/Utility/SFML3D/Drawable.hpp"

#include "../Component.hpp"

namespace rat
{
class Trace;
class Timeline;
class Action;
class Script;

class TraceComponent : public Component
{
public:

// Constructors

	///
	TraceComponent(Entity* parent);

// Getters

	///
	Trace* getTrace() { return _trace.get(); }

// Manipulations

	///
	void pause();

	///
	void resume();

	///
	void setTimeline(int id);

	///
	void stop();

// Config

	///
	virtual void loadFromConfig(Json& config);

	///
	virtual void saveToConfig(Json& config) const;

// Main

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

    ///
    void update(ScenesManager& scenes, float deltaTime);

	///
	void render(sf3d::RenderTarget& target);
	
	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

// Script

	///
	static void initScript(Script& script);

private:

	std::shared_ptr<Trace> _trace;

	Timeline*	_currentTimeline = nullptr;
	Action*		_currentAction = nullptr;

};

}
