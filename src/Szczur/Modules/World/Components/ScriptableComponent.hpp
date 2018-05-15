#pragma once

#include "../Component.hpp"

#include <sol.hpp>

namespace rat {
	class Entity;

	class ScriptableComponent : public Component {
	public:
		///
		ScriptableComponent(Entity* parent);

		///
		void init();

		///
		void update(float deltaTime);

		/// Set script and run
		void loadScript(const std::string& path);

		/// Run script if is set
		void reloadScript();

		/// Run any script for object
		void loadAnyScript(const std::string& path);

		/// Set all values on default and remove script
		void reset();

	private:
		sol::function _updateCallback;
		sol::function _initCallback;
		std::string _scriptFilePath;
    };
}