#include "GUI.hpp"
#include <iostream>
namespace rat {
    GUI::GUI() {
        LOG_INFO(this, "Module GUI constructed")
        initScript();
        //_initAssets();
        //auto* a = reinterpret_cast<CircleChooseWidget*>(addInterface("data/json.json")->get("_root")->get("test")); 
        //a->setAmount(7u);
        auto& window = getModule<Window>().getWindow();
        _canvas.create(window.getSize().x, window.getSize().y);


        //addAsset<sf::Texture>("data/dial_back.png");
        //addAsset<sf::Font>("data/consolab.ttf");
        /*ImageWidget* a = new ImageWidget(getAsset<sf::Texture>("data/dial_back.png"));
        auto* i = addInterface();
        i->add(a);

        a = new ImageWidget(getAsset<sf::Texture>("data/dial_back.png"));
        a->move({0.f, 200.f});
        i->add(a);*/
    }

    void GUI::initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("GUI");
        module.set_function("addInterface", &GUI::addInterface, this);
        module.set_function("getTexture", &GUI::getAsset<sf::Texture>, this);
        module.set_function("getFont", &GUI::getAsset<sf::Font>, this);

        module.set_function("addTexture", &GUI::addAsset<sf::Texture>, this);
        module.set_function("addFont", &GUI::addAsset<sf::Font>, this);



        script.initClasses<Widget, ImageWidget, TextWidget, TextAreaWidget>();
    }


    void GUI::_initAssets() {
        /*addAsset<sf::Texture>("data/button.png");
        addAsset<sf::Texture>("data/button-active.png");
        addAsset<sf::Texture>("data/button-clicked.png");
        addAsset<sf::Texture>("data/check.png");
        addAsset<sf::Texture>("data/check-on.png");
        addAsset<sf::Font>("data/consolab.ttf");
        addAsset<sf::Texture>("data/button.png");
        addAsset<Json>("data/json.json");*/
    }

    GUI::~GUI() {
        LOG_INFO(this, "Module GUI destructed")
        for(auto it : _interfaces)
            delete it;
    }

    Widget* GUI::addInterface() {
        Interface* interface = new Interface(&_assets, getModule<Window>().getWindow().getSize());
        _interfaces.push_back(interface);
        Widget* widget = new Widget;
        interface->add(widget);
        return widget;
    }
    
    void GUI::input(const sf::Event& event) {
        for(auto it : _interfaces)
            it->input(event);
    }

    void GUI::update(float deltaTime) {
        for(auto it : _interfaces)
            it->update(deltaTime);
    }

    void GUI::render() {
        _canvas.clear(sf::Color::Transparent);
        
        for(auto it : _interfaces)
            _canvas.draw(*it);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }

    void GUI::reload() {
        for(auto it : _interfaces)
            it->reload(getModule<Window>().getWindow().getSize());
    }
}