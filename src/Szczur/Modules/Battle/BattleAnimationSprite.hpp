#pragma once

#include "BattleSprite.hpp"

#include <SFML/System/Clock.hpp>

namespace rat
{

class BattleAnimationSprite : public BattleSprite 
{

public:

	BattleAnimationSprite(BattleScene* scene);

	void setTimestamp(float value);

	void setFrames(glm::vec2 frameSize, int framesInRow, int framesAmount);

	void setFrame(int index);

	void setFrameSize(glm::vec2 frameSize);

	void setFramesInRow(int framesInRow);

	void setFramesAmount(int framesAmount);

	void stopAtLastFrame(bool flag);

	void restart();

	void removeOnLastFrame(bool flag);

	void render(sf3d::RenderTarget& canvas) override;

	static void initScript(Script& script);

private:

	void _updateSprite();

private:

	int _currentFrame = 0;
	float _lastTime = 0.f;

	sf::Clock _clock;
	float _timestamp = 0.5f;
	int _framesInRow = 1.f;
	int _framesAmount = 1.f;
	glm::ivec2 _frameSize {10, 10};
	bool _stopAtLastFrame = false;
	bool _removeOnLastFrame = false;

};

}