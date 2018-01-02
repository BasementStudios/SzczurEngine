#include "HUD.hpp"
#include <iostream>

namespace rat {
    void HUD::init() {
        bar.create("data/progressbar.png", sf::Color::Red, sf::Color(100,0,0));
        bar.setProgress(0.0f);
        bar.setPosition(100.f, 400.f);
        pp.setPosition(10.f, 10.f);

        pp.add(PP::ElementType::fire);
        pp.add(PP::ElementType::fire);
        pp.add(PP::ElementType::earth);
        pp.add(PP::ElementType::electric);
        pp.add(PP::ElementType::water);
        pp.add(PP::ElementType::water);
    }
    void HUD::update(float deltaTime) {
        static int timeLeft = 200;
        pp.update(deltaTime);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            bar.addProgress(0.4f*deltaTime);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            bar.addProgress(-0.4f*deltaTime);
        
        timeLeft -= 1;
        if(timeLeft <= 0) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                pp.add(PP::ElementType::fire);
                timeLeft = 200;
            }      
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                pp.add(PP::ElementType::earth);
                timeLeft = 200;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                pp.add(PP::ElementType::electric);
                timeLeft = 200;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                pp.add(PP::ElementType::water);
                timeLeft = 200;
            }
        }
        
    }
    void HUD::render() {
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, bar);
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, pp);
    }
}