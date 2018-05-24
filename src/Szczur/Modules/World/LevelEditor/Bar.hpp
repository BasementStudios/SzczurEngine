#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class ScenesManager;
    class Bar {
    public:
        Bar(ScenesManager& scenes, bool& ifRenderObjectsList);
        void render();
        void update();
        void printMenuInfo(const std::string& text);
    private:
        bool& _ifRenderObjectsList;

        ScenesManager& _scenes;

        sf::Clock _menuInfoClock;
		std::string _menuInfo;
    };
}