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
        widget->setSize(100, 100);
        widget->setPosition(250.f, 250.f);
        widget->setCallback(Widget::CallbackType::onHoverIn, [](Widget* self){
            std::cout << "Beng\n";
        });


        scroll->setSize(100, 200);
        scroll->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        scroll->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        scroll->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        scroll->setPosition(100.f, 100.f);
        widget->add(scroll);

        iWidget = new ImageWidget;
        iWidget->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        iWidget->setPosition(0.f, 0.f);
        iWidget->setSize(50, 50);
        scroll->add(iWidget);
        /*
        widget3 = new Widget;
        widget3->setSize(25, 25);
        widget3->setPosition(0.f, 0.f);
        iWidget->add(widget3);
        */

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

        scroll->setPosition(float(mousePos.x), float(mousePos.y));

        widget->setPropOrigin(_prop, _prop);
        widget->setPosition(_size.x, _size.y);
        //iWidget->setSize(_size.x, _size.y);

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