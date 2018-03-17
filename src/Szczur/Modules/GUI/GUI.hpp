#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "Widget.hpp"
#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "GuiAssetsManager.hpp"
#include "ScrollAreaWidget.hpp"

 
namespace rat {
    class GUI : public Module<Input, Window, Script> { 
    public:
        GUI();

        ~GUI();

        void initScript();

        void input(const sf::Event& event);
        void update(float deltaTime=1.f/60.f);
        void render();
        void reload();

        Widget* addInterface();

        template<typename T>
        T* getAsset(const std::string& key) const;

        template<typename T>
        void addAsset(const std::string& path);
    private:
        //std::vector<Interface*> _interfaces;
        Widget* _root;
        //Widget _root;
        //GuiJson _guiJson;
        BasicGuiAssetsManager _assets;

        sf::RenderTexture _canvas;
    };
}

#include "GUI.tpp"