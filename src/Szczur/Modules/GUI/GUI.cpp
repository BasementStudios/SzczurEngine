#include "GUI.hpp"

namespace rat {

    void GUI::_initAssets() {
    }

    GUI::~GUI() {
        for(auto it : _interfaces)
            delete it;
    }

    Interface* GUI::addInterface(const std::string& jsonFile) {
        Interface* interface = new Interface(&_assets, getModule<Window>().getWindow().getSize(), jsonFile);
        _interfaces.push_back(interface);
        return interface;
    }

    Interface* GUI::addInterface() {
        Interface* interface = new Interface(&_assets, getModule<Window>().getWindow().getSize());
        _interfaces.push_back(interface);
        return interface;
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
        for(auto it : _interfaces)
            getModule<Window>().getWindow().draw(*it);
    }

    void GUI::reload() {
        for(auto it : _interfaces)
            it->reload(getModule<Window>().getWindow().getSize());
    }
}