#include "LevelEditor.hpp"

#include <iostream>
#include <experimental/filesystem>

#ifdef OS_WINDOWS
#include <Shellapi.h>
#endif

#include <ImGui/imgui.h>
#include <NodeEditor/NodeEditor.h>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include "Szczur/Utility/SFML3D/CircleShape.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"

#include "Szczur/Modules/Window/Window.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include "Szczur/Modules/Dialog/Dialog.hpp"

#include "Szczur/Modules/Trace/Timeline.hpp"
#include "Szczur/Modules/Trace/Actions/AnimAction.hpp"
#include "Szczur/Modules/Trace/Actions/MoveAction.hpp"

#include "Szczur/Modules/DialogEditor/DialogEditor.hpp"

#include "Szczur/Modules/Input/InputManager.hpp"

#include "Szczur/Modules/Window/Window.hpp"

namespace rat {
	LevelEditor::LevelEditor(ScenesManager& scenes) :
	_scenes(scenes),
	_objectsList{scenes},
	_objectParameters{scenes},
	_spriteDisplayDataManager{scenes},
	_armatureDisplayDataManager{scenes},
	_bar{scenes, 
		_objectsList.getBool(),
		_spriteDisplayDataManager.getBool(),
		_armatureDisplayDataManager.getBool(),
		_ifRenderDialogEditor,
		_ifRenderAudioEditor
	} {
		_freeCamera.move({1000.f,500.f,2000.f});
		detail::globalPtr<Window>->getWindow().setRenderDistance(300.f);
		_dialogEditor = detail::globalPtr<DialogEditor>;
		_audioEditor = detail::globalPtr<AudioEditor>;
	}

	void LevelEditor::render(sf3d::RenderTarget& target) {
		auto* scene = _scenes.getCurrentScene();
		if(scene) {
			_bar.render();
			_objectParameters.render();
			_objectsList.render();
			_spriteDisplayDataManager.render();
			_armatureDisplayDataManager.render();
			if (_ifRenderDialogEditor) {
				_dialogEditor->update();
			}
			if (_ifRenderAudioEditor) {
				_audioEditor->render();
			}

			scene = _scenes.getCurrentScene();
			sf3d::RectangleShape rect({100.f, 100.f});
			sf3d::CircleShape circ;
			sf3d::CircleShape circ2;
			circ.rotate({-90.f, 0.f, 0.f});
			circ.setColor({1.f, 0.f, 1.f, 0.2f});
			circ2.setColor({0.f, 1.f, 0.f, 0.2f});
			circ2.setRadius(50.f);
			circ2.setOrigin({50.f, 50.f, 0.f});
			rect.setColor({1.f, 1.f, 0.f, 0.2f});
			rect.setOrigin({50.f, 50.f, -10.f});

			//glDisable(GL_DEPTH_TEST);
			scene->forEach([&](const std::string& group, Entity& entity){
				rect.setPosition(entity.getPosition());
				if(scene->focusedObject == entity.getID() && scene->anySelected) {
					rect.setSize({100.f, 100.f});
					rect.setOrigin({50.f, 50.f, -10.f});
					rect.setColor({1.f, 0.3f, 1.f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, 0.1f});
					rect.setSize({80.f, 80.f});
					rect.setOrigin({40.f, 40.f, -10.f});
					rect.setColor({0.f, 1.f, 1.f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, -0.1f});
					// rect.setOutlineColor({1.f, 1.f, 0.f, 0.8f})
					// rect.setOutlineThickness(2.f);
				}
				else {
					rect.setSize({100.f, 100.f});
					rect.setOrigin({50.f, 50.f, -10.f});
					rect.setColor({0.7f, 0.f, 0.8f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, 0.1f});
					rect.setSize({80.f, 80.f});
					rect.setOrigin({40.f, 40.f, -10.f});
					rect.setColor({1.f, 1.f, 0.f, 0.4f});
					target.draw(rect);
					rect.move({0.f, 0.f, -0.1f});
					// rect.setOutlineThickness(0.f);
				}
			});
			scene->forEach([&](const std::string& group, Entity& entity){
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
			});
			for(auto& it : _scenes.getCurrentScene()->getEntrances()) {
				circ2.setPosition(it.position);
				target.draw(circ2);
			}
			//glEnable(GL_DEPTH_TEST);
		}
	}

	void LevelEditor::update(InputManager& input, Camera& camera) {
		auto* scene = _scenes.getCurrentScene();
		auto& window = detail::globalPtr<Window>->getWindow();
		
		auto mouse = input.getMousePosition();

		auto linear = window.getLinerByScreenPos({(float)mouse.x, (float)mouse.y});
		
		// if(input.isReleased(Mouse::Left)) {
		// 	_scenes.getCurrentScene()->forEach([&linear, scene](const std::string&, Entity& entity){
		// 		if(linear.contains(entity.getPosition()-glm::vec3{50.f, -50.f, 0.f}, {100.f, 100.f, 0.f})) {
		// 			scene->focusedObject = entity.getID();
		// 			scene->anySelected = true;
		// 		}
		// 	});
		// }

		sf3d::View view;
		Entity* currentCamera{nullptr};
		_scenes.getCurrentScene()->forEach([this, scene, &currentCamera](const std::string&, Entity& entity){
			if(auto* component = entity.getComponentAs<CameraComponent>(); component != nullptr) {
				if((scene->focusedObject == entity.getID() && scene->anySelected) || component->getLock())
					currentCamera = &entity;
			}
		});
		if(ImGui::IsAnyItemActive() == false) {
			if(currentCamera==nullptr)
				_freeCamera.processEvents(input);
			else
				currentCamera->getComponentAs<CameraComponent>()->processEvents(input);
		}
		if(currentCamera) {
			view.setRotation(currentCamera->getRotation());
			view.setCenter(currentCamera->getPosition());
		}
		else {
			//std::cout << _freeCamera.position.x << ' ' << _freeCamera.position.y << '\n';
			view.setRotation(_freeCamera.rotation);
			view.setCenter(_freeCamera.position);
		}
		camera.setView(view);

		if(input.isReleased(Keyboard::F1)) {
			_scenes.menuSave();
		}
		_bar.update();
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

		if (!ax::NodeEditor::IsActive())
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
}
