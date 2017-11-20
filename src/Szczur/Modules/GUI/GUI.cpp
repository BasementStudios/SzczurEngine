#include "GUI.hpp"

#include <iostream>

namespace rat {
    void GUI::init() {
        /*
        _root.add(new TextWidget(
            TextWidget::createText("Hello Fucking World!", 100u, sf::Color(255,0,255))
        )) ->setPosition({100.f, 100.f});
        
        _root.add( new CheckboxWidget(&_root) )
            ->setPosition({700.f, 700.f});*/
            /*
        Widget* button = _root.add( 
            (new ButtonWidget())->setCallback(
                [](Widget* owner) {
                    std::cout << "Hello\n" << owner->getSize().x << '\n' ;
                    return true;
                }
            )
        );
        button->setPosition({100.f, 100.f});
        button->add( new ImageWidget("data/button.png") );
        */

        Widget* button = _root.add(
            (new ButtonWidget())->setCallback([](){
                std::cout << "HEllo CLICKED\n";
                return true;
            })
        );
        button->add(
            new TextWidget(TextWidget::createText("Hello I'm good", 100u, sf::Color(255,255,0)))
        );
        button->add(
            (new ButtonWidget())->setCallback([](){
                std::cout << "HEllo123123 CLICKED\n";
                return true;
            })->add(
                new TextWidget(TextWidget::createText("Hello I'm goaaaod", 100u, sf::Color(255,255,0)))
            )
        );

        button->setPosition({50.f, 50.f});

    }

    void GUI::input(const sf::Event& event) {
        if(
            event.type == sf::Event::MouseButtonPressed || 
            event.type == sf::Event::MouseButtonReleased || 
            event.type == sf::Event::MouseMoved ||
            event.type == sf::Event::TextEntered
        ) {
            _root.input(event);
        }
    }

    void GUI::update(float deltaTime) {
        _root.update(deltaTime);
    }

    void GUI::render() {
        getModule<Canvas>().draw(Canvas::LayerId::GUI, _root);
    }
}