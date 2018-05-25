#include "GUITest.hpp"

#include "StressTester.hpp"

namespace rat
{
    GUITest::GUITest()
    {
        auto& window = getModule<Window>().getWindow();
        _canvas.create(window.getSize().x, window.getSize().y);

        init();
    }
    void GUITest::init()
    {
        auto& gui = getModule<GUI>();


        gui.addAsset<sf::Texture>("Assets/Test/NinePatchTest.png");
        testPatch.setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"), 200);
        //testPatch.setTextureRect({{0, 200}, {248, 248}});
        testPatch.setPosition(50.f, 50.f);
        testPatch.setSize(1200, 700);
        testPatch.setScale(0.2f, 0.2f);
        //testPatch.setRotation(20.f);
        //testPatch.setDirection(Patch::Direction::None);
        gui.addAsset<sf::Texture>("Assets/Test/Scroller.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBar.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBound.png");
        
        scroller.setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"), 140u);
        scroller.setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        scroller.setBoundTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        scroller.setSize({30, 300});
        scroller.setPosition(200.f, 200.f);

        widget = gui.addInterface();
        widget->setSize(100, 100);
        widget->setPosition(0.f, 0.f);
        widget->setCallback(Widget::CallbackType::onHoverIn, [](Widget* self){
            //std::cout << "Beng\n";
        });
        widget->setPadding(10.f, 10.f);


        scroll->setSize(100, 500);
        scroll->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"), 139);
        scroll->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        scroll->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        scroll->setPosition(100.f, 100.f);
        widget->add(scroll);
/*
        iWidget = new ImageWidget;
        iWidget->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        iWidget->setPosition(0.f, 200.f);
        iWidget->setSize(50, 50);
        scroll->add(iWidget);
        iWidget->setCallback(Widget::CallbackType::onPress, [](Widget* owner){
            std::cout << "Oddej mi tyn rower!!!111!11!\n";
        });

        win->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"), 200);
        widget->add(win);
        win->setPosition(50.f, 50.f);
        win->setScale(0.2f, 0.2f);
        win->setPatchAmount(3, 5);        

        
        widget3 = new Widget;
        widget3->setSize(25, 25);
        widget3->setPosition(0.f, 0.f);
        iWidget->add(widget3);
        
       
       std::cout << "Grubosc widgeta: " << sizeof(Widget) << "\n"; 

       widget->invisible();

       std::cout << "Tester start\n";
       StressTester stresser;
       stresser.level = 10;
       stresser.branchAmount = 4;
       stresser.posRange = 80;
       stresser.sizeRange = 40;
       stresser.makeBranches(widget);
       std::cout << "Tester stop\n";
*/

        blue = new ImageWidget;
        blue->setCallback(Widget::CallbackType::onPress, [](Widget*){
            std::cout << "Blue\n";
        });
        widget->add(blue);
        gui.addAsset<sf::Texture>("Assets/GUITest/Blue.png");
        blue->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png"));
        blue->setColor({0, 0, 0});
        blue->setColor({255, 255, 255}, 12.f);
        
        red = new ImageWidget;
        red->setCallback(Widget::CallbackType::onPress, [](Widget*){
            std::cout << "Red\n";
        });
        gui.addAsset<sf::Texture>("Assets/GUITest/Red.png");
        red->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Red.png"));
        scroll->add(red);

        shape.setSize({100.f, 100.f});
        shape.setPosition(100.f, 100.f);
    }
    
    
    void GUITest::update(float deltaTime)
    {
        const auto& window = getModule<Window>().getWindow();

        auto mousePos = sf::Mouse::getPosition(window);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            _scale-= deltaTime * 0.4f;
            if(_scale < 0.2f) _scale = 0.2f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            _scale+= deltaTime * 0.4f;
            //if(_scale > 1.f) _scale = 1.f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
        {
            _prop+= deltaTime * 0.4f;
            if(_prop > 1.f) _prop = 1.f;            
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
        {
            _prop-= deltaTime * 0.4f;
            if(_prop < 0.f) _prop = 0.f;
        }
        /*
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            _shift+= deltaTime * 0.4f;
            if(_shift > 1.f) _shift = 1.f;            
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            _shift-= deltaTime * 0.4f;
            if(_shift < 0.f) _shift = 0.f;
        }
        */
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            _size.x -= deltaTime * 150.f;
            if(_size.x < 0.f) _size.x = 0.f;
            //if(_shift > 1.f) _shift = 1.f;            
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            _size.x += deltaTime * 150.f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            _size.y += deltaTime * 150.f;
            //if(_shift > 1.f) _shift = 1.f;            
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            _size.y -= deltaTime * 150.f;
            if(_size.y < 0.f) _size.y = 0.f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            if(!hasBeenChanged)
            {
                hasBeenChanged = true;
                auto& gui = getModule<GUI>();
                iWidget->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
            }
        }

        //win->setSize(float(mousePos.x - 50), float(mousePos.y - 50));

        widget->setPropOrigin(_prop, _prop);
        //widget->setPosition(_size.x, _size.y);
        //iWidget->setSize(_size.x, _size.y);

        
        win->setScale(_scale, _scale);
        win->setSize(mousePos.x - 75, mousePos.y - 75);
        

        //scroller.setSize(static_cast<sf::Vector2i>(_size));
        scroller.setSize(sf::Vector2i(mousePos.x - 200, mousePos.y - 200));
        /*
        scroller.setScrollerPosition(static_cast<sf::Vector2f>(mousePos));
        scroller.setWidthProportion(_scale); 
        //scroller.setProportion(_prop); 
        scroller.setBoundShiftProportion(_shift);*/

        //red->setPosition(_size);
        _animColor->update(deltaTime);
        shape.setFillColor(static_cast<ColorAnim*>(_animColor)->getActualColor());


    }
    void GUITest::render()
    {
       _canvas.clear(sf::Color::Transparent);


       _canvas.draw(shape);
        
        //testPatch.draw(_canvas);
        //_canvas.draw(testPatch);
        //_canvas.draw(scroller);
        //_canvas.draw(testPatch);

        //_canvas.draw(*widget);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }
}