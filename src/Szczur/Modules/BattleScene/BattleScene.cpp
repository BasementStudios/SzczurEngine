#include "BattleScene.hpp"

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Logger.hpp"

#include <cmath>

namespace rat {

// ========== Constructors ========== 

BattleScene::BattleScene() {
	LOG_INFO("Initializing BattleScene module");
	initScript();
	init();

	// Test
	auto& window = getModule<Window>().getWindow();
	fieldSize = sf::Vector2f(900, 500);
	fieldPos.x = (window.getSize().x-fieldSize.x)/2.f;
	fieldPos.y = window.getSize().y-fieldSize.y-100;
	BattlePawn* pawn = addPawn("Assets/Battle/karion");
	changePawn(pawn);
	changeSkill("Dash and hit");

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
		addPawn("Assets/Battle/slime", sf::Vector2f(input.getMousePosition()));
	}

	for(auto& obj : pawns) {
		obj->update(deltaTime);
	}

	updateController();
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
		if(obj.get()!=controlledPawn) {
			obj->renderController(canvas, false);
		}
		else {			
			obj->renderController(canvas, true);
		}
	}
	for(auto& obj : pawns) {
		obj->render(canvas);
	}

	// Controller
	renderController();

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

void BattleScene::changePawn(BattlePawn* pawn) {
	controlledPawn = pawn;
	controlledSkill = nullptr;
}

void BattleScene::changeSkill(const std::string& skillName) {
	if(controlledPawn != nullptr) {
		controlledSkill = controlledPawn->getSkill(skillName);
	}
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
// ========== Controller ========== 

void BattleScene::updateController() {

	if(getModule<Input>().getManager().isPressed(Keyboard::Num1)) {
		controlledSkill = controlledPawn->getSkills()[0].get();
	}
	else if(getModule<Input>().getManager().isPressed(Keyboard::Num2)) {
		controlledSkill = controlledPawn->getSkills()[1].get();
	}
	else if(getModule<Input>().getManager().isPressed(Keyboard::Q)) {
		changePawn(pawns[0].get());
		changeSkill("Dash and hit");
	}
	else if(getModule<Input>().getManager().isPressed(Keyboard::E)) {
		changePawn(pawns[1].get());
		changeSkill("Dash and hit");
	}

	BattlePawn* selectedPawn = nullptr;
	auto& input = getModule<Input>().getManager();
	for(auto itr = pawns.rbegin(); itr<pawns.rend(); ++itr) {
		if((*itr)->hitTest(sf::Vector2f(input.getMousePosition()))) {
			selectedPawn = itr->get();
			break;
		}
	}

	if(controlledSkill != nullptr) {
		controlledSkill->updateController(selectedPawn);
	}
}

void BattleScene::renderController() {
	if(controlledPawn != nullptr) {
		sf::RectangleShape shape({80, 80});
		shape.setOutlineColor({255,0,150,150});
		shape.setOutlineThickness(2);
		shape.setFillColor({100,0,50,100});
		auto& skills = controlledPawn->getSkills();
		int i = 0;
		for(auto& obj : skills) {
			if(obj->isUsable()) {
				sf::Sprite icon(obj->getIconSprite());
				icon.setPosition(300+i*90,50);
				shape.setPosition(icon.getPosition());
				if(obj.get() != controlledSkill) {			
					canvas.draw(shape);
					canvas.draw(icon);
				}
				else {
					icon.setColor({255,255,255,255});
					shape.setOutlineColor({255,0,150,255});	
					shape.setFillColor({100,0,50,150});				
					canvas.draw(shape);
					canvas.draw(icon);
					shape.setFillColor({100,0,50,100});				
					shape.setOutlineColor({255,0,150,150});	
					icon.setColor({255,255,255,150});	
				}
				++i;
			}
		}
		// shape.setOutlineColor({255,0,150,255});
	}
	if(controlledSkill != nullptr) {
		controlledSkill->renderController(canvas);
	}
}

// ========== Scripts ========== 

void BattleScene::initScript() {
	auto& script = getModule<Script>();
	auto module = script.newModule("BattleScene");
	script.initClasses<BattlePawn, BattleSkill>(); 

	// TEMPORARY	
	auto moduleUtility = script.newModule("Utility");
	auto classClock = script.newClass<sf::Clock>("Clock", "Utility");
	classClock.set("new", sol::constructors<sf::Clock()>());
	classClock.set("restart", &sf::Clock::restart);
	classClock.set("elapsed", [](sf::Clock& clock){return clock.getElapsedTime().asSeconds();});
	classClock.init();
}

}