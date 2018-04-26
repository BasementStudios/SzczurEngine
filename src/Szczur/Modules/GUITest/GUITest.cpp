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
    }
    
    
    void GUITest::update(float deltaTime)
    {
        const auto& window = getModule<Window>().getWindow();

        auto mousePos = sf::Mouse::getPosition(window);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            _scale-= deltaTime * 0.2f;
            if(_scale < 0.2f) _scale = 0.f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            _scale+= deltaTime * 0.2f;
        }
        testPatch.setScale(_scale, _scale);


        testPatch.setSize(mousePos.x - 20, mousePos.y - 20);        
    }
    void GUITest::render()
    {
       _canvas.clear(sf::Color::Transparent);
        
        testPatch.draw(_canvas);
        //_canvas.draw(testPatch);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }
}