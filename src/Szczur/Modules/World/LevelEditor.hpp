
//#ifdef EDITOR
#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "Components/ArmatureComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/ScriptableComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "data/SpriteDisplayData.hpp"
#include "data/ArmatureDisplayData.hpp"

#include <boost/container/flat_map.hpp>
#include "Szczur/Modules/Input/InputManager.hpp"
#include "Szczur/Modules/AudioEditor/AudioEditor.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"
#include "SceneManager.hpp"

namespace rat {
	class DialogEditor;

	struct FreeCamera {
		glm::vec3 position{0.f, 0.f, 0.f};
		glm::vec3 rotation{0.f, 0.f, 0.f};
		bool rotating{false};
		float velocity{50.f};
		sf::Vector2i previousMouse{0, 0};
		void move(const glm::vec3& offset) {position += offset;}
		void rotate(const glm::vec3& offset) {rotation += offset;}
		void processEvents(InputManager& input);
	};

	class LevelEditor {
	public:
		LevelEditor(SceneManager& scenes);

		void render(sf3d::RenderTarget& target);
		void update(InputManager& input, Camera& camera);
	private:
		SceneManager& _scenes;

		void _processEventsForFreeCamera(InputManager& input);

		void _renderBar();
		void _renderDisplayDataManager();
		void _renderArmatureDisplayManager();
		void _renderFocusedObjectsParams();
		void _renderObjectsList();
		void _renderComponentsManager();

		void _menuSave();
		void _printMenuInfo(const std::string& info);
		sf::Clock _menuInfoClock;
		std::string _menuInfo;

		std::string _getRelativePathFromExplorer(const std::string& title, const std::string& directory, const std::string& filter="", bool saveButton = false); 

		bool _ifRenderObjectsList{true};
		bool _ifRenderDisplayDataManager{false};
		bool _ifRenderArmatureDisplayManager{false};
		bool _ifRenderDialogEditor{false};
		bool _ifRenderAudioEditor{false};
		bool _anySelected{false};
		bool _ifRenderComponentsManager{false};
		size_t _focusedObject{static_cast<size_t>(-1)};
		FreeCamera _freeCamera;
		std::string _currentFilePath;
		
		DialogEditor* _dialogEditor = nullptr;
		AudioEditor* _audioEditor = nullptr;
	};

	
}

//#endif
