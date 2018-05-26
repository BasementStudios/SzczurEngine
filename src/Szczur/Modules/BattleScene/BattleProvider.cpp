#include "BattleProvider.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

class Script;

// ========== Constructors ==========

BattleProvider::BattleProvider(sf::RenderTarget& canvas) 
	: canvas(canvas) {

}

// ========== Drawing ==========

void BattleProvider::drawCircle(const sf::Vector2f& pos, float radius) {
	sf::CircleShape shape(radius);
	shape.setFillColor({0,0,0,0});
	shape.setOutlineColor({255,0,150,150});
	shape.setOutlineThickness(-2);
	shape.setOrigin(radius, radius);
	shape.setPosition(pos);
	canvas.draw(shape);
}

void BattleProvider::drawCircle(float x, float y, float radius) {
	sf::CircleShape shape(radius);
	shape.setFillColor({0,0,0,0});
	shape.setOutlineColor({255,0,150,150});
	shape.setOutlineThickness(-2);
	shape.setOrigin(radius, radius);
	shape.setPosition(x, y);
	canvas.draw(shape);
}

// ========== Script ==========

void BattleProvider::initScript(Script& script) {
	auto object = script.newClass<BattleProvider>("BattleProvider", "BattleScene");

	object.setOverload("drawCircle", 
		sol::resolve<void(const sf::Vector2f&,float)>(&BattleProvider::drawCircle),
		sol::resolve<void(float,float,float)>(&BattleProvider::drawCircle)
	);

	object.init();
}

}