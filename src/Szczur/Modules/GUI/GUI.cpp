#include "GUI.hpp"

namespace rat {
    void GUI::init() {
        /*_root.add(new TextWidget(
            &_root, 
            TextWidget::createText("Hello Fucking World!", 100u, sf::Color(255,0,255))
        )) ->setPosition({500.f, 500.f});
        */
        _root.add( new CheckboxWidget(&_root) )
            ->setPosition({200.f, 200.f});
    }

    void GUI::input(const sf::Event& event) {
        _root.input(event);
    }

    void GUI::update(float deltaTime) {
        _root.update(deltaTime);
    }

    void GUI::render() {
        getModule<Canvas>().draw(Canvas::LayerId::GUI, _root);
    }
}