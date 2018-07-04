#include "LevelEditor.hpp"

#include <iostream>
#include <experimental/filesystem>

#ifdef OS_WINDOWS
#include <shellapi.h>
#endif

#include <imgui.h>
// #include <NodeEditor/NodeEditor.h>

#include "Szczur/Utility/Convert/GLMStreams.hpp"

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include "Szczur/Utility/SFML3D/CircleShape.hpp"
#include "Szczur/Utility/SFML3D/Linear.hpp"
#include "Szczur/Utility/SFML3D/Camera.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"

#include "Szczur/Modules/Window/Window.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include "Szczur/Modules/AudioEditor/AudioEditor.hpp" 
#include "Szczur/Modules/DialogEditor/DialogEditor.hpp"

#include "Szczur/Modules/Input/InputManager.hpp"

#include "Szczur/Modules/Window/Window.hpp"

namespace rat {
	LevelEditor::LevelEditor(ScenesManager& scenes) :
	_scenes(scenes),
	_objectsList{scenes},
	_spriteDisplayDataManager{scenes},
	_armatureDisplayDataManager{scenes} {
		_freeCamera.move({1000.f,500.f,2000.f});
		detail::globalPtr<Window>->getWindow().setRenderDistance(300.f);
		_dialogEditor = detail::globalPtr<DialogEditor>;
		_audioEditor = detail::globalPtr<AudioEditor>;

        _prepareOrigins();

		const auto& videoMode = detail::globalPtr<Window>->getVideoMode();

		_defaultWindowSize = sf::Vector2i(videoMode.width, videoMode.height);
	}

	void LevelEditor::setClipboard(const glm::vec3& value) {
		_vec3Clipboard = value;
	}

	void LevelEditor::setClipboard(const glm::vec2& value) {
		_vec2Clipboard = value;
	}

	glm::vec3 LevelEditor::getClipboardVec3() {
		return _vec3Clipboard;
	}

	glm::vec2 LevelEditor::getClipboardVec2() {
		return _vec2Clipboard;
	}

	ObjectsList& LevelEditor::getObjectsList() {
		return _objectsList;
	}

	void LevelEditor::render(sf3d::RenderTarget& target) {

		// Show ImGui demo window
    	if(_ifShowImGuiDemoWindow) ImGui::ShowDemoWindow();

		auto* scene = _scenes.getCurrentScene();
		if(scene) {

			// Menu bar
			_renderMenuBar();

			// Object properties
			_renderProperties();

			// Tools
			if(_ifRenderObjectsList) _objectsList.render(_ifRenderObjectsList);
			if(_ifRenderSpriteDisplayDataManager) _spriteDisplayDataManager.render(_ifRenderSpriteDisplayDataManager);
			if(_ifRenderArmatureDisplayDataManager) _armatureDisplayDataManager.render(_ifRenderArmatureDisplayDataManager);
			if(_ifRenderDialogEditor) _dialogEditor->update();
			if(_ifRenderAudioEditor) _audioEditor->render();
			

			scene = _scenes.getCurrentScene();
			
			_renderOrigins(target);
		}
	}

	void LevelEditor::update(InputManager& input, Window& windowModule) {
		auto* scene = _scenes.getCurrentScene();
		auto& window = windowModule.getWindow();
		auto mouse = input.getMousePosition();
		Entity* cameraEntity = nullptr;
		
		// Find selected camera
		for (auto& entity : scene->getEntities("single")) { 
			if (auto* comp = entity->getComponentAs<CameraComponent>()) { 
                if (_objectsList.getSelectedID() == entity->getID() || comp->getLock()) { 
                    cameraEntity = entity.get(); 
                } 
            } 
        } 

		// Use camera for rendering
		//	@todo , This should be done when changing camera only...
		if (cameraEntity) {
			window.setCamera(cameraEntity->getComponentAs<CameraComponent>());
		}
		else {
			window.setCamera(_freeCamera);
		}

		// Object selection
		if (!ImGui::IsAnyWindowHovered()) {
			if (input.isPressed(Mouse::Left)) {
				auto mouse = input.getMousePosition();
				
				auto linear = window.getLinearByScreenPos({ mouse.x, mouse.y });

				scene->forEach([&] (const std::string&, Entity& entity) {
					if (linear.contains(entity.getPosition() - glm::vec3{ 50.f, -50.f, 0.f }, { 100.f, 100.f, 0.f })) {
						if (cameraEntity && entity.getID() == cameraEntity->getID())
							return;

						if (input.isKept(rat::Keyboard::LShift)) {
							if (_objectsList.isEntitySelected(&entity)) {
								_entityToUnselect = &entity;
								_entityToUnselectPos = entity.getPosition();
							}
							else {
								_objectsList.addSelected(&entity);

								_setupGroup();
							}
						}
						else {
							_objectsList.select(entity.getID());
						}

						if (_objectsList.isGroupSelected()) {
							_draggingEntity = &entity;
						}

						if (&entity == _draggingEntity) {
							_isDragging = true;

							glm::vec3 projection;

							if (_isDepthDragging)
								projection = linear.getProjectionY(_draggingEntity->getPosition());
							else
								projection = linear.getProjectionZ(_draggingEntity->getPosition());

							_dragLastPos = glm::vec2(projection.x, _isDepthDragging ? projection.z : projection.y);
						}

						_draggingEntity = &entity;
					}
				});
			}

			if (input.isReleased(Mouse::Left)) {
				_isDragging = false;

				//_draggingEntity = nullptr;

				if (_entityToUnselect != nullptr && input.isKept(rat::Keyboard::LShift)) {
					if (_entityToUnselectPos == _entityToUnselect->getPosition()) {
						_objectsList.removedSelected(_entityToUnselect);

						_setupGroup();

						_entityToUnselect = nullptr;
					}
				}
			}

			if (input.isKept(Mouse::Left)) {
				if (_isDragging) {
					auto mouse = _getFixedMousePos(input.getMousePosition());
					auto linear = window.getLinearByScreenPos(mouse);

					glm::vec3 projection;
					glm::vec3 offset;

					if (_isDepthDragging) {
						projection = linear.getProjectionY(_draggingEntity->getPosition());
						offset = glm::vec3(projection.x - _dragLastPos.x, 0.f, projection.z - _dragLastPos.y);
						_dragLastPos = glm::vec2(projection.x, projection.z);

					}
					else {
						projection = linear.getProjectionZ(_draggingEntity->getPosition());
						offset = glm::vec3(projection.x - _dragLastPos.x, projection.y - _dragLastPos.y, 0.f);
						_dragLastPos = glm::vec2(projection.x, projection.y);
					}

					if (_objectsList.isAnySingleEntitySelected()) {
						auto entity = _objectsList.getSelectedEntity();
						entity->move(offset);
					}
					else if (_objectsList.isGroupSelected()) {
						_groupOrigin = glm::vec3();

						auto& group = _objectsList.getSelectedEntities();

						for (auto& entity : group) {
							entity->move(offset);
							_groupOrigin += entity->getPosition();
						}

						_groupOrigin /= group.size();
					}
				}
			}
		}

		if (input.isPressed(Keyboard::Z)) {
			_isDepthDragging = !_isDepthDragging;

			if (_draggingEntity != nullptr) {
				auto mouse = _getFixedMousePos(input.getMousePosition());

				auto linear = window.getLinearByScreenPos(mouse);

				glm::vec3 projection;

				if (_isDepthDragging)
					projection = linear.getProjectionY(_draggingEntity->getPosition());
				else
					projection = linear.getProjectionZ(_draggingEntity->getPosition());

				_dragLastPos = glm::vec2(projection.x, _isDepthDragging ? projection.z : projection.y);

			}
		}
		
		// Camera movement
		if (ImGui::IsAnyItemActive() == false) {
			sf3d::Camera* camera;
			
			// Choose the camera to move
			if (cameraEntity) {
				camera = cameraEntity->getComponentAs<CameraComponent>();
			}
			else {
				camera = &_freeCamera;
			}
			
			// Velocity
			float velocity = 50.f; // @todo , should be const in class
			/**/ if (input.isKept(Keyboard::LShift)) velocity = 200.f;
			else if (input.isKept(Keyboard::LAlt)) velocity = 10.f;

			// Movement
			if (input.isKept(Keyboard::W)) 			camera->bartek({0.f, 0.f,  velocity});
			if (input.isKept(Keyboard::S)) 			camera->bartek({0.f, 0.f, -velocity});
			if (input.isKept(Keyboard::D)) 			camera->bartek({ velocity, 0.f, 0.f});
			if (input.isKept(Keyboard::A)) 			camera->bartek({-velocity, 0.f, 0.f});
			if (input.isKept(Keyboard::Space)) 		camera->bartek({0.f,  velocity, 0.f});
			if (input.isKept(Keyboard::LControl)) 	camera->bartek({0.f, -velocity, 0.f});
			
			// Rotating
			if (_cameraRotating) {
				camera->rotate({
					-(mouse.y - _cameraPreviousMouseOffset.y) / 10.f, 
					-(mouse.x - _cameraPreviousMouseOffset.x) / 10.f, // @todo , should be const in class
					0.f
				});
				_cameraPreviousMouseOffset = mouse;
			}
			if (!ax::NodeEditor::IsActive()) {
				if (input.isPressed(Mouse::Right)) {
					_cameraRotating = true;
					_cameraPreviousMouseOffset = input.getMousePosition();
				}
			}
			if (input.isReleased(Mouse::Right)) {
				_cameraRotating = false;
			}
			
			if (cameraEntity) {
				// Update entity position to position updated by camera
				//	@warn There shouldn't be even any reason to do that, but this needed because of how World works.
				Entity* entity = cameraEntity->getComponentAs<CameraComponent>()->getEntity();
				entity->setPosition(camera->getPosition());
				entity->setRotation(camera->getRotation());
			}
		}

		// Keys
		if (input.isReleased(Keyboard::F1)) {
			if (_scenes.isGameRunning()) {
				printMenuBarInfo("Cannot save while game is running");
			}
			else {
				_scenes.menuSave();
				printMenuBarInfo(std::string("World saved in file: ") + _scenes.currentFilePath);
			}
		}
	}

	glm::vec2 LevelEditor::_getFixedMousePos(const sf::Vector2i& pos) {
		glm::vec2 result;
		
		const auto& size = detail::globalPtr<Window>->getWindow().getSize();

		result.x = (pos.x * _defaultWindowSize.x) / size.x;
		result.y = (pos.y * _defaultWindowSize.y) / size.y;

		return result;
	}

	void LevelEditor::_setupGroup() {
		_currentGroupPosition = glm::vec3();
		_lastGroupPosition = glm::vec3();
		_currentGroupRotation = glm::vec3();
		_lastGroupRotation = glm::vec3();

		_updateGroup();
	}

	void LevelEditor::_updateGroup()
	{
		_groupOrigin = glm::vec3();
		_selectedEntitesBackup.clear();

		auto group = _objectsList.getSelectedEntities();

		for (auto entity : group)
		{
			_groupOrigin += entity->getPosition();
			_selectedEntitesBackup.emplace_back(entity, entity->getPosition(), entity->getRotation());
		}

		_groupOrigin /= group.size();
	}
}
