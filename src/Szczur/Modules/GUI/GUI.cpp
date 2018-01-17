#include "GUI.hpp"

namespace rat {


    
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
        //getModule<Canvas>().draw(Canvas::LayerId::GUI, _root);
        getModule<Window>().getWindow().draw(_root);
    }

    void GUI::reload() {
        _guiJson.reload(getModule<Window>().getWindow().getSize(), &_root);
    }
}