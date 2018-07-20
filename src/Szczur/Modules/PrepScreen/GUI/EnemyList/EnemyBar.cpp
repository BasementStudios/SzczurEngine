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

        _background = new WindowWidget;
        _icon = new ImageWidget;
        _name = new TextWidget;
        _hp = new TextWidget;

        const auto prSize = sf::Vector2f{0.4f, 0.13f};


        _addWidget(_background);
        _background->setMainPatchPropSize({prSize.y, prSize.y});
        _background->setPropSize(prSize);
        _background->makeChildrenPenetrable();

        _background->setCallback(Widget::CallbackType::onHoverIn, [this](Widget*){
            _onHoverIn();
        });
        _background->setCallback(Widget::CallbackType::onHoverOut, [this](Widget*){
            _onHoverOut();
        });

        auto* mainList = new ListWidget;
        _background->add(mainList);

        _slotBack = new WindowWidget;
        mainList->add(_slotBack);
        _slotBack->setPropSize(prSize);
        _slotBack->setMainPatchPropSize({prSize.y, prSize.y});
        _slotBack->makeChildrenUnresizable();

        auto* infoList = new ListWidget;
        _slotBack->add(infoList);
        infoList->makeHorizontal();

        const sf::Vector2f prPad = {0.01f, 0.01f};

        _iconBack = new WindowWidget;
        _iconBack->setPropSize(prSize.y, prSize.y);
        _iconBack->setMainPatchPropSize({prSize.y, prSize.y}); 
        infoList->add(_iconBack);
        
        const float iconPropDim = (prSize.y);
        _icon->setPropSize(iconPropDim, iconPropDim);
        _icon->setPropPosition(0.5f, 0.5f);
        _iconBack->add(_icon);

        auto* titles = new Widget;
        titles->setPropSize(prSize.x - prSize.y, prSize.y);
        titles->setPropPadding(0.02f, 0.02f);
        infoList->add(titles);

        titles->add(_name);
        _name->setCharacterPropSize(0.025f);
        _name->setPropPosition(1.f, 0.f);
        _name->setOutlinePropThickness(0.06f);
        _name->setOutlineColor(sf::Color::Black);

        titles->add(_hp);
        _hp->setCharacterPropSize(0.025f);
        _hp->setPropPosition(1.f, 1.f);
        _hp->setOutlinePropThickness(0.06f);
        _hp->setOutlineColor(sf::Color::Black);

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
        _description->setOutlinePropThickness(0.06f);
        _description->setOutlineColor(sf::Color::Black);

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
        _background->setTexture(barTex, 30);
        _iconBack->setTexture(barTex, 30);
        _slotBack->setTexture(barTex, 30);
        auto* font = gui.getAsset<sf::Font>("Assets/fonts/NotoSerif-Regular.ttf");
        _name->setFont(font);
        _hp->setFont(font);
        _description->setFont(font);
    }

    void EnemyBar::_onHoverIn()
    {
        if(!_enemy) return;    
    }
    void EnemyBar::_onHoverOut()
    {
        if(!_enemy) return;
    }
}
