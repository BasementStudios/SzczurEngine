#include "QuestJournal.hpp"
namespace rat
{
    QuestJournal::QuestJournal()
    {
        LOG_INFO("QuestJournal module initializing...");

        it = _quests.begin();

        auto& mainWindow = getModule<Window>();
        auto& window = mainWindow.getWindow();
        mainWindow.pushGLStates();
        mainWindow.popGLStates();
        auto& gui = getModule<GUI>();

        gui.addAsset<sf::Texture>("Assets/GUITest/Blue.png");
        gui.addAsset<sf::Font>("Assets/GUITest/testfont.otf");
        gui.addAsset<sf::Texture>("Assets/Test/Scroller.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBar.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBound.png");


        _font = gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf");
        _ButtonWidget = new ImageWidget;
        _ButtonWidget->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png"));
        _ButtonWidget->setCallback(Widget::CallbackType::onPress,[this](auto){
            this->turnOFF();
        });

        _doneSwitch = new ImageWidget;
        _doneSwitch->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png"));
        _doneSwitch->setPropPosition(0.01f,0.47f);
        _doneSwitch->setCallback(Widget::CallbackType::onPress, [this](auto){
                this->displayDoneList();
        });

        _switch = new ImageWidget;
        _switch->setTexture(gui.getAsset<sf::Texture>("Assets/GUITest/Blue.png"));
        _switch->setPropPosition(0.01f,0.6f);
        _switch->setCallback(Widget::CallbackType::onPress, [this](auto){
                this->displayNormalList();
        });



        _interface = gui.addInterface();
        
        _interface->add(_ButtonWidget);
        _interface->add(_switch);
        _interface->add(_doneSwitch);

        _scroller = new ScrollAreaWidget;
        _list = new ListWidget;
        _doneScroller = new ScrollAreaWidget;
        _doneList = new ListWidget;
        _scroller->add(_list);
        _doneScroller->add(_doneList);

        _list->setBetweenPadding(20.f);
        _list->setPropSize(0.6f, 1.f);
        _doneList->setBetweenPadding(20.f);
        _doneList->setPropSize(0.6f,1.f);

        _scroller->deactivate();
        _scroller->invisible();
        
        _interface->add(_scroller);
        _interface->add(_doneScroller);

        _interface->setSizingWidthToHeightProportion(1.f);

        _fileLoader = std::make_shared<journal::FileLoader>(); 
        _stepManager = std::make_shared<journal::StepsManager>(gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf"),_interface);
      
        _descriptionManager = std::make_shared<journal::DescriptionManager>(gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf"),_interface);
        
        _questName = std::make_shared<journal::QuestName>(gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf"),_interface);


        _stepManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _descriptionManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _scroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _scroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _scroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _scroller->setPropSize(0.7f, 0.5f);
        _scroller->setPropPosition(0.1f, 0.85f);

        _doneScroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _doneScroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _doneScroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _doneScroller->setPropSize(0.7f, 0.5f);
        _doneScroller->setPropPosition(0.1f, 0.85f);

/*
        addQuest(0);
        addStep(2);
        addStep(3);
        addStep(4);
        addStep(5);
        addDescription(1);
        addDescription(2);

        addQuest(7);
        addStep(9);
        addStep(10);
        addStep(11);
        addDescription(3);
        addDescription(4);
        addDescription(5);

        addQuest(13);
        addStep(15);
        addStep(16);

        addQuest(18);
        addStep(20);
        addStep(21);

        finishQuest("MISJA 2");
        */

        LOG_INFO("QuestJournal module initialized");
    }

    QuestJournal::~QuestJournal()
    {
        LOG_INFO("QuestJournal module destructed");
    }

    void QuestJournal::addQuest(const unsigned int &i)
    {
        std::shared_ptr<journal::Quest> quest= std::make_shared<journal::Quest>(_fileLoader);
        quest->setQuestName(i);
        _quests.push_back(quest);
        it = _quests.end()-1;
        
        TextWidget* widget;
        widget = new TextWidget;
        widget->setString(quest->getQuestName());
        widget->setCharacterSize(30);
        widget->setColor(sf::Color::White);
        widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                refresh(widget->getString());
        }
        );
        widget->setFont(getModule<GUI>().getAsset<sf::Font>("Assets/GUITest/testfont.otf"));
        _list->add(widget);
        
        _stepManager->setQuest(quest);
        _descriptionManager->setQuest(quest);
        _questName->setQuest(quest);

    }

    void QuestJournal::addStep(const unsigned int &i)
    {
        (*it)->nextStep(i);
    }

    void QuestJournal::addDescription(const unsigned int &descriptionNumber)
    {
        (*it)->addQuestDescription(descriptionNumber);
    }

    void QuestJournal::moveIterator(std::string questName)
    {
        for(it = _quests.begin();it!=_quests.end();it++)
        {
            if((*it)->getQuestName() == questName)    
                return;
        }
    }
    void QuestJournal::refresh(std::string questName)
    {
        for(auto i : _quests)
        {
            if(i->getQuestName()==questName)
            {
               
                    _stepManager->setQuest(i);
                    _descriptionManager->setQuest(i);
                    _questName->setQuest(i);
                    return;
            }
        }
        for(auto i : _doneQuests)
        {
            if(i->getQuestName()==questName)
            {
                    _stepManager->setQuest(i);
                    _descriptionManager->setQuest(i);
                    _questName->setQuest(i);
                    return;
            }
        }
     
    }

    void QuestJournal::turnOFF()
    {
        _interface->invisible();
        _interface->deactivate();
    }

    void QuestJournal::turnON()
    {
        _interface->activate();
        _interface->visible();
    }

    void QuestJournal::finishQuest(std::string name)
    {
        for(auto i = _quests.begin(); i != _quests.end();i++)
        {
            if((*i)->getQuestName()==name)
            {
                TextWidget* widget;


                _doneQuests.push_back(*i);
                
                widget = new TextWidget;
                widget->setString((*i)->getQuestName());
                widget->setFont(_font);
                widget->setCharacterSize(25);
                widget->setColor(sf::Color::White);
                widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                    refresh(widget->getString());
                });
                _doneList->add(widget);


                _quests.erase(i);
                _list->clear();
                
                for(auto k = _quests.begin();k != _quests.end();k++)
                {
                    widget = new TextWidget;
                    widget->setString((*k)->getQuestName());
                    widget->setFont(_font);
                    widget->setCharacterSize(25);
                    widget->setColor(sf::Color::White);
                    widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                        refresh(widget->getString());
                    });
                    _list->add(widget);
                }
                return;
            }
        }
    }

    void QuestJournal::displayNormalList()
    {
        _doneScroller->deactivate();
        _doneScroller->invisible();

        _scroller->activate();
        _scroller->visible();
    }

    void QuestJournal::displayDoneList()
    {
        _scroller->deactivate();
        _scroller->invisible();

        _doneScroller->activate();
        _doneScroller->visible();
    }
}