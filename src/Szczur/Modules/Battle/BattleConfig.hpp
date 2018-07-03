#pragma once

#include <vector>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace rat {

class Entity;

class BattleConfig {

	struct PawnData {
		std::string nameID;
		glm::vec2 position;
		PawnData(const std::string& nameID);
	};

public:

// Getters

	///
	glm::vec3 getPosition() const;

	///
	glm::vec2 getSize() const;

	///
	float getScale() const;

	///
	const std::vector<BattleConfig::PawnData>& getPawns() const;

	///
	std::vector<BattleConfig::PawnData>& getPawns();

// Manipulators

	///
	void setPosition(const glm::vec3& value);

	///
	void setSize(const glm::vec2& value);

	///
	void setScale(float value);

	///
	PawnData* addPawn(const std::string& nameID);

	///
	void setupEntity();

private:

	glm::vec3 _scenePosition;
	glm::vec2 _sceneSize{100.f, 100.f};
	float _pawnScale = 1.f;

	std::vector<BattleConfig::PawnData> _pawns;
};

}