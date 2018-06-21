#include "InfoBar.hpp"

#include <algorithm>

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"

namespace rat 
{
    InfoBar::InfoBar()
    {
        _name = new TextWidget;
        _window = new WindowWidget;
        _addWidget(_window);

        _window->setPadding(5, 20);
        _window->setSize(300, 120);
        _window->setScale(0.3f, 0.3f);

        _name->setCharacterSize(25);
        _name->setPropOrigin(0.5f, 0.f);
        _name->setPosition(float(_window->getMinimalSize().x) / 2.f - _window->getPadding().x, 0.f);
        _window->add(_name);

        _infoArea = new Widget;
        _infoArea->setPosition(0, 40);
        _window->add(_infoArea);
        _infoArea->setPadding(10.f, 0.f);
    }

    void InfoBar::setName(const std::string& name)
    {
        _name->setString(name);
    }
    const std::string& InfoBar::getName() const
    {
        return _name->getString();
    }
    void InfoBar::setInfo(const std::string& info)
    {
        auto wordBegin = info.begin();
        size_t i = 0;
        while(true)
        {
            auto wordEnd = std::find_if(wordBegin, info.end(), [](const char& c){
                return c == '\n';
            });
            std::string word(wordBegin, wordEnd);

            TextAreaWidget* infoBar{nullptr};

            if(_infos.size() <= i)
            {
                infoBar = new TextAreaWidget;
                _initInfoBar(infoBar);
                _infos.emplace_back(infoBar);
            }
            else
            {
                infoBar = _infos[i];
            }
            infoBar->setString(word);

            if(wordEnd == info.end()) break;
            wordBegin = ++wordEnd;
            i++;
        }
        _calcInfosPos();
    }

    void InfoBar::_initInfoBar(TextAreaWidget* infoBar)
    {  
        infoBar->setCharacterSize(13);
        infoBar->setAreaSize({30, 20});
        infoBar->setColor(sf::Color::White);
        infoBar->setFont(_font);
        _infoArea->add(infoBar);
    }

    void InfoBar::_calcInfosPos()
    {
        float height{0.f};

        for(auto* infoBar : _infos)
        {
            infoBar->setPosition(0.f, height);

            height += (float(infoBar->getSize().y) + 20.f);
        }
    }
    

    void InfoBar::initAssetsViaGUI(GUI& gui)
    {
        _font = gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf");
        _name->setFont(_font);
        _window->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Bar.png"), 6);
    }
}