#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/SFML3D/RenderLayer.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "ScenesManager.hpp"

#ifdef EDITOR
	#include "LevelEditor/LevelEditor.hpp"
#endif

namespace rat
{

class World : public Module<Window, Input, Script>
{
public:

	///
	World();

	///
	~World();

	/// Non-copyable
	World(const World&) = delete;
	World& operator = (const World&) = delete;

	/// Non-movable
	World(World&&) = delete;
	World& operator = (World&&) = delete;

	///
	void init();
	
	///
	void update(float deltaTime);

	///
	void render();

	///
	void processEvent(sf::Event event);

	#ifdef EDITOR
	LevelEditor& getLevelEditor() {return _levelEditor;}
	#endif

	// void initScript();

	///
	const ScenesManager& getScenes() const;

	///
	ScenesManager& getScenes();

	///
	void initScript();

	///	
	bool isEditor();

	///
	void setEditor(bool flag);

	///
	void fadeIntoScene(size_t id, float fadeTime = 1.f);

private:

	bool _doEditor{true};

	ScenesManager _scenes;

	#ifdef EDITOR
	LevelEditor _levelEditor;
	#endif

// Fade into scene
	sf::RectangleShape _blackScreen; // @todo: change to postprocess effect when will be avaliable
	int _fadeStage = 0;
	bool _isChangingScene = false;
	size_t _sceneToChange = 0;
	float _fadeTime = 1.f;
	sf::Clock _fadeStart;
};

}
