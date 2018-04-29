#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"

#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include <glm/glm.hpp>
#include <ImGui/imgui.h> 
#include <SFML/Graphics.hpp>

namespace rat
{

/* Operators */
/// Constructor
World::World()
{
	LOG_INFO(this, ": World module initializing"); 
	objects.emplace("Foreground", ObjectsHolder_t{});
	objects.emplace("Background", ObjectsHolder_t{});
	objects.emplace("Path", ObjectsHolder_t{});
	objects.emplace("Single", ObjectsHolder_t{});

	this->init();
	LOG_INFO(this, ": World module initialized!"); 
}
/// Destructor
World::~World()
{
    LOG_INFO(this, ": World module destructed"); 
}



/* Methods*/
/// init
void World::init()
{
	// @info For testing purposes
	{
		this->spriteDisplayData.reserve(20);
		for(auto& it : objects) {
			it.second.getContainer<Object<BaseObject, SpriteComponent>>().reserve(20);
			//it.second.getContainer<Object<BaseObject>>().reserve(20);
		}
		//_camera = &(this->objects["Singsle"].emplace(BaseObject{"Camera"}, SpriteDisplayData{nullptr}));
		_camera = &(objects["Single"].emplace(BaseObject{"Camera"}, SpriteComponent{nullptr}));
		//this->objects["Path"].emplace(BaseObject{"locha"}, SpriteComponent{nullptr}).setPosition({0.f, 0.f, -1000.f});

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	}
}
#ifdef EDITOR

void World::_renderBar() {
	if(ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu("Files")) {
			if(ImGui::MenuItem("Load")) {
				std::cout << "LOAD\n";
			}
			if(ImGui::MenuItem("Save")) {
				std::cout << "Save\n";
			}
			if(ImGui::MenuItem("Save as")) {
				std::cout << "Save as\n";
			}
			if(ImGui::MenuItem("Exit")) {
				std::cout << "Exit\n";
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Tools")) {
			ImGui::MenuItem("Objects List", nullptr, &_ifRenderObjectsList);
			ImGui::MenuItem("Display Data Manager", nullptr, &_ifRenderDisplayDataManager);
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}

void World::_renderDisplayDataManager() {
	static char enteredText[255];
	if(ImGui::Begin("Display Data Manager", &_ifRenderDisplayDataManager)) {
		if(ImGui::InputText("", enteredText, 255)) {
		}
		ImGui::SameLine();
		if(ImGui::Button("Add")) {
			try{
				spriteDisplayData.emplace_back(enteredText);
			}
			catch(const std::exception& exc) {
			}
			for(int i = 0; i<255; ++i)
				enteredText[i] = '\0';
		}
		ImGui::Separator();
		for(auto it = spriteDisplayData.begin(); it!=spriteDisplayData.end(); ++it) {
			if(ImGui::SmallButton("-")) {
				//spriteDisplayData.erase(it);
				//--it;
			}
			ImGui::SameLine();
			ImGui::Text(it->name.c_str());
		}
	}
	ImGui::End();
}

void World::_renderFocusedObjectsParams() {
	if(ImGui::Begin("Object Parameters")) {
		glm::vec3 position = _focusedObject->getPosition();
		glm::vec3 origin = _focusedObject->getOrigin();
		origin.y = -origin.y;
		glm::vec3 rotation = _focusedObject->getRotation();
		glm::vec3 scale = _focusedObject->getScale();
		char name[255];
		std::strcpy(&name[0], _focusedObject->name.c_str());
		ImGui::InputText("", name, 255);
		_focusedObject->name = std::string(name);
		ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
		ImGui::DragFloat3("Origin", reinterpret_cast<float*>(&origin));
		ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation));
		ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.01f);
		ImGui::Checkbox("Locked", &(_focusedObject->locked));
		_focusedObject->setPosition(position);
		_focusedObject->setOrigin(origin);
		_focusedObject->setRotation(rotation);
		_focusedObject->setScale(scale);
		ImGui::Spacing();
		if(SpriteComponent* object = dynamic_cast<SpriteComponent*>(_focusedObject)) {
			if(ImGui::BeginMenu("DisplayData")) {
				if(ImGui::MenuItem("None", nullptr, object->getDisplayData() == nullptr))
					object->setDisplayData(nullptr);
				for(auto& it : spriteDisplayData) {
					if(ImGui::MenuItem(it.name.c_str(), nullptr, object->getDisplayData() == &it))
						object->setDisplayData(&it);
				}

				ImGui::EndMenu();
			}
		}

	}
	ImGui::End();
}

void World::_renderObjectsList() {
	if(ImGui::Begin("Objects", &_ifRenderObjectsList)){
		ImGui::Separator();
		if(ImGui::BeginChild("Objects")) {
			int i{0};
			for(auto& it : objects) {
				if(ImGui::SmallButton((std::string{"+###"} + std::to_string(i++)).c_str())) {
					_focusedObject = &(it.second.emplace(BaseObject{"Unnamed"}, SpriteComponent{nullptr}));
					_focusedObject->move({0.f, 0.f, -1000.f});
				}
				ImGui::SameLine();
				if(ImGui::TreeNode(it.first.c_str())) {
					it.second.forEach<BaseObject>([this, &i](auto&& object) {
						bool temp = &object == _focusedObject;
						if(ImGui::Selectable((object.name+"###"+std::to_string(i++)).c_str(), temp)) {
							if(temp)
								_focusedObject = nullptr;
							else
								_focusedObject = &object;
						}
					});
					ImGui::TreePop();
				}
				ImGui::Separator();
			}
			ImGui::EndChild();
		}

	}
	ImGui::End();
}
#endif
/// update
void World::update(float deltaTime)
{
	auto& input = getModule<Input>().getManager();

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		auto& target = getModule<Window>().getWindow();
		glm::vec2 mouse = {sf::Mouse::getPosition(target).x, sf::Mouse::getPosition(target).y};
		bool anyClicked{false};

		for(auto& it : objects) {
			it.second.forEach<BaseObject>([this, &anyClicked, &target, mouse](auto&& object) {
				if(!object.locked) {
					if(target.getLinerByScreenPos(mouse).contains(object.getPosition()-glm::vec3{50.f, -50.f, 0.f}, {100.f, 100.f, 0.f})) {
						_focusedObject = &object;
						anyClicked = true;
					}
					if(!anyClicked);
						//_focusedObject = nullptr;
				}
			});
		}
	}
	//this->pathObjects.forEach<InputController>([&input](auto&& object) {
		//object.update(input);
	//});
	#ifdef EDITOR
		_renderBar();
		if(_focusedObject)
			_renderFocusedObjectsParams();
		if(_ifRenderObjectsList)
			_renderObjectsList();
		if(_ifRenderDisplayDataManager)
			_renderDisplayDataManager();
	#endif
	sf3d::View view;
	view.setRotation(_camera->getRotation());
	view.setCenter(_camera->getPosition());
	//view.setCenter({1000.f, 0.f, 0.f});
	getModule<Camera>().setView(view);
}

/// render
void World::render()
{
	auto& target = getModule<Window>().getWindow(); // @todo . by render arg
	
	glEnable(GL_DEPTH_TEST);
    target.clear(37, 37, 48, 255, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	sf3d::RectangleShape rect({100.f, 100.f});
	rect.setColor({1.f, 1.f, 0.f, 0.2f});
	rect.setOrigin({50.f, 50.f, 0.f});

	for(auto& it : objects) {
		it.second.forEach<sf3d::Drawable>([&target](auto&& drawable) {
			target.draw(drawable);
		});
	}
	#ifdef EDITOR
		glDisable(GL_DEPTH_TEST);
		
		for(auto& it : objects) {
			it.second.forEach<BaseObject>([this, &target, &rect](auto&& object) {
				if(!object.locked) {
					if(_focusedObject == &object)
						rect.setColor({1.f, 0.f, 1.f, 0.2f});
					else
						rect.setColor({1.f, 1.f, 0.f, 0.2f});
					rect.setPosition(object.getPosition());
					target.draw(rect);
				}
			});
		}

		glEnable(GL_DEPTH_TEST);
	#endif
}

}
