#pragma once

#include "ScenesManager.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#ifdef EDITOR
	#include "LevelEditor/LevelEditor.hpp"
#endif

namespace rat
{

class World : public Module<Window, Input, Camera, Script>
{
public:

	///
	World();

	///
	World(const World&) = delete;

	///
	World& operator = (const World&) = delete;

	///
	World(World&&) = delete;

	///
	World& operator = (World&&) = delete;

	///
	~World();

	///
	void update(float deltaTime);

	///
	void render();

	#ifdef EDITOR
	LevelEditor& getLevelEditor() {return _levelEditor;}
	#endif

	// void initScript();

	///
	ScenesManager& getScenes();

	///
	const ScenesManager& getScenes() const;

	///
	void initScript();

	///	
	bool isEditor();

	///
	void setEditor(bool flag);

private:

	bool _doEditor{true};

	ScenesManager _scenes;

	#ifdef EDITOR
	LevelEditor _levelEditor;
	#endif
};

}
