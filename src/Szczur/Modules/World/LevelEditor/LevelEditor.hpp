
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

#include <Szczur/Utility/SFML3D/RectangleShape.hpp>
#include <Szczur/Utility/SFML3D/CircleShape.hpp>

namespace rat {
class DialogEditor;
class AudioEditor;
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

	///
	void setClipboard(const glm::vec3& value);

	///
	void setClipboard(const glm::vec2& value);

	///
	glm::vec3 getClipboardVec3();

	///
	glm::vec2 getClipboardVec2();

	///
	ObjectsList& getObjectsList();

	void updateCurrentCamera();

	void updateCamera(Camera& camera);

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
	void _renderPlayBar();

	///
	void _render();

private:

// World

	sf3d::CircleShape originCircle;
	sf3d::RectangleShape originRect;

// World

	ScenesManager& _scenes;
	Entity* _currentCamera{ nullptr };

// Parts of editor

	FreeCamera _freeCamera;
	ObjectsList _objectsList;
	SpriteDisplayDataManager _spriteDisplayDataManager;
	ArmatureDisplayDataManager _armatureDisplayDataManager;
	DialogEditor* _dialogEditor = nullptr;
	AudioEditor* _audioEditor = nullptr;

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

// Clipboard

	glm::vec3 _vec3Clipboard{0.f,0.f,0.f};
	glm::vec2 _vec2Clipboard{0.f,0.f};
};

	
}

//#endif
