#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
namespace rat
{
	class Equipment : public Module<Window, Input, GUI> {
	public:

		void init();
		void update(float deltaTime = (1.f / 60.f));
		void render();

		Equipment();
		~Equipment() {}

	private:

		Widget* _base{ nullptr };
		sf::RenderTexture _canvas;
	};
}