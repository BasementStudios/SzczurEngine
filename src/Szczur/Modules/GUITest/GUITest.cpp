#include "GUITest.hpp"

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


        gui.addAsset<sf::Texture>("Assets/Test/NineMinusPatchTest.png");
        testPatch.setTexture(gui.getAsset<sf::Texture>("Assets/Test/NineMinusPatchTest.png"), 200);
        //testPatch.setTextureRect({{0, 200}, {248, 248}});
        testPatch.setPosition(20.f, 20.f);
        testPatch.setSize(1200, 700);
        testPatch.setScale(0.2f, 0.2f);
        //testPatch.setDirection(Patch::Direction::None);
        gui.addAsset<sf::Texture>("Assets/Test/Scroller.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBar.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBound.png");
        
        scroller.setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        scroller.setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        scroller.setBoundTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        scroller.setSize({30, 300});
        scroller.setPosition(200.f, 200.f);


        widget = gui.addInterface();
        //widget->setMinimalSize(250, 250);
        //widget->setOriginProp(0.5f, 0.5f);

        std::cout << "Masslo\n";
        widget1 = new Widget;
        //widget1->setOriginProp(0.2f, 0.2f);
        widget1->setPosition(0.f, 0.f);
        widget->add(widget1);
        
        widget2 = new Widget;
        //widget2->setMinimalSize(50, 50);
        widget2->setPosition(0.f, 0.f);
        widget1->add(widget2);
        

        iWidget = new ImageWidget;
        iWidget->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        widget->add(iWidget);

        widget3 = new Widget;
        //widget3->setMinimalSize(25, 25);
        widget3->setPosition(0.f, 0.f);
        iWidget->add(widget3);

        std::cout << "Ogorek\n";
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
                std::cout << "Beng\n";
                hasBeenChanged = true;
                auto& gui = getModule<GUI>();
                iWidget->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
            }
        }

        //widget->setPosition(float(mousePos.x), float(mousePos.y));

        widget3->setPosition(_size.x, _size.y);
        //iWidget->setMinimalSize(_size.x, _size.y);

        /*
        //testPatch.setScale(_scale, _scale);
        testPatch.setSize(mousePos.x - 200, mousePos.y - 200);
        

        scroller.setSize(static_cast<sf::Vector2i>(_size));
        //scroller.setSize(sf::Vector2i(mousePos.x - 200, mousePos.y - 200));

        scroller.setScrollerPosition(static_cast<sf::Vector2f>(mousePos));
        scroller.setWidthProportion(_scale); 
        //scroller.setProportion(_prop); 
        scroller.setBoundShiftProportion(_shift);*/
    }
    void GUITest::render()
    {
       _canvas.clear(sf::Color::Transparent);
        
        //testPatch.draw(_canvas);
        //_canvas.draw(scroller);
        //_canvas.draw(testPatch);

        //_canvas.draw(*widget);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }
}