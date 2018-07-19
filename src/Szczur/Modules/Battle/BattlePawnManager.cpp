#include "BattlePawnManager.hpp"

#include <fstream>

#include "BattlePawn.hpp"
#include "BattleScene.hpp"

#include <nlohmann/json.hpp>

namespace rat {

BattlePawnManager::PawnData::PawnData(const std::string& nameID)
{
	this->nameID = nameID;
}	

void BattlePawnManager::loadAllPawns(const std::string& configPath)
{
	std::ifstream file(configPath);

	if(file.good()) {
		_configPath = configPath;

		nlohmann::json config;
		file>>config;

		if(auto& pawns = config["pawns"]; !pawns.is_null()) {
			_pawns.clear();

			for(auto itrPawn = pawns.begin(); itrPawn != pawns.end(); ++itrPawn) {
				auto& pawn = itrPawn.value();

				PawnData* newPawn = addPawn(itrPawn.key());
				try {
					if(newPawn != nullptr) {
						newPawn->armaturePath = pawn["armature"];
						newPawn->scale = pawn["scale"];
						newPawn->radius = pawn["radius"];
						newPawn->origin.x = pawn["origin"][0].get<float>();
						newPawn->origin.y = pawn["origin"][1].get<float>();

						if(auto& var = pawn["script"]; !var.is_null()) 
							newPawn->scriptPath = std::string("Assets/Pawns/") + newPawn->nameID + "/" + var.get<std::string>();

						if(auto& var = pawn["health"]; !var.is_null()) {
							newPawn->health = var[0];
							newPawn->maxHealth = var[1];
						}

						if(auto& var = pawn["time"]; !var.is_null()) {
							newPawn->time = var[0];
							newPawn->maxTime = var[1];

						if(auto& var = pawn["height"]; !var.is_null()) 
							newPawn->height = var;
						}

						if(auto& var = pawn["damageAnimation"]; !var.is_null()) {
							newPawn->damageAnimationName = var[0];
							newPawn->damageAnimationFade = var[1];
							newPawn->damageAnimationWait = var[2];
							newPawn->damageAnimationSpeed = var[3];
							newPawn->isDamageAnimation = true;
						}
					}
				}
				catch(nlohmann::json::exception e) {
					LOG_EXCEPTION(e);
					_pawns.pop_back();
				}
			}
		}
	}
	else {
		LOG_ERROR("Cannot load pawns: ", configPath);
	}
}

void BattlePawnManager::reloadAllPawns()
{
	loadAllPawns(_configPath);
}

BattlePawnManager::PawnData* BattlePawnManager::addPawn(const std::string& nameID)
{
	for(auto& pawn : _pawns) {
		if(pawn->nameID == nameID) {
			return nullptr;
		}
	}
	return _pawns.emplace_back(new PawnData(nameID)).get();
}

std::vector<std::unique_ptr<BattlePawnManager::PawnData>>& BattlePawnManager::getDataPawns()
{
	return _pawns;
}

std::unique_ptr<BattlePawn> BattlePawnManager::createPawn(const std::string& nameID, BattleScene* scene)
{
	for(auto& pawn : _pawns) {
		if(pawn->nameID == nameID) {
			std::unique_ptr<BattlePawn> newPawn(new BattlePawn(scene));
			newPawn->setNameID(pawn->nameID);
			newPawn->setArmature(pawn->armaturePath);
			newPawn->setScale(pawn->scale);
			newPawn->setRadius(pawn->radius);
			newPawn->setOrigin(pawn->origin);
			newPawn->setHealth(pawn->health);
			newPawn->setMaxHealth(pawn->maxHealth);
			newPawn->setTime(pawn->time);
			newPawn->setMaxTime(pawn->maxTime);
			newPawn->setHeight(pawn->height);
			if(pawn->isDamageAnimation) {
				newPawn->setDamageAnimation(
					pawn->damageAnimationName,
					pawn->damageAnimationFade,
					pawn->damageAnimationWait,
					pawn->damageAnimationSpeed
				);
			}
			newPawn->loadScript(pawn->scriptPath);
			return std::move(newPawn);
		}
	}

	return std::unique_ptr<BattlePawn>(nullptr);
}

} // namespace rat