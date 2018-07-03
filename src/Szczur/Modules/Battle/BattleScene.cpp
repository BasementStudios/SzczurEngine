#include "BattleScene.hpp"

#include "Battle.hpp"
#include "BattlePawn.hpp"
#include "BattleConfig.hpp"

#include <Szczur/Modules/World/Scene.hpp>

namespace rat {

// ----------------- Constructors -----------------

BattleScene::BattleScene(Scene* scene) 
	: _scene(scene)
{
	_battleModule = detail::globalPtr<Battle>;
}

BattleScene::~BattleScene() 
{
	;
}

// ----------------- Getters -----------------

float BattleScene::getScale()
{
	return _scale;
}

Scene* BattleScene::getScene()
{
	return _scene;
}

glm::vec3 BattleScene::getPosition()
{
	return _leftCorner;
}

glm::vec3 BattleScene::getRealPosition()
{
	return _position;
}

glm::vec2 BattleScene::getSize()
{
	return _size;
}

// ----------------- Config -----------------

void BattleScene::setupConfig(const BattleConfig& config) 
{
	setSize(config.getSize());
	setPosition(config.getPosition());
	setScale(config.getScale());

	_pawns.clear();
	for(const auto& obj : config.getPawns()) {
		auto* newPawn = addPawn(obj.nameID);
		if(newPawn) {
			newPawn->setPosition(obj.position);
		}
		else {
			LOG_ERROR("newPawn == nullptr");
		}
	}

	_updateAreaShape();
}

void BattleScene::setupConfig(BattleConfig* config) 
{
	setupConfig(*config);
	_lastConfig = config;
}

BattleConfig BattleScene::createConfig()
{
	BattleConfig config;

	config.setSize(_size);
	config.setPosition(_position);
	config.setScale(_scale);

	for(auto& pawn : _pawns) {
		config.addPawn(pawn->getNameID())->position = pawn->getPosition();
	}

	return std::move(config);
}

void BattleScene::updateConfig()
{
	if(_lastConfig) {
		*_lastConfig = createConfig();
	}
}

// ----------------- Manipulations -----------------

void BattleScene::setSize(glm::vec2 size)
{
	_size = size;
	_updateAreaShape();
	_updateLeftCorner();
}

void BattleScene::setPosition(glm::vec3 position)
{
	_position = position;
	_updateAreaShape();
	_updateLeftCorner();
}

void BattleScene::setScale(float scale)
{
	_scale = scale;
	for(auto& pawn : _pawns) {
		pawn->setScale(pawn->getScale());
	}
}

BattlePawn* BattleScene::addPawn(const std::string& nameID)
{
	auto newPawn = _battleModule->getPawnManager().createPawn(nameID, this);
	
	if(!newPawn) 
		return nullptr;
	
	return _pawns.emplace_back(std::move(newPawn)).get();
}

void BattleScene::reloadAllPawns() 
{	
	std::vector<std::unique_ptr<BattlePawn>> oldPawns(std::move(_pawns)); 

	for(auto& oldPawn : oldPawns) {
		auto* newPawn = addPawn(oldPawn->getNameID());
		if(newPawn != nullptr) {
			newPawn->setPosition(oldPawn->getPosition());
		}
	}
}

// ----------------- Main -----------------

void BattleScene::update(float deltaTime)
{
	for(auto& obj : _pawns) {
		obj->update(deltaTime);
	}
}

void BattleScene::render(sf3d::RenderTarget& target)
{
	for(auto& obj : _pawns) {
		obj->render(target);
	}
 
    target.draw(_areaShape);
}

void BattleScene::updateEditor()
{
	// Change area size
	glm::vec2 areaSize = getSize();
	if(ImGui::DragFloat2("Area size", reinterpret_cast<float*>(&areaSize))) {
		setSize(areaSize);
	}

	// Change main scale for pawns
	float scale = getScale();
	if(ImGui::DragFloat("Pawns scale", &scale)) {
		setScale(scale);
	}

	// Add pawn button
	ImGui::Separator();
	if(ImGui::Button("Add pawn...")) {
		ImGui::OpenPopup("Available pawns##battle_pawn_manager");
	}

	if(ImGui::BeginPopupModal("Available pawns##battle_pawn_manager")) {
		for(auto& obj : _battleModule->getPawnManager().getDataPawns()) {
			if(ImGui::Selectable(obj->nameID.c_str())) {				
				addPawn(obj->nameID)->setPosition(glm::vec2(0.f, 0.f));
			}
		}
		ImGui::EndPopup();
	}

	int i = 0;
	for(auto itr = _pawns.begin(); itr != _pawns.end();) {

		bool removeSignal = false;

		ImGui::PushID(i);
		(*itr)->updateEditor(removeSignal);
		ImGui::PopID();

		if(removeSignal) {
			_pawns.erase(itr);
		}
		else {
			++itr;
			++i;
		}
	}
}

// ----------------- Render -----------------

void BattleScene::_updateAreaShape()
{
	_areaShape.setRotation({ 90.f, 0.f, 0.f });
   	_areaShape.setColor({ 0.3f, 0.60f, 0.80f, 0.3f }); 
   	_areaShape.setSize({_size.x, _size.y}); 
   	_areaShape.setOrigin({ _size.x / 2.f, _size.y / 2.f, 0.f });
   	_areaShape.setPosition(_position); 
}

void BattleScene::_updateLeftCorner()
{
	_leftCorner.x = _position.x-_size.x/2.f;
	_leftCorner.y = _position.y;
	_leftCorner.z = _position.z-_size.y/2.f;

	for(auto& obj : _pawns) {
		obj->setPosition(obj->getPosition());
	}
}

} // namespace rat;