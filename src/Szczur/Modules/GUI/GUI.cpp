#include "GUI.hpp"

#include <iostream>
#include <fstream>

namespace rat {
    void GUI::init() {
        GuiJson::initJson("data/json.json", &_root, getModule<Canvas>().getWindow().getSize());
    }

    

    void GUI::input(const sf::Event& event) {
        if(
            event.type == sf::Event::MouseButtonPressed || 
            event.type == sf::Event::MouseButtonReleased || 
            event.type == sf::Event::MouseMoved ||
            event.type == sf::Event::TextEntered
        ) {
            _root.input(event);
        }
    }

    void GUI::update(float deltaTime) {
        _root.update(deltaTime);
    }

    void GUI::render() {
        getModule<Canvas>().draw(Canvas::LayerId::GUI, _root);
    }
}