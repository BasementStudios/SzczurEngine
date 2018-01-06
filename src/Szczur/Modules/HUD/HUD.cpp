#include "HUD.hpp"
#include <iostream>

namespace rat {
    void HUD::init() {
        bar.create("data/progressbar.png", sf::Color::Red, sf::Color(100,0,0));
        bar.setProgress(0.0f);
        bar.setPosition(100.f, 400.f);

        pp.move(10, 80);

        status.move(10,10);
    }
    void HUD::update(float deltaTime) {
        static int timeLeft = 20;
        status.update(deltaTime);
        pp.update(deltaTime);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            bar.addProgress(0.4f*deltaTime);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            bar.addProgress(-0.4f*deltaTime);
        
        timeLeft -= 1;
        if(timeLeft <= 0) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                if(!status.add(Status::ElementType::fire))
                    status.remove(Status::ElementType::fire);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                if(!status.add(Status::ElementType::electric))
                    status.remove(Status::ElementType::electric);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                if(!status.add(Status::ElementType::water))
                    status.remove(Status::ElementType::water);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                if(!status.add(Status::ElementType::wind))
                    status.remove(Status::ElementType::wind);
                timeLeft = 40;
            }

            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                pp.add(PP::ElementType::duch);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                pp.add(PP::ElementType::esencja);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                pp.add(PP::ElementType::fizyczny);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                pp.add(PP::ElementType::ogien);
                timeLeft = 40;
            }


            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                pp.add(PP::ElementType::powietrze);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                pp.add(PP::ElementType::woda);
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                pp.add(PP::ElementType::ziemia);
                timeLeft = 40;
            }


            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                pp.remove();
                timeLeft = 40;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                pp.randRemove();
                timeLeft = 40;
            }
            
        }
        
    }
    void HUD::render() {
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, bar);
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, status);
        _getModule<Canvas>().draw(Canvas::LayerId::HUD, pp);
    }
}