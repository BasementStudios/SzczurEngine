#include "BattlePawn.hpp"

#include <json.hpp>
using Json = nlohmann::json;

#include <Szczur/Modules/Input/Input.hpp>

#include "BattleScene.hpp"

namespace rat {

// ========== Constructors ========== 

BattlePawn::BattlePawn(BattleScene& battleScene) 
	: battleScene(battleScene), input(*detail::globalPtr<Input>) {
}
 
// ========== Main ========== 

void BattlePawn::update(float deltaTime) {

}


void BattlePawn::render(BattlePawn::RenderTarget& canvas) {

	//Visual
	if(isTexture) {
		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
		sprite.setOrigin(frameSize.x/2.f, frameSize.y);
		sprite.setPosition(pos);
		canvas.draw(sprite);
	}

	sf::CircleShape collider(colliderRadius);
	collider.setOrigin(colliderRadius, colliderRadius);
	collider.setPosition(pos);
	collider.setFillColor({150,0,255,100});
	collider.setOutlineColor({150,0,255,200});
	collider.setOutlineThickness(-2);
	canvas.draw(collider);
}

// ========== Getters ========== 

const sf::Vector2f& BattlePawn::getPosition() const {
	return pos;
}
float BattlePawn::getColliderRadius() const {
	return colliderRadius;
}

// ========== Manipulations ========== 

void BattlePawn::setPosition(const sf::Vector2f& position) {
	pos = position;
	battleScene.fixPosition(*this);
}
void BattlePawn::setPositionX(float x) {
	pos.x = x;
	battleScene.fixPosition(*this);
}
void BattlePawn::setPositionY(float y) {
	pos.y = y;
	battleScene.fixPosition(*this);
}

void BattlePawn::setRawPosition(const sf::Vector2f& position) {
	pos = position;
}
void BattlePawn::setRawPositionX(float x) {
	pos.x = x;
}
void BattlePawn::setRawPositionY(float y) {
	pos.y = y;
}

void BattlePawn::move(const sf::Vector2f& vector) {
	pos += vector;
	battleScene.fixPosition(*this);
}

void BattlePawn::moveRaw(const sf::Vector2f& vector) {
	pos += vector;
}

void BattlePawn::loadPawn(const std::string& dirPath) {
	Json config;
	std::ifstream(dirPath+"/config.json") >> config;
	frameSize.x = config["frameSize"][0];
	frameSize.y = config["frameSize"][1];
	isTexture = texture.loadFromFile(dirPath+"/"+std::string(config["texture"]));
}

// ========== Controller ========== 

void BattlePawn::renderController(RenderTarget& canvas) {

	sf::CircleShape cursor(colliderRadius);
	cursor.setOrigin(colliderRadius, colliderRadius);
	cursor.setFillColor({0,0,0,0});
	cursor.setOutlineThickness(-2);
	cursor.setOutlineColor({255,0,150,200});
	cursor.setPosition(sf::Vector2f(input.getManager().getMousePosition()));
	canvas.draw(cursor);
}

void BattlePawn::updateController() {

}

}