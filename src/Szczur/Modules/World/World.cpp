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
		using stoneObject_t = Object<BaseObject, SpriteComponent>; // @todo deduction WIP :f
		
		auto& stoneDisplayData = this->spriteDisplayData.emplace_back("Stone");
		auto& lochaDisplayData = this->spriteDisplayData.emplace_back("Locha");
		auto& lochaObject = this->objects.emplace<stoneObject_t>(BaseObject{"locha"}, SpriteComponent{&lochaDisplayData});
		auto& stoneObject = this->objects.emplace<stoneObject_t>(BaseObject{"kamyk"}, SpriteComponent{&stoneDisplayData});
		
		lochaObject.setPosition({1000.f, 0.f, -3000.f});
		stoneObject.setPosition({0.f, 0.f, -3000.f});
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	}
}

void World::_renderFocusedObjectsParams() {
	ImGui::Begin("Object Parameters");
	glm::vec3 position = _focusedObject->getPosition();
	glm::vec3 origin = _focusedObject->getOrigin();
	origin.y = -origin.y;
	glm::vec3 rotation = _focusedObject->getRotation();
	glm::vec3 scale = _focusedObject->getScale();
	ImGui::TextColored({1.f, 0.f, 0.f, 1.f}, _focusedObject->name.c_str());
	ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
	ImGui::DragFloat3("Origin", reinterpret_cast<float*>(&origin));
	ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation));
	ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.01f);
	_focusedObject->setPosition(position);
	_focusedObject->setOrigin(origin);
	_focusedObject->setRotation(rotation);
	_focusedObject->setScale(scale);
	ImGui::Spacing();
	if(SpriteComponent* object = dynamic_cast<SpriteComponent*>(_focusedObject)) {
		if(ImGui::BeginMenu("DisplayData")) {
			for(auto& it : spriteDisplayData) {
				if(ImGui::Button(it.name.c_str())) {
					object->setDisplayData(&it);
					LOG_INFO((void*)(&it));
				}
			}

			ImGui::EndMenu();
		}
	}

	ImGui::End();
}

void World::_renderObjectsList() {
	ImGui::Begin("Objects");
	this->objects.forEach<BaseObject>([this](auto&& object) {
		if(dynamic_cast<SpriteComponent*>(&object))
			ImGui::TextColored({1.f, 0.f, 0.f, 1.f}, "S");
		//else if(dynamic_cast<ArmatureComponent*>(&object))
			//ImGui::TextColored({0.f, 1.f, 0.f, 1.f}, "A");
		else
			ImGui::TextColored({0.8f, 0.8f, 0.8f, 1.f}, "N");
		ImGui::SameLine();
		if(ImGui::Button("Focus"))
			_focusedObject = &object;
		ImGui::SameLine();
		if(ImGui::CollapsingHeader(object.name.c_str())) {
			ImGui::Text("Test");
		}
	});


	ImGui::End();
}

/// update
void World::update(float deltaTime)
{
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		auto& target = getModule<Window>().getWindow();
		glm::vec2 mouse = {sf::Mouse::getPosition(target).x, sf::Mouse::getPosition(target).y};
		bool anyClicked{false};
		this->objects.forEach<BaseObject>([this, &anyClicked, &target, mouse](auto&& object) {
			if(target.getLinerByScreenPos(mouse).contains(object.getPosition()-glm::vec3{50.f, -50.f, 0.f}, {100.f, 100.f, 0.f})) {
				_focusedObject = &object;
				anyClicked = true;
			}
			if(!anyClicked);
				//_focusedObject = nullptr;
		});
	}
	#ifdef EDITOR
		if(_focusedObject)
			_renderFocusedObjectsParams();
		_renderObjectsList();
	#endif
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

	this->objects.forEach<sf3d::Drawable>([&target](auto&& drawable) {
		target.draw(drawable);
	});
	#ifdef EDITOR
		glDisable(GL_DEPTH_TEST);
		this->objects.forEach<sf3d::Transformable>([&target, &rect](auto&& transformable) {
			rect.setPosition(transformable.getPosition());
			target.draw(rect);
		});
		glEnable(GL_DEPTH_TEST);
	#endif
}

}
