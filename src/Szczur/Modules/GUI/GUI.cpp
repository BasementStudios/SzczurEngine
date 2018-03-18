#include "GUI.hpp"
#include <iostream>
namespace rat {
    GUI::GUI() :
    _root(new Widget) {
        LOG_INFO(this, "Module GUI constructed")
        initScript();
        auto& window = getModule<Window>().getWindow();
        _canvas.create(window.getSize().x, window.getSize().y);
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
    }

    GUI::~GUI() {
        LOG_INFO(this, "Module GUI destructed")
        delete _root;
    }

    Widget* GUI::addInterface() {
        Widget* widget = new Widget;
        _root->add(widget);
        return widget;
    }
    
    void GUI::input(const sf::Event& event) {
        _root->input(event);
    }

    void GUI::update(float deltaTime) {
        _root->update(deltaTime);
    }

    void GUI::render() {
        _canvas.clear(sf::Color::Transparent);
        
        _canvas.draw(*_root);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }

    void GUI::reload() {
        //_root->reload(getModule<Window>().getWindow().getSize());
    }
}