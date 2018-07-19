#pragma once

#include <Szczur/Modules/World/Entity.hpp>

#include <Szczur/Utility/SFML3D/Texture.hpp>

namespace rat
{

class BattleScene;
class Script;

class BattleSpellIndicatorManager
{

public:

	BattleSpellIndicatorManager();

	void setRenderTarget(sf3d::RenderTarget& canvas);

	void setHeight(float height);

	void setBattleScene(BattleScene* scene);

	void renderCircleIndicator(glm::vec2 pos, float radius, float deltaY);

	void renderLineIndicator(glm::vec2 pos, float width, float height, float angle, float deltaY);

	void renderLineIndicator2(glm::vec2 pos, float width, float height, float angle, float deltaY);

	void renderHpBarIndicator(glm::vec2 pos, float height, float hp, float maxHp);

	void renderHpBarIndicator(glm::vec2 pos, float height, float hp, float maxHp, int sizeIndex);

	void renderTimeBarIndicator(glm::vec2 pos, float height, float time, float maxTime);

	void renderTimeBarIndicator(glm::vec2 pos, float height, float time, float maxTime, int sizeIndex);

	static void initScript(Script& script);

private:

	BattleScene* _scene = nullptr;

	sf3d::RenderTarget _canvas;
	float _height = 20;
	glm::vec2 _leftCorner;

	// Circle indicator
	sf3d::Texture _circleTexture;
	sf3d::Sprite _circleSprite;
	float _circleFactor;

	// Line indicator
	sf3d::Texture _lineTexture;
	sf3d::Texture _lineTexture2;
	sf3d::Sprite _lineSprite;
	sf3d::Sprite _lineSprite2;
	float _lineFactorX;
	float _lineFactorY;

	// Hp bar indicator
	sf3d::Texture _hpBarTexture[3];
	sf3d::Texture _hpBarBackTexture[3];
	sf3d::Sprite _hpBarSprite;
	float _hpBarFactor;

	// Time bar indicator
	sf3d::Texture _timeBarTexture[3];
	sf3d::Texture _timeBarBackTexture[3];
	sf3d::Texture _timeBarFullTexture[3];
	sf3d::Sprite _timeBarSprite;
	float _timeBarFactor;
};

}