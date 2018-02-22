#include "Interface.hpp"

namespace rat {
    Interface::Interface(BasicGuiAssetsManager* assets, sf::Vector2u windowSize) :
    _assets(assets) {

    }

    Interface::Interface(BasicGuiAssetsManager* assets, sf::Vector2u windowSize, const std::string& jsonFile) :
    _assets(assets) {
        GuiJson guiJson;
        guiJson.init(assets->get<Json>(jsonFile), assets, &_root, windowSize);
    }

    Widget* Interface::add(const std::string& key, Widget* widget) {
        return _root.add(key, widget);
    }

    Widget* Interface::get(const std::string& key) const {
        return _root.get(key);
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