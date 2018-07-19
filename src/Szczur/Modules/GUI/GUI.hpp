#pragma once
#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "Widget.hpp"
#include "InterfaceWidget.hpp"

#include "GuiAssetsManager.hpp"
 
namespace rat {
    class GUI : public Module<Input, Window, Script> { 
    public:
        GUI();

        ~GUI();

        void initScript();

        void input(const sf::Event& event);
        void update(float deltaTime=1.f/60.f);
        void render();

        InterfaceWidget* addInterface();

        template<typename T>
        T* getAsset(const std::string& key);

        sf::Texture* getTexture(const std::string& key);
        sf::Font* getFont(const std::string& key);

        template<typename T>
        void addAsset(const std::string& path);

        void addTexture(const std::string& key);
        void addFont(const std::string& key);
    private:
        //std::vector<Interface*> _interfaces;
        Widget _root;
        std::vector<InterfaceWidget*> _interfaces;
        //Widget _root;
        //GuiJson _guiJson;
        BasicGuiAssetsManager _assets;

        sf::RenderTexture _canvas;
        const sf::Vector2u _standartWindowSize;
    };
}

#include "GUI.tpp"