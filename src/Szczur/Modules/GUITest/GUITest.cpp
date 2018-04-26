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
        testPatch.setTexture(gui.getAsset<sf::Texture>("Assets/Test/NineMinusPatchTest.png"));
        testPatch.setTextureRect({{0, 200}, {248, 248}});
        testPatch.setPosition(0.f, 0.f);
        testPatch.setSize(1200, 700);
        testPatch.setScale(0.2f, 0.2f);
        testPatch.setDirection(Patch::Direction::None);
    }
    
    
    void GUITest::update(float deltaTime)
    {
        const auto& window = getModule<Window>().getWindow();

        auto mousePos = sf::Mouse::getPosition(window);

        
        testPatch.setSize(mousePos.x, mousePos.y);        
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