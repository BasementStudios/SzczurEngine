#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/vec2.hpp>

namespace rat {

class BattlePawn;
class BattleScene;

class BattlePawnManager {

	struct PawnData {
		std::string nameID;
		std::string armaturePath;
		std::string scriptPath;
		float scale = 1.f;
		float radius = 100.f;
		glm::vec2 origin;
		float health = 100.f;
		float maxHealth = 100.f;
		float time = 0.f;
		float maxTime = 100.f;
		float height = 0.f;

		PawnData(const std::string& nameID);
	};

public:

	/// Load all pawns from one .json file
	void loadAllPawns(const std::string& configPath);

	/// 
	void reloadAllPawns();

	///
	PawnData* addPawn(const std::string& nameID);

	///
	std::vector<std::unique_ptr<PawnData>>& getDataPawns();

	///
	std::unique_ptr<BattlePawn> createPawn(const std::string& nameID, BattleScene* scene);

private:

	std::string _configPath;

	std::vector<std::unique_ptr<PawnData>> _pawns;

};

} // namespace rat