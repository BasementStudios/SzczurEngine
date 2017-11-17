#include "GUI.hpp"

namespace rat {
    void GUI::init() {
        _shape.setSize({100.f, 100.f});
        _shape.setFillColor(sf::Color::Red);

    }

    void GUI::update(float deltaTime) {
        _shape.rotate(10.f * deltaTime);
    }

    void GUI::render() {
        getModule<Canvas>().draw(Canvas::LayerId::GUI, _shape);
    }
}