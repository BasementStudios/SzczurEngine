#pragma once

#include <string>

#include <Szczur/Utility/SFML3D/Sprite.hpp>

#include <glm/vec2.hpp>

namespace rat
{

class BattleScene;
class BattleTextureManager;

class Script;

class BattleSprite
{

public:

	BattleSprite(BattleScene* scene);

	void setPosition(glm::vec2 position);

	glm::vec2 getPosition();

	void setOrigin(glm::vec2 origin);

	glm::vec2 getOrigin();

	void setScale(float scale);

	void setHeight(float height);

	void move(glm::vec2 direction);

	void kill();

	bool isAlive();

	sf3d::Sprite& getSprite();

	void setTexture(const std::string& textureName);

	virtual void render(sf3d::RenderTarget& canvas);

	static void initScript(Script& script);

protected:

	BattleTextureManager& _textureManager;

	BattleScene* _scene = nullptr;

	sf3d::Sprite _sprite;

	glm::vec2 _position;
	float _scale = 1.f;

	bool _isLife = true;

};

}