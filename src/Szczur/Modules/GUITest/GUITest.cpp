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

        scroller.setSize({50, 300});
        scroller.setPosition(200.f, 200.f);
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
        testPatch.setScale(_scale, _scale);


        //testPatch.setSize(mousePos.x - 20, mousePos.y - 20); 
        scroller.setSize({30, mousePos.y - 200});
        scroller.setWidthProportion(_scale); 
        scroller.setProportion(_prop); 
        scroller.setBoundShiftProportion(_shift);
    }
    void GUITest::render()
    {
       _canvas.clear(sf::Color::Transparent);
        
        //testPatch.draw(_canvas);
        _canvas.draw(scroller);
        //_canvas.draw(testPatch);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }
}