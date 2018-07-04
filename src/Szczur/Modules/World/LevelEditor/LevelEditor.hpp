
#pragma once
//#ifdef EDITOR

#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include "Szczur/Utility/SFML3D/CircleShape.hpp"
#include "Szczur/Utility/SFML3D/Camera.hpp"
#include "Szczur/Modules/Window/Window.hpp"

#include <tuple>

#include <tuple>

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../Components/ArmatureComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/ScriptableComponent.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Data/SpriteDisplayData.hpp"
#include "../Data/ArmatureDisplayData.hpp"
#include "../ScenesManager.hpp"
#include "ObjectsList.hpp"
#include "SpriteDisplayDataManager.hpp"
#include "ArmatureDisplayDataManager.hpp"

namespace rat {
class DialogEditor;
class AudioEditor;
class InputManager;

class LevelEditor {
public:

// Constructors

	///
	LevelEditor(ScenesManager& scenes);

// Main

	///
	void render(sf3d::RenderTarget& target);

	///
	void update(InputManager& input, Window& camera);

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

	void updateCamera(sf3d::Camera& camera);

private:
	
	///
	void _renderMenuBar();

	///
	void _renderProperties();

	///
	void _renderComponentsManager();

	///
	void _renderSingleProperty();

	///
	void _renderGroupProperty();

    ///
    void _prepareOrigins();

	///
	void _renderOrigins(sf3d::RenderTarget& target);

    ///
    void _renderOriginRectangle(const glm::vec3& position, bool selected, sf3d::RenderTarget& target);
 
    ///
    void _renderOriginCircle(const glm::vec3& position, bool selected, sf3d::RenderTarget& target);
 
	///
	void _renderPlayBar();

	///
	void _render();

	///
	glm::vec2 _getFixedMousePos(const sf::Vector2i& pos);

	///
	void _setupGroup();

	///
	void _updateGroup();

private:

// Select fix
	sf::Vector2i _defaultWindowSize;

// Origins

    sf3d::CircleShape _originCirIn, _originCirOut, _originCirInSel, _originCirOutSel, _groupOriginCir;
    sf3d::RectangleShape _originRectIn, _originRectOut, _originRectInSel, _originRectOutSel;
 

// World

	ScenesManager& _scenes;

// Parts of editor

	ObjectsList _objectsList;
	SpriteDisplayDataManager _spriteDisplayDataManager;
	ArmatureDisplayDataManager _armatureDisplayDataManager;
	DialogEditor* _dialogEditor = nullptr;
	AudioEditor* _audioEditor = nullptr;

// Free camera

	sf3d::Camera _freeCamera;
	bool _cameraRotating {false};
	sf::Vector2i _cameraPreviousMouseOffset {0, 0};

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

// Dragging

	bool _isDragging = false;
	glm::vec2 _dragLastPos;
	Entity* _draggingEntity = nullptr;
	bool _isDepthDragging = true;

	glm::vec3 _groupOrigin;

	// property window
	glm::vec3 _currentGroupPosition;
	glm::vec3 _lastGroupPosition;
	glm::vec3 _currentGroupRotation;
	glm::vec3 _lastGroupRotation;

	// pos, rotation
	std::vector<std::tuple<Entity*, glm::vec3, glm::vec3>> _selectedEntitesBackup;

	Entity* _entityToUnselect = nullptr;
	glm::vec3 _entityToUnselectPos;
};

	
}

//#endif
