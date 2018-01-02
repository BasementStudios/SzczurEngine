#include "HUD.hpp"
#include <iostream>

namespace rat {
    void HUD::init() {
        bar.create("data/progressbar.png", sf::Color::Red, sf::Color(100,0,0));
        bar.setProgress(0.0f);
    }
    void HUD::update(float deltaTime) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            bar.addProgress(0.4f*deltaTime);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            bar.addProgress(-0.4f*deltaTime);
    }
    void HUD::render() {
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, bar);
    }
}