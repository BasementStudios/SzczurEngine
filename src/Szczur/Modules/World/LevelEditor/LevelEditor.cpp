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
		_freeCamera.move({1000.f,500.f,2000.f}); // @todo . On ratio change there should be update to the camera
		_freeCamera.setRenderDistance(300.f);
		_dialogEditor = detail::globalPtr<DialogEditor>;
		_audioEditor = detail::globalPtr<AudioEditor>;

		_prepareOrigins();

		const auto& videoMode = detail::globalPtr<Window>->getVideoMode();

		_defaultWindowSize = sf::Vector2i(videoMode.width, videoMode.height);

		_selectionRect.setFillColor(sf::Color(30, 136, 229, 100));
		_selectionRect.setOutlineColor(sf::Color::Black);

		loadConfig(LevelEdtiorConfigPath);
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
			if(_ifRenderReloader) _renderReloader();
			

			scene = _scenes.getCurrentScene();
			
			_renderOrigins(target);
		}

		if (_isGroupSelecting) {
			auto& window = detail::globalPtr<Window>->getWindow();
				
			window.pushGLStates();
			window.draw(_selectionRect);
			window.popGLStates();
		}
	}

	void LevelEditor::updateDisabledEditor(InputManager& input)
	{		
		auto* scene = _scenes.getCurrentScene();

		// Find selected camera
		Entity* cameraEntity = nullptr;
		for (auto& entity : scene->getEntities("single")) { 
			if (auto* comp = entity->getComponentAs<CameraComponent>()) { 
				if (_objectsList.getSelectedID() == entity->getID() || comp->getLock()) { 
					cameraEntity = entity.get(); 
				} 
			} 
		}

		// Update movement
		updateCameraMovement(cameraEntity, input);
	}

	void LevelEditor::update(InputManager& input, Window& windowModule) {
		auto* scene = _scenes.getCurrentScene();
		auto& window = windowModule.getWindow();
		auto mouse = _getFixedMousePos(input.getMousePosition());
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
				auto mouse = _getFixedMousePos(input.getMousePosition());
				
				auto linear = window.getLinearByScreenPosition({ mouse.x, mouse.y });

				bool selected = false;

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

						selected = true;
					}
				});

				// if none object was selected
				if (!selected) {
					// enable group seleciton
					_isGroupSelecting = true;

					// set start pos of selection
					_selectionStartPos = mouse;

					// set pos for rect
					_selectionRect.setPosition({ mouse.x, mouse.y });
					_selectionRect.setSize({ 0.f, 0.f });
				}
			}

			if (input.isReleased(Mouse::Left)) {
				_isDragging = false;

				// if is group selection
				if (_isGroupSelecting) {
					auto mouse = _getFixedMousePos(input.getMousePosition());

					// calc offset
					auto offset = mouse - _selectionStartPos;

					glm::vec2 start = _selectionStartPos;
					glm::vec2 end = mouse;

					if (offset.x < 0.f)
						std::swap(start.x, end.x);
					
					if (offset.y < 0.f)
						std::swap(start.y, end.y);

					// calc linears
					auto linearStart = window.getLinearByScreenPosition({ start.x, start.y });
					auto linearEnd = window.getLinearByScreenPosition({ end.x, end.y });

					// disable dragging
					_isDragging = false;

					scene->forEach([&] (const std::string&, Entity& entity) {
						// get size of rect in 3d
						auto size = linearEnd.getProjectionZ(entity.getPosition()) - linearStart.getProjectionZ(entity.getPosition());

						// check if object is in rect
						if (linearEnd.contains(entity.getPosition() - glm::vec3{ 50.f, -50.f, 0.f }, { std::abs(size.x), std::abs(size.y), 0.f })) {
							// add to list
							_objectsList.addSelected(&entity);
							_draggingEntity = &entity;
						}
					});

					// setup group
					_setupGroup();

					// disable group selection
					_isGroupSelecting = false;
				}

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
				if (_isDragging && _dragAndDropObjects) {
					auto mouse = _getFixedMousePos(input.getMousePosition());
					auto linear = window.getLinearByScreenPosition(mouse);

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

				// update group selection's size
				if (_isGroupSelecting) {
					auto size = _getFixedMousePos(input.getMousePosition()) - _selectionStartPos;

					_selectionRect.setSize({ size.x, size.y });
				}
			}

			if (input.isPressed(Keyboard::Z)) {
				_isDepthDragging = !_isDepthDragging;

				if (_draggingEntity != nullptr) {
					auto mouse = _getFixedMousePos(input.getMousePosition());

					auto linear = window.getLinearByScreenPosition(mouse);

					glm::vec3 projection;

					if (_isDepthDragging)
						projection = linear.getProjectionY(_draggingEntity->getPosition());
					else
						projection = linear.getProjectionZ(_draggingEntity->getPosition());

					_dragLastPos = glm::vec2(projection.x, _isDepthDragging ? projection.z : projection.y);

				}
			}
		}
		
		// Camera movement
		if (ImGui::IsAnyItemActive() == false) {
			updateCameraMovement(cameraEntity, input);
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

		if (input.isPressed(Keyboard::F4)) {
			changeCameraLock();
		}
	}

	void LevelEditor::updateCameraMovement(Entity* cameraEntity, InputManager& input)
	{
		auto mouse = input.getMousePosition();

		// Camera movement
		sf3d::Camera* camera;
		
		// Choose the camera to move
		if (cameraEntity) {
			auto* comp = cameraEntity->getComponentAs<CameraComponent>();
			if(comp->isNoMove()) {
				return;
			}
			camera = static_cast<sf3d::Camera*>(comp);
		}
		else {
			camera = &_freeCamera;
		}
		
		// Velocity
		float velocity = 30.f; // @todo , should be const in class
		/**/ if (input.isKept(Keyboard::LShift)) velocity = 150.f;
		else if (input.isKept(Keyboard::LAlt)) velocity = 10.f;

		// Natural movement
		// if (input.isKept(Keyboard::W)) 			camera->bartek({0.f, 0.f,  velocity});
		// if (input.isKept(Keyboard::S)) 			camera->bartek({0.f, 0.f, -velocity});
		// if (input.isKept(Keyboard::D)) 			camera->bartek({ velocity, 0.f, 0.f});
		// if (input.isKept(Keyboard::A)) 			camera->bartek({-velocity, 0.f, 0.f});
		// if (input.isKept(Keyboard::Space)) 		camera->bartek({0.f,  velocity, 0.f});
		// if (input.isKept(Keyboard::LControl)) 	camera->bartek({0.f, -velocity, 0.f});

		// "Minecraft" movement
		auto rotation = camera->getRotation();

		if (_isMCCameraMovement) {
			if (input.isKept(Keyboard::W))
			{
				camera->move({
					-velocity * glm::sin(glm::radians(rotation.y)),
					0.f,
					-velocity * glm::cos(glm::radians(rotation.y))
				});
			}

			if (input.isKept(Keyboard::S))
			{
				camera->move({
					velocity * glm::sin(glm::radians(rotation.y)),
					0.f,
					velocity * glm::cos(glm::radians(rotation.y))
				});
			}

			if (input.isKept(Keyboard::D))
			{
				camera->move(glm::vec3{
					velocity * glm::cos(glm::radians(rotation.y)),
					0.f,
					-velocity * glm::sin(glm::radians(rotation.y))
				});
			}

			if (input.isKept(Keyboard::A))
			{
				camera->move(glm::vec3{
					-velocity * glm::cos(glm::radians(rotation.y)),
					0.f,
					velocity * glm::sin(glm::radians(rotation.y))
				});
			}
		}
		else {
			if (input.isKept(Keyboard::W)) camera->move({ 0.f, 0.f, -velocity });
			if (input.isKept(Keyboard::S)) camera->move({ 0.f, 0.f,  velocity });
			if (input.isKept(Keyboard::D)) camera->move({ velocity, 0.f, 0.f });
			if (input.isKept(Keyboard::A)) camera->move({ -velocity, 0.f, 0.f });
		}

		if (input.isKept(Keyboard::Space)) 		camera->move({0.f,  velocity, 0.f});
		if (input.isKept(Keyboard::LControl)) 	camera->move({0.f, -velocity, 0.f});
		
		// Rotating
		if (_cameraRotating) {
			camera->rotate({
				-(mouse.y - _cameraPreviousMouseOffset.y) / 10.f, 
				-(mouse.x - _cameraPreviousMouseOffset.x) / 10.f, // @todo , should be const in class
				0.f
			});
			_cameraPreviousMouseOffset = sf::Vector2i(mouse.x, mouse.y);
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

	void LevelEditor::changeCameraLock()
	{
		auto* scene = _scenes.getCurrentScene();
		for (auto& entity : scene->getEntities("single")) { 
			if (auto* comp = entity->getComponentAs<CameraComponent>()) { 
				comp->setLock(!comp->getLock()); 
				comp->setNoMove(comp->getLock()); 
				if(comp->getLock()) {
					printMenuBarInfo("[x] Camera locked!");
				}
				else {
					printMenuBarInfo("[ ] Camera unlocked!");
				}
				break;
			} 
		} 
	}

	void LevelEditor::saveConfig(const std::string& path) {
		std::ofstream file{ path };
		Json config;

		config["mcCameraMovement"] = _isMCCameraMovement;
		config["dragAndDropObjects"] = _dragAndDropObjects;
		config["reloadArmatureOnStart"] = _reloadArmatureAtStart;

		file << std::setw(4) << config << std::endl;
	}

	void LevelEditor::loadConfig(const std::string& path) {
		if (std::experimental::filesystem::exists(path)) {
			std::ifstream file{ path };
			Json config;

			file >> config;

			if (config.find("mcCameraMovement") != config.end())
				_isMCCameraMovement = config["mcCameraMovement"];

			if (config.find("dragAndDropObjects") != config.end())
				_dragAndDropObjects = config["dragAndDropObjects"];

			if (config.find("reloadArmatureOnStart") != config.end())
				_reloadArmatureAtStart = config["reloadArmatureOnStart"];
		}
	}

	bool LevelEditor::reloadArmaturesAtStart()
	{
		return _reloadArmatureAtStart;
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
		_currentGroupRotation = glm::vec3();

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
