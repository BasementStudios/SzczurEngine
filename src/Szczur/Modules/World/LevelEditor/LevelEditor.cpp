#include "LevelEditor.hpp"

#include <iostream>
#include <experimental/filesystem>

#ifdef OS_WINDOWS
#include <shellapi.h>
#endif

#include <ImGui/imgui.h>
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

	void LevelEditor::_renderOriginRectangle(const glm::vec3& position, const glm::vec4& color, bool selected, sf3d::RenderTarget& target) {
		originRect.setSize({100.f, 100.f});
		originRect.setPosition(position);
		originRect.setOrigin({50.f, 50.f, -10.f});

		if(selected) {
			originRect.setColor({1.f, 0.3f, 1.f, 0.4f});
			target.draw(originRect);

			originRect.move({0.f, 0.f, 0.1f});
			originRect.setSize({80.f, 80.f});
			originRect.setOrigin({40.f, 40.f, -10.f});
		}
		else {
			originRect.setColor({0.6f, 0.f, 0.7f, 0.4f});
			target.draw(originRect);

			originRect.move({0.f, 0.f, 0.1f});
			originRect.setSize({70.f, 70.f});
			originRect.setOrigin({35.f, 35.f, -10.f});
		}

		originRect.setColor(color);
		target.draw(originRect);
	}

	void LevelEditor::_renderOriginCircle(const glm::vec3& position, const glm::vec4& color, bool selected, sf3d::RenderTarget& target) {
		originCircle.setRadius(50.f);
		originCircle.setPosition(position);
		originCircle.setOrigin({50.f, 50.f, -10.f});

		if(selected) {
			originCircle.setColor({1.f, 0.3f, 1.f, 0.4f});
			target.draw(originCircle);

			originCircle.move({0.f, 0.f, 0.1f});
			originCircle.setRadius(40.f);
			originCircle.setOrigin({40.f, 40.f, -10.f});
		}
		else {
			originCircle.setColor({0.6f, 0.f, 0.7f, 0.4f});
			target.draw(originCircle);

			originCircle.move({0.f, 0.f, 0.1f});
			originCircle.setRadius(35.f);
			originCircle.setOrigin({35.f, 35.f, -10.f});
		}

		originCircle.setColor(color);
		target.draw(originCircle);
	}


	void LevelEditor::_renderOrigins(sf3d::RenderTarget& target) {


		// Render origins for background group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("background")) {
			if(_objectsList.getSelectedID() == entity->getID()) {
				_renderOriginRectangle(entity->getPosition(), {0.f, 1.f, 1.f, 0.6f}, true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), {1.f, 1.f, 0.f, 0.6f}, false, target);
			}
		}

		// Render origins for path group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("path")) {			
			if(_objectsList.getSelectedID() == entity->getID()) {
				_renderOriginRectangle(entity->getPosition(), {0.f, 1.f, 1.f, 0.6f}, true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), {1.f, 1.f, 0.f, 0.6f}, false, target);
			}
		}

		// Render origins for foreground group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("foreground")) {			
			if(_objectsList.getSelectedID() == entity->getID()) {
				_renderOriginRectangle(entity->getPosition(), {0.f, 1.f, 1.f, 0.6f}, true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), {1.f, 1.f, 0.f, 0.6f}, false, target);
			}
		}

		// Render origins for single group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("single")) {			
			if(_objectsList.getSelectedID() == entity->getID()) {
				_renderOriginRectangle(entity->getPosition(), {0.f, 1.f, 1.f, 0.6f}, true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), {1.f, 1.f, 0.f, 0.6f}, false, target);
			}
		}

		// Render origins for entries group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("entries")) {			
			if(_objectsList.getSelectedID() == entity->getID()) {
				_renderOriginCircle(entity->getPosition(), {0.1f, 1.f, 0.4f, 0.6f}, true, target);
			}
			else {
				_renderOriginCircle(entity->getPosition(), {0.05f, 0.7f, 0.4f, 0.6f}, false, target);
			}
		}

		sf3d::CircleShape circ;
		sf3d::CircleShape circ2;
		circ.rotate({-90.f, 0.f, 0.f});
		circ.setColor({1.f, 0.f, 1.f, 0.2f});
		circ2.setColor({0.f, 1.f, 0.f, 0.2f});
		circ2.setRadius(50.f);
		
		circ2.setOrigin({50.f, 50.f, 0.f});
		_scenes.getCurrentScene()->forEach([&](const std::string& group, Entity& entity){
			if(auto* comp = entity.getComponentAs<InteractableComponent>(); comp != nullptr) {
				circ.setColor({1.f, 0.f, 1.f, 0.2f});
				circ.setPosition(entity.getPosition() + glm::vec3{0.f, comp->getHeight(), 0.f});
				float r = comp->getDistance();
				circ.setRadius(r);
				circ.setOrigin({r, r, 0.f});
				target.draw(circ);
			}
			if(auto* comp = entity.getComponentAs<TriggerComponent>(); comp != nullptr) {
				circ.setColor({1.f, 1.f, 0.f, 0.2f});
				circ.setPosition(entity.getPosition());
				float r = comp->getRadius();
				circ.setRadius(r);
				circ.setOrigin({r, r, 0.f});
				target.draw(circ);
			}
			if(auto* comp = entity.getComponentAs<ColliderComponent>(); comp != nullptr) {
				if (comp->isCircleCollider()) {
					circ.setColor({ 0.13f, 0.59f, 0.95f, 0.2f });
					circ.setPosition(entity.getPosition());
					float r = comp->getCircleRadius();
					circ.setRadius(r);
					circ.setOrigin({ r, r, 0.f });
					target.draw(circ);
				}
				if (comp->isBoxCollider()) {
					sf3d::RectangleShape rect;
					rect.rotate({ -90.f, 0.f, 0.f });
					rect.setColor({ 0.3f, 0.69f, 0.31f, 0.2f });
					rect.setPosition(entity.getPosition() + glm::vec3(0.f, 10.f, 0.f));
					auto& size = comp->getBoxSize();
					rect.setSize(size);
					rect.setOrigin({ size.x / 2.f, size.y / 2.f, 0.f });
					target.draw(rect);
				}
			}
		});
	}

	void LevelEditor::update(InputManager& input, Window& windowModule) {
		auto& window = windowModule.getWindow();
		auto mouse = input.getMousePosition();
		Entity* cameraEntity = nullptr;
		
		// Find selected camera
		for (auto& entity : _scenes.getCurrentScene()->getEntities("single")) { 
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
			auto linear = window.getLinerByScreenPos({(float)mouse.x, (float)mouse.y});
			if (input.isPressed(Mouse::Left)) {
				_scenes.getCurrentScene()->forEach([&](const std::string&, Entity& entity){
					if(linear.contains(entity.getPosition()-glm::vec3{50.f, -50.f, 0.f}, {100.f, 100.f, 0.f})) {
						if(cameraEntity && entity.getID() == cameraEntity->getID()) return;
						_objectsList.select(entity.getID());
					}
				});
			}
		}

		// _scenes.getCurrentScene()->forEach([this, scene, &cameraEntity](const std::string&, Entity& entity){
		// 	if(auto* component = entity.getComponentAs<CameraComponent>(); component != nullptr) {
		// 		if(_objectsList.getSelectedID() == entity.getID() || component->getLock())
		// 			cameraEntity = &entity;
		// 	}
		// });
		
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
					(mouse.y - _cameraPreviousMouseOffset.y) / 10.f, 
					(mouse.x - _cameraPreviousMouseOffset.x) / 10.f, // @todo , should be const in class
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

		if (input.isReleased(Keyboard::F1)) {
			_scenes.menuSave();
			printMenuBarInfo(std::string("World saved in file: ") + _scenes.currentFilePath);
		}
	}
}
