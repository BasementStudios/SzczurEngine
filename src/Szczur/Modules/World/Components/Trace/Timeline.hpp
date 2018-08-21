#pragma once

#include <memory>
#include <vector>

#include <sol.hpp>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"

#include "Action.hpp"

namespace rat
{
class Script;

class Timeline
{

	using Container_t = std::vector<std::unique_ptr<Action>>;

public:
	enum Status
	{
		Playing = 0,
		Paused,
		Stopped
	};

	bool Loop = false;

	bool ShowLines = true;

	float SpeedMultiplier = 1.f;

private:
	Entity* _entity = nullptr;

	int _id;

	Container_t _actions;

	int _currentActionIndex = 0;

	Status _status = Stopped;

	sf3d::VertexArray _vertexArray{ 0 };

	// lua callbacks
	sol::function _onPlayCallback;
	sol::function _onPauseCallback;
	sol::function _onResumeCallback;
	sol::function _onStopCallback;

public:
	Timeline(int id, const std::string& name, Entity* entity);
	~Timeline();

	void addAction(Action* action);
	void removeAction(Action* action);

	void update(float deltaTime);

	void updateVertexArray();

	void start();

	void pause();
	
	void resume();
	
	void stop();

	const Status& getStatus() const { return _status; }

	auto& getActions() { return _actions; }

	const auto& getId() const { return _id; }

	const auto& getName() const { return _name; }

	void changeVertexArraySize(size_t newSize) { _vertexArray.resize(newSize); }

	size_t getVertexArraySize() { return _vertexArray.getSize(); }

	Entity* getEntity() { return _entity; }

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;

	static void initScript(Script& script);

// Scripts helper
protected:
	// Move to pos
	void addMoveAction(const glm::vec3& pos, float speed, bool relative);

	// Move to random pos
	void addMoveAction(const glm::vec3& rangeStart, const glm::vec3& rangeEnd, float speed, bool relative);
};

}
