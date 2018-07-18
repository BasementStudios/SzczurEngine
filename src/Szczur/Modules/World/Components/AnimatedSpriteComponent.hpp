#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/SpriteDisplayData.hpp"

namespace rat
{

class Entity;
class Script;
template<class T> class ScriptClass;

class AnimatedSpriteComponent : public sf3d::Drawable, public Component
{
public:
	enum SpriteSheet
	{
		LeftToRight,
		UpToDown
	};

public:
	///
	AnimatedSpriteComponent(Entity* parent);

	///
	AnimatedSpriteComponent(const AnimatedSpriteComponent&) = default;

	///
	AnimatedSpriteComponent& operator = (const AnimatedSpriteComponent&) = default;

	///
	AnimatedSpriteComponent(AnimatedSpriteComponent&&) = default;

	///
	AnimatedSpriteComponent& operator = (AnimatedSpriteComponent&&) = default;

	///
	~AnimatedSpriteComponent() = default;

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	///
	void setSpriteDisplayData(SpriteDisplayData* spriteDisplayData);

	///
	void setTexture(const std::string& texturePath);

	///
	SpriteDisplayData* getSpriteDisplayData() const;

	///
	virtual void* getFeature(Component::Feature_e feature) override;

	///
	virtual const void* getFeature(Component::Feature_e feature) const override;

	///
	virtual void loadFromConfig(nlohmann::json& config) override;

	///
	virtual void saveToConfig(nlohmann::json& config) const override;

	///
	virtual void update(ScenesManager& scenes, float deltaTime);

	///
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;

	///
	virtual void setTextureRect(const sf::FloatRect& rect);

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

	///
	static void initScript(ScriptClass<Entity>& entity, Script& script);

	///
	void play();

	///
	void setLoop(bool loop) { _loop = loop; }

	///
	bool isLoop() { return _loop; }

	///
	void setFrameNum(int frames) { _frames = frames; }
	
	///
	int getFrameNum() { return _frames; }

	///
	void setFrameSize(int width, int height) { _frameSize = { width, height }; }

	///
	const glm::vec2& getFrameSize() { return _frameSize; }

	///
	void setRows(int rows) { _rows = rows; }

	///
	int getRows() { return _rows; }

	///
	void setColumns(int columns) { _columns = columns; }

	///
	int getColumns() { return _columns; }

	///
	void setSpeed(float speed) { _speed = speed; }

	///
	float getSpeed() { return _speed; }

private:
	// @vertical: -1 left, 0 center, 1 right
	// @horizontal: -1 top, 0 center, 1 bottom
	void setOrigin(int vertical = 0, int horizontal = 0);

private:
	SpriteDisplayData* _spriteDisplayData = nullptr;
	sf3d::VertexArray _vertices{ 4 };

	bool _isPlaying = true;

	int _currentFrame = 0;
	int _frames = 1;
	bool _autoUpdateFrames = true;

	bool _loop = true;

	glm::vec2 _frameSize;
	bool _autoUpdateFrameSize = true;

	int _rows = 1;
	int _columns = 1;

	sf::Clock _lastUpdate;

	float _speed = 1.f;
};

}
