#include "GUI.hpp"
#include <iostream>
#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "ScrollAreaWidget.hpp"

#include "Szczur/Utility/Logger.hpp"
#include <ctime>

namespace rat {
    GUI::GUI() 
    :
    _standartWindowSize(getModule<Window>().getWindow().getSize())
    {
        LOG_INFO(this, "Module GUI constructed")
        initScript();
        auto& mainWindow = getModule<Window>();
        auto& window = mainWindow.getWindow();
        mainWindow.pushGLStates(); 
        _canvas.create(window.getSize().x, window.getSize().y); 
        mainWindow.popGLStates();
    }

    void GUI::initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("GUI");
        module.set_function("addInterface", &GUI::addInterface, this);
        module.set_function("getTexture", &GUI::getAsset<sf::Texture>, this);
        module.set_function("getFont", &GUI::getAsset<sf::Font>, this);

        module.set_function("addTexture", &GUI::addAsset<sf::Texture>, this);
        module.set_function("addFont", &GUI::addAsset<sf::Font>, this);




        script.initClasses<Widget, ImageWidget, TextWidget, TextAreaWidget, ScrollAreaWidget>();
        //script.scriptFile("data/_GUI.lua");
    }

    GUI::~GUI() {
        LOG_INFO(this, "Module GUI destructed")
    }

    Widget* GUI::addInterface() {
        auto* interface = new InterfaceWidget;
        
        _root.add(interface);
        _interfaces.emplace_back(interface);
        return interface;
    }
    
    void GUI::input(const sf::Event& event) {
        if(event.type == sf::Event::Resized)
        {
            sf::Vector2u winSize = { event.size.width, event.size.height };

            sf::Vector2f winProp = { float(event.size.width) / float(_standartWindowSize.x),
            float(event.size.height) / float(_standartWindowSize.y) };
            Widget::setWinProp(winProp);

            for(auto* interface : _interfaces)
            {
                interface->updateSizeByWindowSize(winSize);
            }
        }
        _root.invokeInput(event);
        _root.input(event);
    }

    void GUI::update(float deltaTime) 
    {
        _root.invokeToCalculate();
        _root.update(deltaTime);
    }

    void GUI::render() {
        auto& mainWindow = getModule<Window>();

        mainWindow.pushGLStates();
 
        _canvas.clear(sf::Color::Transparent);
        _canvas.draw(_root);
        _canvas.display();

        mainWindow.getWindow().draw(sf::Sprite(_canvas.getTexture()));
 
        mainWindow.popGLStates();
    }
}