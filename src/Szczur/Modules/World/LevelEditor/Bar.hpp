#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class ScenesManager;
    class Bar {
    public:
        Bar(
            ScenesManager& scenes,
            bool& ifRenderObjectsList,
            bool& ifRenderSpriteDisplayDataManager,
            bool& ifRenderArmatureDisplayDataManager,
            bool& ifRenderDialogEditor,
            bool& ifRenderAudioEditor
        );
        void render();
        void update();
        void printMenuInfo(const std::string& text);
    private:
        bool& _ifRenderObjectsList;
        bool& _ifRenderSpriteDisplayDataManager;
        bool& _ifRenderArmatureDisplayDataManager;
        bool& _ifRenderDialogEditor;
        bool& _ifRenderAudioEditor;
        bool _ifShowImGuiDemoWindow;

        ScenesManager& _scenes;

        sf::Clock _menuInfoClock;
		std::string _menuInfo;
    };
}