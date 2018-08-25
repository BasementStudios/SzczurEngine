#pragma once

#include "../Action.hpp"

namespace rat
{

struct MovePosition
{
public:
	// relative to current position
	bool Relative = true;

	// Determines whether the value is random
	bool Random = false;

	// if Random == false
	// if Relative == false: Position in world
	// if Relative ==  true: Relative to current position
	glm::vec3 Value;


	// if Random == true
	// Start range for random
	glm::vec3 RangeStart;
			
	// End range for random
	glm::vec3 RangeEnd;
};

class MoveAction : public Action
{
public:
	MovePosition Start;

	MovePosition End;

	float Speed = 1.f;

	bool Teleport = false;

private:
	glm::vec3 _startPos;
	glm::vec3 _endPos;

	float _progress;
	float _progressSpeed;

	glm::vec3 _delta;

public:
	MoveAction();

	~MoveAction() = default;

	void update(float deltaTime, Timeline* timeline) override;

	void start() override;

private:
	void calcPosition(const MovePosition& position, const glm::vec3& relativeValue, glm::vec3& outPosition);
};

}
