#include "BattleConfig.hpp"

#include <Szczur/Modules/World/Entity.hpp>

namespace rat {

BattleConfig::PawnData::PawnData(const std::string& nameID)
	: nameID(nameID)
{

}

// ----------------- Getters -----------------

glm::vec3 BattleConfig::getPosition() const
{
	return _scenePosition; 
}

glm::vec2 BattleConfig::getSize() const
{
	return _sceneSize; 
}

float BattleConfig::getScale() const
{
	return _pawnScale; 	
}

const std::vector<BattleConfig::PawnData>& BattleConfig::getPawns() const
{
	return _pawns; 
}

std::vector<BattleConfig::PawnData>& BattleConfig::getPawns()
{
	return _pawns; 
}

// ----------------- Manipulators -----------------

void BattleConfig::setPosition(const glm::vec3& value)
{
	_scenePosition = value;
}

void BattleConfig::setSize(const glm::vec2& value)
{
	_sceneSize = value;
}

void BattleConfig::setScale(float value)
{
	_pawnScale = value;
}

BattleConfig::PawnData* BattleConfig::addPawn(const std::string& nameID)
{
	return &_pawns.emplace_back(nameID);
}


} // namespace rat