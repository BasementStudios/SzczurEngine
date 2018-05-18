#include "BattleScene.hpp"

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Logger.hpp"

#include <cmath>

namespace rat {

// ========== Constructors ========== 

BattleScene::BattleScene() {
	LOG_INFO("Initializing BattleScene module");
	init();

	// Test
	auto& window = getModule<Window>().getWindow();
	fieldPos.x = (window.getSize().x-fieldSize.x)/2.f;
	fieldPos.y = window.getSize().y-fieldSize.y-100;
	setController(addPawn("Assets/Battle/pawn_1"));

	LOG_INFO("Module BattleScene initialized");
}

BattleScene::~BattleScene() {
	LOG_INFO("Module BattleScene destructed");
}

// ========== Main ========== 

void BattleScene::init() {
	auto& window = getModule<Window>();
	window.pushGLStates();
	canvas.create(window.getWindow().getSize().x, window.getWindow().getSize().y);
	canvas.clear({0,0,0,0});
	window.popGLStates();
}

void BattleScene::update(float deltaTime) {

	// Test
	if(InputManager& input = getModule<Input>().getManager(); input.isPressed(Mouse::Right)) {
		addPawn("Assets/Battle/pawn_1", sf::Vector2f(input.getMousePosition()));
	}

}

void BattleScene::render() {	
	auto& window = getModule<Window>();
	window.pushGLStates();
	canvas.clear();
	
	// Battle field
	sf::RectangleShape battleField(fieldSize);
	battleField.setPosition(fieldPos);
	battleField.setFillColor({0,0,0,0});
	battleField.setOutlineThickness(2);
	battleField.setOutlineColor({255,0,150,255});
	canvas.draw(battleField);

	// Pawns
	for(auto& obj : pawns) {
		obj->render(canvas);
	}

	if(controlledPawn != nullptr) {
		controlledPawn->renderController(canvas);
	}

	canvas.display();
	window.draw(sf::Sprite(canvas.getTexture()));
	window.popGLStates();
}

void BattleScene::input(sf::Event& event) {

}

// ========== Pawns manipulations ========== 

BattlePawn* BattleScene::addPawn(const std::string& dirPath, const sf::Vector2f& position) {
	BattlePawn* pawn =  pawns.emplace_back(new BattlePawn(*this)).get();
	pawn->loadPawn(dirPath);
	pawn->setPosition(position);
	return pawn;
}

void BattleScene::setController(BattlePawn* pawn) {
	controlledPawn = pawn;
}

void BattleScene::fixPosition(BattlePawn& pawn) {
	if(pawn.getPosition().x-pawn.getColliderRadius()<fieldPos.x) {
		pawn.setRawPositionX(fieldPos.x+pawn.getColliderRadius());
	}
	else if(pawn.getPosition().x+pawn.getColliderRadius()>fieldPos.x+fieldSize.x) {
		pawn.setRawPositionX(fieldPos.x+fieldSize.x-pawn.getColliderRadius());
	}
	if(pawn.getPosition().y-pawn.getColliderRadius()<fieldPos.y) {
		pawn.setRawPositionY(fieldPos.y+pawn.getColliderRadius());
	}
	else if(pawn.getPosition().y+pawn.getColliderRadius()>fieldPos.y+fieldSize.y) {
		pawn.setRawPositionY(fieldPos.y+fieldSize.y-pawn.getColliderRadius());
	}

	bool checkAllAgain = false;
	int tries = 10;
	do {
		checkAllAgain = false;
		for(auto& obj : pawns) {
			if(obj.get() == &pawn) { 
				continue;
			}

			// Distance
			const sf::Vector2f& v1 = obj->getPosition();
			const sf::Vector2f& v2 = pawn.getPosition();
			float dx = v1.x-v2.x;
			float dy = v1.y-v2.y; 
			float dis = std::sqrt(dx*dx+dy*dy);

			// Angle
			float ang = std::atan2(v2.y-v1.y, v2.x-v1.x);
			if(tries<0) {
				break;
			}

			// Move
			if(float d = obj->getColliderRadius()+pawn.getColliderRadius()-dis; d>=0) {
				pawn.moveRaw({std::cos(ang)*d, std::sin(ang)*d});
				checkAllAgain = true;
			}
		}
		--tries;
	} 
	while(checkAllAgain);
}

}