#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include <Szczur/Modules/Window/Window.hpp>
#include <Szczur/Modules/Input/Input.hpp>
#include <Szczur/Modules/Script/Script.hpp>

#include "BattlePawn.hpp"

namespace rat {

class BattleScene : public Module<Window, Input, Script> {
public:

// Constructors

	///
	BattleScene();

	///
	BattleScene(const BattleScene&) = delete;

	///
	BattleScene& operator = (const BattleScene&) = delete;

	///
	BattleScene(BattleScene&&) = delete;

	///
	BattleScene& operator = (BattleScene&&) = delete;

	///
	~BattleScene();

// Main

	///
	void update(float deltaTime);

	///
	void render();

	///
	void input(sf::Event& event);

// Pawns manipulations

	/// Add new pawn to battle field
	BattlePawn* addPawn(const std::string& dirPath, const sf::Vector2f& position={0,0});

	/// Set pawn to controll
	void changePawn(BattlePawn* pawn);

	/// Set skill to use (if pawn is set)
	void changeSkill(const std::string& skillName);

	/// Update position for pawn (remove overlaps)
	void fixPosition(BattlePawn& pawn);

// Scripts

	void initScript();

private:
	// Render
	sf::RenderTexture canvas;

	// Battle field
	std::vector<std::unique_ptr<BattlePawn>> pawns;
	sf::Vector2f fieldSize{800, 600};
	sf::Vector2f fieldPos{0, 0};
	BattlePawn* controlledPawn = nullptr;
	BattleSkill* controlledSkill = nullptr;

// Main

	///
	void init();
};

}