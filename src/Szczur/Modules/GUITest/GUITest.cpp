#include "GUITest.hpp"

namespace rat
{
    GUITest::GUITest()
    {
        auto& window = getModule<Window>().getWindow();
        _canvas.create(window.getSize().x, window.getSize().y);
    }
    void GUITest::init()
    {
        
    }
    
    
    void GUITest::update(float deltaTime)
    {

    }
    void GUITest::render()
    {
       _canvas.clear(sf::Color::Transparent);
        
        //_canvas.draw(_root);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }
}