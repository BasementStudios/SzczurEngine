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
#include "SpriteComponent.hpp"

namespace rat
{

	/* Operators */
	/// Constructor
	World::World()
	: _collectingHolder{ { "background", {} }, { "foreground", {} }, { "path", {} }, { "single", {} } },
	_levelEditor{_collectingHolder, _spriteDisplayDataHolder, _armatureDisplayDataHolder}
	{
		
	}

	///
	void World::update()
	{
		_levelEditor.update(getModule<Input>().getManager(), getModule<Camera>());
	}

	///
	void World::render()
	{
		auto& window = getModule<Window>().getWindow();
		for (auto& holder : _collectingHolder) {
			for (auto& entity : holder.second) {
				if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr) {
					sf3d::RenderStates states = window.getDefaultRenderStates();
					states.transform *= entity.getTransform();
					window.draw(*ptr, states);
				}
			}
		}
		_levelEditor.render(window);
		#ifdef EDITOR
			
			
		#endif
	}

	World::EntitiesHolder_t::iterator World::addEntity(const std::string group, const std::string name) {
		auto& subGroup = _getSubHolder(group);
		if (auto it = _find(group, name); it == subGroup.end()) {
			subGroup.emplace_back(name);
			return subGroup.end()-1;
		}
		return subGroup.end();
	}

	Entity* World::getEntity(const std::string group, const std::string name) {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			return &(*it);
		}

		return nullptr;
	}

	const Entity* World::getEntity(const std::string group, const std::string name) const {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			return &(*it);
		}

		return nullptr;
	}

	///
	bool World::removeEntity(const std::string group, const std::string name) {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			_getSubHolder(group).erase(it);

			return true;
		}

		return false;
	}

	///
	void World::removeAllEntities(const std::string group){
		_getSubHolder(group).clear();
	}

	///
	void World::removeAllEntities()	{
		for (auto& holder : _collectingHolder) {
			holder.second.clear();
		}
	}

	///
	bool World::hasEntity(const std::string group, const std::string name) {
		return _find(group, name) != _getSubHolder(group).end();
	}


	///
	void World::loadFromFile(const std::string& filepath){
		std::ifstream file{ filepath };
		Json config;

		file >> config;

		for (auto it = config.begin(); it != config.end(); ++it)
		{
			for (Json& entity : it.value())
			{
				addEntity(it.key(), entity["name"]);

				getEntity(it.key(), entity["name"])->loadFromConfig(entity);
			}
		}
	}

	///
	void World::saveToFile(const std::string& filepath) const{
		std::ofstream file{ filepath };
		Json config;

		for (auto& holder : _collectingHolder) {
			Json& currGroup = config[holder.first] = Json::array();

			for (auto& entity : holder.second) {
				currGroup.push_back(Json::object());
				Json& currEntity = currGroup.back();

				entity.saveToConfig(currEntity);
			}
		}

		file << std::setw(4) << config << std::endl;
	}

	std::string World::_getUniqueName() {
		static size_t id = 0;

		return "unnamed_" + std::to_string(id++);
	}

	World::EntitiesHolder_t& World::_getSubHolder(const std::string& group) {
		return _collectingHolder.at(group);
	}

	const World::EntitiesHolder_t& World::_getSubHolder(const std::string& group) const	{
		return _collectingHolder.at(group);
	}

	typename World::EntitiesHolder_t::iterator World::_find(const std::string group, const std::string& name) {
		auto& subHolder = _getSubHolder(group);

		return std::find_if(subHolder.begin(), subHolder.end(), [&](const auto& arg) {
			return arg.getName() == name;
		});
	}

	typename World::EntitiesHolder_t::const_iterator World::_find(const std::string group, const std::string& name) const {
		const auto& subHolder = _getSubHolder(group);

		return std::find_if(subHolder.begin(), subHolder.end(), [&](const auto& arg) {
			return arg.getName() == name;
		});
	}
}
