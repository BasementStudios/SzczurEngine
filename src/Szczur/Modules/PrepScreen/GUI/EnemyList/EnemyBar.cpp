#include "EnemyBar.hpp"
#include "EnemyArea.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "../../Enemy/Enemy.hpp"


namespace rat
{
    EnemyBar::EnemyBar(EnemyArea& parentArea)
    :
    _parentArea(parentArea)
    {
        _getBase()->setPropPosition(0.5f, 0.f);

        _border = new WindowWidget;
        _icon = new ImageWidget;
        _name = new TextWidget;
        _hp = new TextWidget;

        const auto prSize = sf::Vector2f{0.312037037037037f, 0.1148148148148f};


        _addWidget(_border);
        _border->setMainPatchPropSize({prSize.y, prSize.y});
        _border->setPropSize(prSize);
        _border->makeChildrenPenetrable();
        //_border->makeChildrenUnresizable();

        _filter = new WindowWidget;
        _filter->setPropSize(prSize);
        _filter->setMainPatchPropSize(prSize);
        _border->add(_filter);
        _filter->invisible();

        _border->setCallback(Widget::CallbackType::onHoverIn, [this](Widget*){
            _onHoverIn();
        });
        _border->setCallback(Widget::CallbackType::onHoverOut, [this](Widget*){
            _onHoverOut();
        });

        auto* mainList = new ListWidget;
        _border->add(mainList);

        auto* infoList = new ListWidget;
        mainList->add(infoList);
        infoList->makeHorizontal();

        const sf::Vector2f prPad = {0.01f, 0.01f};

        auto* iconParent = new Widget;
        iconParent->setPropSize(prSize.y, prSize.y);
        //iconParent->setPropPadding(prPad); 
        infoList->add(iconParent);
        
        const float iconPropDim = (prSize.y - prPad.x) * 0.9f;
        _icon->setPropSize(iconPropDim, iconPropDim);
        _icon->setPropPosition(0.5f, 0.5f);
        iconParent->add(_icon);

        auto* titles = new Widget;
        titles->setPropSize(prSize.x - prSize.y, prSize.y);
        titles->setPropPadding(0.01f, 0.01f);
        infoList->add(titles);

        titles->add(_name);
        _name->setCharacterSize(20);
        _name->setPropPosition(1.f, 0.f);

        titles->add(_hp);
        _hp->setCharacterPropSize(0.025f);
        _hp->setPropPosition(1.f, 1.f);

        _descParent = new Widget;
        _descParent->setPropSize(prSize.x, 0.f);
        _descParent->setPropPadding(0.f, 0.01f);
        _descParent->fullyDeactivate();
        //_descParent->makeChildrenUnresizable();
        mainList->add(_descParent);

        _description = new TextAreaWidget;
        _descParent->add(_description);
        _description->setCharacterPropSize(0.02f);
        _description->setPropPosition(0.5f, 0.5f);
        _description->setAlign(TextAreaWidget::Align::Center);
        _description->setPropSize(prSize.x * 0.9f, 0.f);

        auto* base = _getBase();
        base->makeChildrenPenetrable();
        base->setCallback(Widget::CallbackType::onPress, [this](auto){
            if(_descParent->isFullyDeactivated()) _descParent->fullyActivate();
            else _descParent->fullyDeactivate();
        });
    }

    void EnemyBar::setEnemy(const Enemy* enemy)
    {
        _icon->setTexture(enemy->getTexture());
        _hp->setString("HP: " + std::to_string(enemy->getMaxHP()));
        _name->setString(enemy->getName());
        _enemy = enemy;
        _description->setString(enemy->getDescription());
    }

    void EnemyBar::initAssetsViaGUI(GUI& gui)
    {
        auto* barTex = gui.getAsset<sf::Texture>("Assets/PrepScreen/SkillBack.png");
        _border->setTexture(barTex, 30);
        auto* font = gui.getAsset<sf::Font>("Assets/fonts/NotoSerif-Regular.ttf");
        _name->setFont(font);
        _hp->setFont(font);
        _description->setFont(font);
        _filter->setTexture(gui.getTexture("Assets/PrepScreen/SkillBarFilter.png"), 280, 100);
    }

    void EnemyBar::_onHoverIn()
    {
        //_filter->visible();
        if(!_enemy) return;    
        //_parentArea.setEnemyInfo(_enemy, {0.f, getPosition().y});
    }
    void EnemyBar::_onHoverOut()
    {
        //_filter->invisible();
        if(!_enemy) return;
        //if(_parentArea.isEnemyInInfo(_enemy) || true) _parentArea.deactivateInfo();
    }
}
