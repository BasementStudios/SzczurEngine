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


        gui.addAsset<sf::Texture>("Assets/Test/NinePatchTest.png");
        gui.addAsset<sf::Texture>("Assets/Test/Scroller.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBar.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBound.png");
        gui.addAsset<sf::Texture>("Assets/Test/NinePatchTest.png");


        _widget = gui.addInterface();
        _widget->setSize(100, 100);


        
        auto* list = new ListWidget;
        list->setPosition(700.f, 300.f);
        list->setPadding(20.f, 10.f);
        list->setBetweenPadding(30.f);
        _widget->add(list);

        std::vector<Widget*> _ws(4, new Widget);
        size_t i = 0;
        for(auto* w : _ws)
        {
            std::cout << "Kek\n";
            list->add(w);
            w->setSize(50, 50);
            w->setCallback(Widget::CallbackType::onHover, [i](auto){
                std::cout << "Kekunio" << i << '\n';
            });
            std::cout << "Kek2\n";
            i++;
        }

        auto* scroller = new ScrollAreaWidget;
        _widget->add(scroller);
        scroller->setSize(200, 200);
        scroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        scroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));

        auto* widego = new Widget;
        scroller->add(widego);
        widego->setSize(20.f, 20.f);
        widego->setPosition(20.f, 20.f);
        widego->setCallback(Widget::CallbackType::onPress, [](auto){
            std::cout << "Mekorororor\n";
        });
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