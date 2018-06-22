#include "QuestJournal.hpp"
namespace rat
{
    QuestJournal::QuestJournal()
    {

        LOG_INFO("QuestJournal module initialized");
        auto& gui = getModule<GUI>();

        gui.addAsset<sf::Texture>("Assets/GUITest/Blue.png");
        auto* tex = gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png");

        _interface = gui.addInterface();
        _interface->setSizingWidthToHeightProportion(1.f);
        
        _widget = new ImageWidget;
        _interface->add(_widget);
        _widget->setTexture(tex);
        _widget->setPosition(0,0);

        //_widget->setPropSize(0.15f, 0.15f);
      //  if(!tex) LOG_ERROR("KEK");
       
    }

    QuestJournal::~QuestJournal()
    {
        LOG_INFO("QuestJournal module destructed");
    }
}