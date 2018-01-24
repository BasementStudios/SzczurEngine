#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Json.hpp"

#include "Interface.hpp"
#include "Widget.hpp"
#include "GuiJson.hpp"
#include "GuiAssetsManager.hpp"

 
namespace rat {
    class GUI : public Module<Canvas, Assets, Input, Window> { 
    public:
        template<typename Tuple>
        GUI(Tuple&& tuple);

        ~GUI();

        void input(const sf::Event& event);
        void update(float deltaTime);
        void render();
        void reload();

        Interface* addInterface(const std::string& jsonFile);

        template<typename T>
        T* getAsset(const std::string& key) const;

        template<typename T>
        void addAsset(const std::string& path);
    private:
        std::vector<Interface*> _interfaces;
        //Widget _root;
        //GuiJson _guiJson;
        BasicGuiAssetsManager _assets;

        void _initAssets();
    };
}

#include "GUI.tpp"