#include "BattlePawn.hpp"

#include "BattleScene.hpp"

#include <Szczur/Modules/World/Scene.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

// ----------------- Constructors -----------------

BattlePawn::BattlePawn(BattleScene* scene) 
	: _entity( scene->getScene(), "pawn")
{
	_scriptModule = detail::globalPtr<Script>;
	_armature = static_cast<ArmatureComponent*>(_entity.addComponent<ArmatureComponent>());
	_scene = scene;

	// Radius shape
	_radiusShape.setColor(glm::vec4(0.7f, 0.5f, 0.1f, 0.3f));
	_radiusShape.setRotation(glm::vec3(90.f, 0.f, 0.f));
}

// ----------------- Getters : General -----------------

///
BattleScene* BattlePawn::getScene()
{
	return _scene;
}

///
const std::string& BattlePawn::getNameID()
{
	return _nameID;
}

///
float BattlePawn::getScale()
{
	return _scale;
}

///
const std::string& BattlePawn::getArmaturePath()
{
	return _armature->getName();
}

///
glm::vec2 BattlePawn::getOrigin()
{
	const auto& origin = _entity.getOrigin();
	return glm::vec2(origin.x, origin.y);
}

///
const std::string& BattlePawn::getScriptPath()
{
	return _scriptPath;
}

// ----------------- Getters : Battle -----------------

///
glm::vec2 BattlePawn::getPosition()
{
	return _position;
}

///
float BattlePawn::getRadius()
{
	return _radius;
}

ArmatureComponent* BattlePawn::getArmature()
{
	return _armature;
}

// ----------------- Manipulations : General -----------------

void BattlePawn::setNameID(const std::string& name)
{
	_nameID = name;
}

void BattlePawn::setScale(float scale)
{
	_scale = scale;
	float value = _scale*_scene->getScale();
	_entity.setScale(glm::vec3(value, value, value));
}

void BattlePawn::setArmature(const std::string& armDir) 
{
	_entity.getComponentAs<ArmatureComponent>()->loadArmature(armDir);
}

void BattlePawn::setOrigin(const glm::vec2& origin)
{
	_entity.setOrigin(glm::vec3(origin.x, origin.y, 0.f));
}

// ----------------- Manipulations : Battle -----------------

void BattlePawn::setRadius(float radius)
{
	if(radius<0.f) radius = 0.f;
	_radius = radius;
	_radiusShape.setRadius(_radius);
	_radiusShape.setOrigin(glm::vec3(_radius, _radius, 5.f));
}

void BattlePawn::setPosition(glm::vec2 position) 
{
	_position = position;
	_moved = true;
	_movedRender = true;
}

void BattlePawn::move(glm::vec2 delta)
{
	_position.x += delta.x;
	_position.y += delta.y;
	_moved = true;
	_movedRender = true;
}

void BattlePawn::loadScript(const std::string& scriptPath)
{
	_scriptPath = scriptPath;

	_scriptModule->get()["THIS"] = this;
	_scriptModule->scriptFile(_scriptPath);
	_scriptModule->get()["THIS"] = sol::nil;
}

bool BattlePawn::popMovedFlag()
{
	bool flag = _moved;
	_moved = false;
	return flag;
}

// ----------------- Main -----------------

void BattlePawn::update(float deltaTime)
{
	_entity.update(*_entity.getScene()->getScenes(), deltaTime);
}

void BattlePawn::render(sf3d::RenderTarget& target)
{
	if(_movedRender) {
		glm::vec3 pos(getScene()->getPosition());
		pos.x += _position.x;
		pos.z += _position.y;
		_entity.setPosition(pos);
		_radiusShape.setPosition(pos);
		_movedRender = false;
	}

	_entity.render(target);
	target.draw(_radiusShape);
}

void BattlePawn::updateEditor(bool& removeSignal)
{
	ImGui::Text(_nameID.c_str());

	// Remove pawn button
	ImGui::SameLine();
	if(ImGui::Button(" - ")) {
		// remove pawn
		removeSignal = true;
	}

	// Edit pawn button
	ImGui::SameLine();
	if(ImGui::Button("...")) {
		// edit pawn
	}

	// Change position drag float
	glm::vec2 pos = _position;
	if(ImGui::DragFloat2("Position", reinterpret_cast<float*>(&pos))) {
		setPosition(pos);
	}

	// // Change position drag float
	// float radius = _radius;
	// if(ImGui::DragFloat("Radius", &radius)) {
	// 	setRadius(radius);
	// }
}

} // namespace rat