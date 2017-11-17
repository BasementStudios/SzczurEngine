#include "GUI.hpp"

namespace rat {
    void GUI::init() {
        _root.add(new TextWidget(
            &_root, 
            TextWidget::createText("Hello Fucking World!", 100u, sf::Color(255,0,255))
        ));
    }

    void GUI::input(sf::Event event) {
        _root.input(event);
    }

    void GUI::update(float deltaTime) {
        _root.update(deltaTime);
    }

    void GUI::render() {
        getModule<Canvas>().draw(Canvas::LayerId::GUI, _root);
    }
}