#include "Canvas.hpp"

namespace rat {
    void Canvas::init() {

    }

    void Canvas::addLayer(rat::Hash32_t layerId) {
        _layers.emplace(layerId, new Layer);
    }

    void Canvas::resize(const sf::Vector2u& size) {
        _size = size;
        for (auto& layer : _layers) {
            std::get<1>(layer)->create(size);
        }
    }

    void Canvas::resize(const sf::RenderWindow& window) {
        resize(window.getSize());
    }

    void Canvas::display(sf::RenderTarget& target, rat::Hash32_t layerId) {
        sf::Sprite spr;
        _layers.at(layerId)->display();
        spr.setTexture(_layers.at(layerId)->getTexture());
        target.draw(spr);
        _layers.at(layerId)->clear(sf::Color::Transparent);
    }

    void Canvas::render(sf::Drawable& drawable, rat::Hash32_t layerId) {
        _layers.at(layerId)->draw(drawable);
    }

    void Canvas::render(Drawable& drawable, rat::Hash32_t layerId) {
        _layers.at(layerId)->draw(drawable);
    }

    Layer& Canvas::getLayer(rat::Hash32_t layerId) {
        return *_layers.at(layerId);
    }

    const Layer& Canvas::getLayer(rat::Hash32_t layerId) const {
        return *_layers.at(layerId);
    }

    sf::Vector2u Canvas::getLayersSize() const {
        return _size;
    }
}
