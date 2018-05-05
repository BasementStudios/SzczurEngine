#pragma once

#include <fstream>

#include "boost/container/flat_map.hpp"
#include <Szczur/Utility/Modules/Module.hpp>
#include <Szczur/Modules/Input/Input.hpp>
#include <Szczur/Modules/Window/Window.hpp>
#include <Szczur/Modules/Camera/Camera.hpp>

#include "Entity.hpp"
#include "SpriteDisplayData.hpp"

namespace rat
{

	class World : public Module<Window, Input, Camera>
	{
	public:

		using EntitiesHolder_t   = std::vector<Entity>;
		using SpriteDisplayDataHolder_t = std::vector<SpriteDisplayData>;
		using CollectingHolder_t = boost::container::flat_map<std::string, EntitiesHolder_t>;
		///
		World();
			

		///
		World(const World&) = delete;

		///
		World& operator = (const World&) = delete;

		///
		World(World&&) = delete;

		///
		World& operator = (World&&) = delete;

		///
		~World() = default;

		///
		void update();

		///
		void render();

		///
		EntitiesHolder_t::iterator addEntity(const std::string group, const std::string name = _getUniqueName());

		///
		Entity* getEntity(const std::string group, const std::string name);

		const Entity* getEntity(const std::string group, const std::string name) const;

		///
		bool removeEntity(const std::string group, const std::string name);

		///
		void removeAllEntities(const std::string group);

		///
		void removeAllEntities();

		///
		bool hasEntity(const std::string group, const std::string name);

		///
		void loadFromFile(const std::string& filepath);

		///
		void saveToFile(const std::string& filepath) const;

	private:
		#ifdef EDITOR
			void _renderBar();
			void _renderDisplayDataManager();
			void _renderFocusedObjectsParams();
			void _renderObjectsList();
			void _renderModulesManager();

			bool _ifRenderObjectsList{true};
			bool _ifRenderDisplayDataManager{false};
			bool _anySelected{false};
			bool _ifRenderModulesManager{false};
			EntitiesHolder_t::iterator _focusedObject;
			EntitiesHolder_t::iterator _camera;
			Entity _freeCamera{""};
			float _freeCameraVelocity{50.f};
			bool _rotatingCamera{false};
			sf::Vector2i _previousMouse;

		#endif

		static std::string _getUniqueName();

		EntitiesHolder_t& _getSubHolder(const std::string& group);

		const EntitiesHolder_t& _getSubHolder(const std::string& group) const;

		typename EntitiesHolder_t::iterator _find(const std::string group, const std::string& name);

		typename EntitiesHolder_t::const_iterator _find(const std::string group, const std::string& name) const;

		CollectingHolder_t _collectingHolder;

		SpriteDisplayDataHolder_t _spriteDisplayDataHolder;

	};

}
