#include "GUI.hpp"
#include <iostream>
#include "ImageWidget.hpp"
#include "WindowWidget.hpp"
#include "ListWidget.hpp"
#include "InterfaceWidget.hpp"
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
        auto winSize = window.getSize();
        _canvas.create(winSize.x, winSize.y); 

        _root.setSize(static_cast<sf::Vector2f>(winSize));
        _root.makeChildrenUnresizable();
    }

    void GUI::initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("GUI");
        module.set_function("addInterface", &GUI::addInterface, this);
        module.set_function("getTexture", &GUI::getAsset<sf::Texture>, this);
        module.set_function("getFont", &GUI::getAsset<sf::Font>, this);

        module.set_function("addTexture", &GUI::addAsset<sf::Texture>, this);
        module.set_function("addFont", &GUI::addAsset<sf::Font>, this);




        script.initClasses<Widget, InterfaceWidget, WindowWidget, ListWidget, ImageWidget, TextWidget, TextAreaWidget, ScrollAreaWidget>();
        //script.scriptFile("data/_GUI.lua");
    }

    GUI::~GUI() {
        LOG_INFO(this, "Module GUI destructed")
    }

    InterfaceWidget* GUI::addInterface() 
    {
        auto* interface = new InterfaceWidget;
        
        _root.add(interface);
        interface->updateSizeByWindowSize(static_cast<sf::Vector2u>(_root.getMinimalSize()));
        interface->setPropPosition(0.5f, 0.5f);
        _interfaces.emplace_back(interface);
        return interface;
    }
    
    void GUI::input(const sf::Event& event) 
    {
        if(event.type == sf::Event::Resized)
        {
            sf::Vector2u winSize = { (unsigned int)(event.size.width), (unsigned int)(event.size.height) };

            auto& mainWindow = getModule<Window>();
            mainWindow.setVideoMode(sf::VideoMode{winSize.x, winSize.y});

            _canvas.create(winSize.x, winSize.y);

            _root.setSize(static_cast<sf::Vector2f>(winSize));

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
        _root.invokeToCalcPropPosition();
        _root.invokeToCalcPosition();
        _root.update(deltaTime);
    }

    sf::Texture* GUI::getTexture(const std::string& key)
    {
        return getAsset<sf::Texture>(key);
    }
    sf::Font* GUI::getFont(const std::string& key)
    {
        return getAsset<sf::Font>(key);
    }

    void GUI::addTexture(const std::string& key)
    {
        addAsset<sf::Texture>(key);
    }
    void GUI::addFont(const std::string& key)
    {
        addAsset<sf::Font>(key);
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