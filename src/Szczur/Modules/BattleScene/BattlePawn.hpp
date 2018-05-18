#pragma once 

#include <SFML/Graphics.hpp>

namespace rat {

class Input;
class BattleScene;

class BattlePawn {
	using RenderTarget = sf::RenderTarget;
public:

// Constructors

	///
	BattlePawn(BattleScene& battleScene);

// Main

	///
	void update(float deltaTime);

	///
	void render(RenderTarget& canvas);

// Getters

	///
	const sf::Vector2f& getPosition() const;

	///
	float getColliderRadius() const;

// Manipulations

	/// Set position with collision callculation
	void setPosition(const sf::Vector2f& position);
	void setPositionX(float x);
	void setPositionY(float y);

	/// Set position without collision callculation
	void setRawPosition(const sf::Vector2f& position);
	void setRawPositionX(float x);
	void setRawPositionY(float y);

	///
	void move(const sf::Vector2f& vector);

	///
	void moveRaw(const sf::Vector2f& vector);

	/// Load pawn from folder
	void loadPawn(const std::string& dirPath);

// Controller

	///
	void renderController(RenderTarget& canvas);

	///
	void updateController();

private:
	// Main
	BattleScene& battleScene;
	sf::Vector2f pos{0,0};
	float colliderRadius = 20.f;

	// Visual
	sf::Texture texture;
	sf::Vector2i frameSize;
	bool isTexture = false;

	// Controller
	Input& input;
};

}