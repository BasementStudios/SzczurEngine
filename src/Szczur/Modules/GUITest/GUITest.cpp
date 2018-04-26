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
        testPatch.setPosition(0.f, 100.f);
        testPatch.setSize(1200, 700);
    }
    
    
    void GUITest::update(float deltaTime)
    {
        const auto& window = getModule<Window>().getWindow();

        auto mousePos = sf::Mouse::getPosition(window);
        testPatch.setSize(mousePos.x, 300);        
    }
    void GUITest::render()
    {
       _canvas.clear(sf::Color::Transparent);
        
        testPatch.draw(_canvas);
        //_canvas.draw(_root);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }
}