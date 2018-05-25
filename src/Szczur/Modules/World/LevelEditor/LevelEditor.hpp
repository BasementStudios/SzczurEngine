
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
#include "Szczur/Modules/AudioEditor/AudioEditor.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"

#include "ObjectsList.hpp"
#include "Bar.hpp"
#include "ObjectParameters.hpp"
#include "SpriteDisplayDataManager.hpp"
#include "ArmatureDisplayDataManager.hpp"

namespace rat {
	class DialogEditor;
	class InputManager;

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
		LevelEditor(ScenesManager& scenes);

		void render(sf3d::RenderTarget& target);
		void update(InputManager& input, Camera& camera);
	private:
		ScenesManager& _scenes;

		void _processEventsForFreeCamera(InputManager& input);

		FreeCamera _freeCamera;
		
		DialogEditor* _dialogEditor = nullptr;
		AudioEditor* _audioEditor = nullptr;
		bool _ifRenderDialogEditor{false};
		bool _ifRenderAudioEditor{false};



		ObjectsList _objectsList;
		Bar _bar;
		ObjectParameters _objectParameters;
		SpriteDisplayDataManager _spriteDisplayDataManager;
		ArmatureDisplayDataManager _armatureDisplayDataManager;

	};

	
}

//#endif
