#include "GUITest.hpp"

#include "StressTester.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"


namespace rat
{
    GUITest::GUITest()
    {
        auto& mainWindow = getModule<Window>();
        auto& window = mainWindow.getWindow();

        mainWindow.pushGLStates();
        _canvas.create(window.getSize().x, window.getSize().y);
        mainWindow.popGLStates();

        init();
    }
    void GUITest::init()
    {
        auto& gui = getModule<GUI>();

        /*
        gui.addAsset<sf::Texture>("Assets/Test/NinePatchTest.png");
        gui.addAsset<sf::Texture>("Assets/Test/Scroller.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBar.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBound.png");
        gui.addAsset<sf::Texture>("Assets/Test/NinePatchTest.png");
        */

        gui.addAsset<sf::Texture>("Assets/GUITest/Blue.png");
        gui.addAsset<sf::Texture>("Assets/GUITest/Red.png");

        _widget = gui.addInterface();
        //_widget->setSize(100, 100);
        _widget->setSizingWidthToHeightProportion(1.f);
        _widget->setWidthToHeightProp(16.f/9.f);



        
        auto* list = new ListWidget;
        list->makeHorizontal();
        list->setPosition(400.f, 400.f);
        list->setPadding(20.f, 10.f);
        list->setBetweenPadding(30.f);
        _widget->add(list);
        
        
        std::vector<ImageWidget*> _ws(4, nullptr);
        size_t i = 0;
        for(auto* w : _ws)
        {
            w = new ImageWidget;
            int addon = i * 50;
            list->add(w);
            if(i == 1)
            {
            w->setSize(50, 50 + addon);
            w->setPosition(addon, 0);

            }

            w->setCallback(Widget::CallbackType::onHoverOut, [](auto* w){
                w->setColor({255, 255, 255}, 1.f);
            });
            w->setCallback(Widget::CallbackType::onHoverIn, [](auto* w){
                w->setColor({0, 0, 0}, 1.f);
            });
            w->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png"));

            i++;
        }
        auto* testRes = new ImageWidget;

        _widget->add(testRes);

        testRes->setPropSize(0.2f, 0.2f);
        testRes->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png"));
        testRes->setPropPosition(0.f, 0.f);

        
        _imageWidget = new ImageWidget;

        _widget->add(_imageWidget);
        _imageWidget->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png"));
        _imageWidget->setPropSize(0.2f, 0.2f);
        _imageWidget->setPropPosition(1.f, 1.f);

        _imageWidget->setTexture(nullptr);

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
    }
    void GUITest::render()
    {
        auto& mainWindow = getModule<Window>();

        mainWindow.pushGLStates();

       _canvas.clear(sf::Color::Transparent);
        _canvas.display();

        mainWindow.getWindow().draw(sf::Sprite(_canvas.getTexture()));

        mainWindow.popGLStates();
    }
}