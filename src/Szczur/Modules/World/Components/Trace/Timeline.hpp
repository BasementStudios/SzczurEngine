#pragma once

#include <memory>
#include <vector>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"

#include "Action.hpp"

namespace rat
{

class Timeline
{
	using Container_t = std::vector<std::unique_ptr<Action>>;

public:
	bool Loop = false;

	bool ShowLines = false;

	float SpeedMultiplier = 1.f;

private:
	int _id;

	Container_t _actions;

	int _currentActionIndex = 0;

	bool _finished = false;

	sf3d::VertexArray _vertexArray{ 0 };

public:
	Timeline(int id);
	~Timeline();

	void addAction(Action* action);
	void removeAction(Action* action);

	void update(float deltaTime);

	void updateVertexArray();

	void start();

	bool finished() { return _finished; }

	auto& getActions() { return _actions; }

	const auto getId() { return _id; }

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;
};

}
