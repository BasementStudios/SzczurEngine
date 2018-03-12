#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Json.hpp"

#include "Interface.hpp"
#include "Widget.hpp"
#include "GuiJson.hpp"
#include "GuiAssetsManager.hpp"

 
namespace rat {
    class GUI : public Module<Input, Window> { 
    public:
        GUI();

        ~GUI();

        void input(const sf::Event& event);
        void update(float deltaTime=1.f/60.f);
        void render();
        void reload();

        Interface* addInterface(const std::string& jsonFile);
        Interface* addInterface();

        template<typename T>
        T* getAsset(const std::string& key) const;

        template<typename T>
        void addAsset(const std::string& path);
    private:
        std::vector<Interface*> _interfaces;
        //Widget _root;
        //GuiJson _guiJson;
        BasicGuiAssetsManager _assets;

        sf::RenderTexture _canvas;

        void _initAssets();
    };
}

#include "GUI.tpp"