#include "GUI.hpp"

#include <iostream>

namespace rat {
    void GUI::init() {

        RadioWidget* radioWidget = new RadioWidget(4u, "data/check-on.png", "data/check.png");

        _root.add(radioWidget) -> move({100.f, 100.f});

        radioWidget->setCallback(Widget::CallbackType::onHover, [radioWidget](Widget*){
            std::cout << radioWidget->whichTriggered() << '\n';
        });



        /*
        Widget* menu = _root.add(new Widget());

        for(int ix = 0; ix<4; ix++) {
            Widget* button = new Widget();
            ImageWidget* start[3];
            start[0] = new ImageWidget("data/button.png");
            start[1] = new ImageWidget("data/button-active.png");
            start[2] = new ImageWidget("data/button-clicked.png");

            start[1]->invisible();
            start[2]->invisible();

            for(int i = 0; i<3; i++) {
                button->add(start[i]);
            }

            button->move({0.f, 64.f*ix});

            menu->add(button);

            button->setCallback( Widget::CallbackType::onHoverIn, [start](Widget*){
                start[0]->invisible();
                start[2]->invisible();
                start[1]->visible();
            } );
            button->setCallback( Widget::CallbackType::onHoverOut, [start](Widget*){
                start[0]->visible();
                start[2]->invisible();
                start[1]->invisible();
            } );
            button->setCallback( Widget::CallbackType::onPress, [start](Widget*){
                start[0]->invisible();
                start[2]->visible();
                start[1]->invisible();
            } );
        }
        


        
        for(int i = 0; i<5; i++){
            std::string name;
            switch(i) {
                case 0:
                    name = "Start";
                break;
                case 1:
                    name = "Load";
                break;
                case 2:
                    name = "Options";
                break;
                case 3:
                    name = "Credits";
                break;
                case 4:
                    name = "Exit";
                break;
            }
            TextWidget* textWidget = new TextWidget(
                TextWidget::createText(name, sf::Color::Red, 70u),
                "data/consolab.ttf"
            );

            Widget* test = _root.add(textWidget);

            test->setCallback(Widget::CallbackType::onHoverIn, [textWidget](Widget* owner) {
                //std::cout << "Yellow\n";
                textWidget->setColor(sf::Color::Yellow);
            });
            test->setCallback(Widget::CallbackType::onHoverOut, [textWidget](Widget* owner) {
                //std::cout << "Red\n";
                textWidget->setColor(sf::Color::Red);
            });
            test->setCallback(Widget::CallbackType::onPress, [textWidget](Widget* owner) {
                //std::cout << "Blue\n";
                textWidget->setColor(sf::Color::Blue);
            });
            test->setCallback(Widget::CallbackType::onRelease, [textWidget](Widget* owner) {
                //std::cout << "Red\n";
                textWidget->setColor(sf::Color::Red);
            });
            test->setPosition({255.f, 120.f + (110.f*i)});
        }
        _root.setCallback(Widget::CallbackType::onHover, [](Widget*){
            //std::cout << "Im in _root\n";
        });*/
        
        

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