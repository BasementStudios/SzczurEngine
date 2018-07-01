#include "LevelEditor.hpp"

#include <iostream>
#include <experimental/filesystem>

#ifdef OS_WINDOWS
#include <Shellapi.h>
#endif

#include <imgui.h>
// #include <NodeEditor/NodeEditor.h>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include "Szczur/Utility/SFML3D/CircleShape.hpp"

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

	void LevelEditor::updateCurrentCamera()
	{
		_currentCamera = nullptr;

		for (auto& ent : _scenes.getCurrentScene()->getEntities("single"))
		{
			if (auto* comp = ent->getComponentAs<CameraComponent>())
			{
				if (_objectsList.getSelectedID() == ent->getID() || comp->getLock())
				{
					_currentCamera = ent.get();
				}
			}
		}
	}

	void LevelEditor::updateCamera(Camera& camera)
	{
		sf3d::View view{ camera.getView() };
		if (_currentCamera)
			camera.setView(_currentCamera->getComponentAs<CameraComponent>()->getRecalculatedView(view));
		else
		{
			view.setRotation(_freeCamera.rotation);
			view.setCenter(_freeCamera.position);
			camera.setView(view);
		}
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

	void LevelEditor::update(InputManager& input, Camera& camera) {
		auto* scene = _scenes.getCurrentScene();
		auto& window = detail::globalPtr<Window>->getWindow();
		

		updateCurrentCamera();

		if(!ImGui::IsAnyWindowHovered()) {
			if(input.isPressed(Mouse::Left)) {
				auto mouse = getFixedMousePos(input.getMousePosition());
				
				auto linear = window.getLinerByScreenPos(mouse);

				_scenes.getCurrentScene()->forEach([&] (const std::string&, Entity& entity) {
					if (linear.contains(entity.getPosition() - glm::vec3{ 50.f, -50.f, 0.f }, { 100.f, 100.f, 0.f })) {
						if (_currentCamera && entity.getID() == _currentCamera->getID())
							return;

						if (_objectsList.isEntitySelected(&entity)) {
							_entityToUnselect = &entity;
							_entityToUnselectPos = entity.getPosition();
						}
						else {
							if (input.isKept(rat::Keyboard::LShift))
								_objectsList.addSelected(&entity);
							else
								_objectsList.select(entity.getID());
						}

						_isDragging = true;
						_dragLastPos = input.getMousePosition();
					}
				});
			}

			if (input.isReleased(Mouse::Left)) {
				_isDragging = false;

				if (_entityToUnselect != nullptr) {
					if (_entityToUnselectPos == _entityToUnselect->getPosition()) {
						_objectsList.removedSelected(_entityToUnselect);

						_entityToUnselect = nullptr;
					}
				}
			}

			if (input.isKept(Mouse::Left)) {
				if (_isDragging) {
					auto mouse = input.getMousePosition();

					auto offset = mouse - _dragLastPos;

					if (_objectsList.isAnySingleEntitySelected()) {
						auto entity = _objectsList.getSelectedEntity();
						entity->move(glm::vec3(offset.x, -offset.y, 0.f));
					}
					else if (_objectsList.isGroupSelected()) {
						for (auto& entity : _objectsList.getSelectedEntities()) {
							entity->move(glm::vec3(offset.x, -offset.y, 0.f));
						}
					}

					_dragLastPos = mouse;
				}
			}
		}

		// _scenes.getCurrentScene()->forEach([this, scene, &_currentCamera](const std::string&, Entity& entity){
		// 	if(auto* component = entity.getComponentAs<CameraComponent>(); component != nullptr) {
		// 		if(_objectsList.getSelectedID() == entity.getID() || component->getLock())
		// 			_currentCamera = &entity;
		// 	}
		// });
		
		if(ImGui::IsAnyItemActive() == false) {
			if(_currentCamera)
				_currentCamera->getComponentAs<CameraComponent>()->processEvents(input);
			else
				_freeCamera.processEvents(input);
		}

		if(input.isReleased(Keyboard::F1)) {
			if(_scenes.isGameRunning()) {
				printMenuBarInfo("Cannot save while game is running");
			}
			else {
				_scenes.menuSave();
				printMenuBarInfo(std::string("World saved in file: ")+_scenes.currentFilePath);
			}
		}
	}

	void FreeCamera::processEvents(InputManager& input) {

		if (detail::globalPtr<Dialog>->isDialogPlaying())
		 	return;

		velocity = 50.f;
		if(input.isKept(Keyboard::LShift)) {
			velocity = 200.f;
		}

		if(input.isKept(Keyboard::W)) {
			move({
				velocity * glm::sin(glm::radians(rotation.y)),
				0.f,
				-velocity * glm::cos(glm::radians(rotation.y))
			});
		}
		if(input.isKept(Keyboard::S))
			move({
				-velocity * glm::sin(glm::radians(rotation.y)),
				0.f,
				velocity * glm::cos(glm::radians(rotation.y))
			});
		if(input.isKept(Keyboard::D)) {
			move(glm::vec3{
				velocity * glm::cos(glm::radians(rotation.y)),
				0.f,
				velocity * glm::sin(glm::radians(rotation.y))
			});
		}
		if(input.isKept(Keyboard::A)) {
			move(glm::vec3{
				-velocity * glm::cos(glm::radians(rotation.y)),
				0.f,
				-velocity * glm::sin(glm::radians(rotation.y))
			});
		}
		if(input.isKept(Keyboard::Space))
			move({0.f, velocity, 0.f});
		if(input.isKept(Keyboard::LControl))
			move({0.f, -velocity, 0.f});
		if(rotating) {
			auto mouse = input.getMousePosition();
			rotate({
				(mouse.y - previousMouse.y)/10.f,
				(mouse.x - previousMouse.x)/10.f,
				0.f
			});
			previousMouse = mouse;
		}

		if(!ax::NodeEditor::IsActive())
		{
			if (input.isPressed(Mouse::Right))
			{
				rotating = true;
				previousMouse = input.getMousePosition();
			}
			if (input.isReleased(Mouse::Right))
			{
				rotating = false;
			}
		}
	}

	glm::vec2 LevelEditor::getFixedMousePos(const sf::Vector2i& pos) {
		glm::vec2 result;
		
		const auto& size = detail::globalPtr<Window>->getWindow().getSize();

		result.x = (pos.x * _defaultWindowSize.x) / size.x;
		result.y = (pos.y * _defaultWindowSize.y) / size.y;

		return result;
	}
}
