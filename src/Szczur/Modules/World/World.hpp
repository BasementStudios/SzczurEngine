#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/SFML3D/RenderLayer.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "ScenesManager.hpp"
#include "Szczur/Modules/Battle/Battle.hpp"

#ifdef EDITOR
	#include "LevelEditor/LevelEditor.hpp"
#endif

namespace rat
{

enum class FadeStage
{
	FadingIn = 0,
	FadingOut,
	None
};

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

	void loadFuckingShaderConfig(const std::string& paff);

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

	///
	void fadeIn(float time);
	
	///
	void fadeOut(float time);

private:
	void onFade(FadeStage endedFadeStage);

private:

	bool _doEditor{true};

	ScenesManager _scenes;

	#ifdef EDITOR
	LevelEditor _levelEditor;
	#endif

// Fade
	FadeStage _fadeStage = FadeStage::None;
	float _fadeTime = 1.f;
	sf::Clock _fadeStart;
	sol::function _fadeCallback;

// Changing scene with fade
	bool _isChangingScene = false;
	size_t _sceneToChange = 0;

	sf3d::ShaderProgram _thisWholeCodeWillBeDeletedAndReplacedWithBetterCodeWhichWouldIncludePlanningAndTestingInsteadOfJustWritingShit;
	float _elapsedTime = 0.f;
};

}
