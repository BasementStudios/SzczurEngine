#include "HUD.hpp"
#include <iostream>

namespace rat {
    void HUD::init() {
        bar.create("data/progressbar.png", sf::Color::Red, sf::Color(100,0,0));
        bar.setProgress(0.0f);
        bar.setPosition(100.f, 400.f);
        //status.add(Status::ElementType::fire);
        //status.add(Status::ElementType::water);
        //status.add(Status::ElementType::electric);
        status.move(10,10);
    }
    void HUD::update(float deltaTime) {
        static int timeLeft = 60;
        status.update(deltaTime);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            bar.addProgress(0.4f*deltaTime);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            bar.addProgress(-0.4f*deltaTime);
        
        timeLeft -= 1;
        if(timeLeft <= 0) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                if(!status.add(Status::ElementType::fire))
                    status.remove(Status::ElementType::fire);
                timeLeft = 60;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                if(!status.add(Status::ElementType::electric))
                    status.remove(Status::ElementType::electric);
                timeLeft = 60;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                if(!status.add(Status::ElementType::water))
                    status.remove(Status::ElementType::water);
                timeLeft = 60;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                if(!status.add(Status::ElementType::wind))
                    status.remove(Status::ElementType::wind);
                timeLeft = 60;
            }
        }
        
    }
    void HUD::render() {
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, bar);
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, status);
    }
}