
//#ifdef EDITOR
#pragma once

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../Components/ArmatureComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/ScriptableComponent.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Data/SpriteDisplayData.hpp"
#include "../Data/ArmatureDisplayData.hpp"
#include "../ScenesManager.hpp"

#include <boost/container/flat_map.hpp>
#include "Szczur/Modules/Camera/Camera.hpp"

#include "ObjectsList.hpp"
#include "SpriteDisplayDataManager.hpp"
#include "ArmatureDisplayDataManager.hpp"

namespace rat {

class InputManager;

struct FreeCamera {
public:
	
	///
	void move(const glm::vec3& offset) {position += offset;}

	///
	void rotate(const glm::vec3& offset) {rotation += offset;}

	///
	void processEvents(InputManager& input);

public:

	glm::vec3 position{0.f, 0.f, 0.f};
	glm::vec3 rotation{0.f, 0.f, 0.f};
	bool rotating{false};
	float velocity{50.f};
	sf::Vector2i previousMouse{0, 0};
};

class LevelEditor {
public:

// Constructors

	///
	LevelEditor(ScenesManager& scenes);

// Main

	///
	void render(sf3d::RenderTarget& target);

	///
	void update(InputManager& input, Camera& camera);

	///
	void printMenuBarInfo(const std::string& text);

private:

	///
	void _processEventsForFreeCamera(InputManager& input);
	
	///
	void _renderMenuBar();

	///
	void _renderProperties();

	///
	void _renderComponentsManager();

	///
	void _renderOrigins(sf3d::RenderTarget& target);

	///
	void _renderOriginRectangle(const glm::vec3& position, const glm::vec4& color, bool selected, sf3d::RenderTarget& target);

	///
	void _renderOriginCircle(const glm::vec3& position, const glm::vec4& color, bool selected, sf3d::RenderTarget& target);

	///
	void _render();


private:

// World

	ScenesManager& _scenes;

// Parts of editor

	FreeCamera _freeCamera;
	ObjectsList _objectsList;
	SpriteDisplayDataManager _spriteDisplayDataManager;
	ArmatureDisplayDataManager _armatureDisplayDataManager;
	// DialogEditor* _dialogEditor = nullptr;
	// AudioEditor* _audioEditor = nullptr;

// Menu info

	std::string _menuInfo;
	sf::Clock _menuInfoClock;

// Ifs

	bool _ifRenderDialogEditor{false};
	bool _ifRenderAudioEditor{false};
	bool _ifRenderObjectsList{true};
	bool _ifRenderSpriteDisplayDataManager{false};
	bool _ifRenderArmatureDisplayDataManager{false};
	bool _ifRenderProperties{false};
	bool _ifShowImGuiDemoWindow{false};

};

	
}

//#endif
