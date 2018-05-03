#include "InfoSkillArea.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"

namespace rat
{
/*
    InfoSkillArea::InfoSkillArea()
    {
        _properties = new Widget;
        _addWidget(_properties);

        _initProperty(_name);
        _initProperty(_cooldown);
        _initProperty(_durationTime);
        _initProperty(_areas);

        _initTextArea(_description);
        _initTextArea(_effect);
        /*
        for(int i = 0; i < 4; i++)
        {
            TextWidget* property = new TextWidget;
            _properties.emplace_back
        }
        
    }
    void InfoSkillArea::setSize(float width, float height)
    {

    }



    void InfoSkillArea::_setBaseInfo(SkillBaseInfo& info)
    {
        _name->setString("Name: " + info.name);
        _description->setString(info.description);
        size_t descNum = info.description.size();
        _description->setSize({_textWidth, (descNum/_textWidth) + 1u});
        _cooldown->setString("Cooldown: " + std::to_string(info.cooldown));

    }
    void InfoSkillArea::_recalculate()
    {

    }
    void InfoSkillArea::_initProperty(TextWidget* property)
    {
        delete property;
        property = new TextWidget;
        property->setCharacterSize(_fontSize);
        property->setColor(sf::Color::Red);
        _properties->add(property);
        //_addWidget(property);
    }
    void InfoSkillArea::_initTextArea(TextAreaWidget* text)
    {
        delete text;
        text = new TextAreaWidget;
        text->setCharacterSize(_fontSize);
        text->setColor(sf::Color::Red);
        _textAreas.emplace_back(text);
        _addWidget(text);
    }
*/
}