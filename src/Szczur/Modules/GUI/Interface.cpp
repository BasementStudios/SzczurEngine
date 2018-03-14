#include "Interface.hpp"

namespace rat {
    Interface::Interface(BasicGuiAssetsManager* assets, sf::Vector2u windowSize) :
    _assets(assets) {

    }


    Widget* Interface::add(Widget* widget) {
        if(widget)
            _root.add(widget);
        else {
            LOG_ERROR("Given widget to Interface::add is nullptr")
        }
        return widget;
    }

    void Interface::input(const sf::Event& event) {
        _root.input(event);
    }

    void Interface::update(float deltaTime) {
        _root.update(deltaTime);
    }

    void Interface::reload(sf::Vector2u newSize) {

    }

    void Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_root);
    }

}