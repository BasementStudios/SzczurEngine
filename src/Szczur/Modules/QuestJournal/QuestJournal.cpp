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
        gui.addAsset<sf::Font>("Assets/GUITest/arial.ttf");
        gui.addAsset<sf::Texture>("Assets/Test/Scroller.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBar.png");
        gui.addAsset<sf::Texture>("Assets/Test/ScrollerBound.png");
        gui.addAsset<sf::Texture>("Assets/Test/ButtonTest.png");
        gui.addTexture("Assets/Test/NinePatchTest.png");
        gui.addFont("Assets/GUITest/testfont.otf");

        _font = gui.getAsset<sf::Font>("Assets/GUITest/arial.ttf");
        _ButtonWidget = new ImageWidget;
        _ButtonWidget->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ButtonTest.png"));
        _ButtonWidget->setCallback(Widget::CallbackType::onPress,[this](auto){
            this->turnOFF();
        });
        _ButtonWidget->setPropSize(0.07,0.07);

        _switch = new WindowWidget;
        _switch->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ButtonTest.png"),60);
        _switch->setPropPosition(0.01f,0.47f);
        _switch->setCallback(Widget::CallbackType::onPress, [this](auto){
                this->displayNormalList();   
        });
        _switch->setPropSize(0.2,0.1);

        _doneSwitch = new WindowWidget;
        _doneSwitch->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ButtonTest.png"),60);
        _doneSwitch->setPropPosition(0.01f,0.6f);
        _doneSwitch->setCallback(Widget::CallbackType::onPress, [this](auto){
                this->displayDoneList();
        });
         _doneSwitch->setPropSize(0.2,0.1);


        _interface = gui.addInterface();

        _interface->add(_ButtonWidget);
        _interface->add(_switch);
        _interface->add(_doneSwitch);

        for(int k=0;k<4;k++)
        {
            _ninePatchWidget[k] = new WindowWidget;
            _ninePatchWidget[k]->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"),190);
        }

        _ninePatchWidget[0]->setPropPosition(0.27f,0.07f);
        _ninePatchWidget[0]->setPropSize(0.46f,0.35f);
        _interface->add(_ninePatchWidget[0]);


        _ninePatchWidget[2]->setPropPosition(0.97f, 0.88f);
        _ninePatchWidget[2]->setPropSize(0.75f, 0.9f);
        _interface->add(_ninePatchWidget[2]);

        _ninePatchWidget[3]->setPropPosition(0.25f, 0.9f);
        _ninePatchWidget[3]->setPropSize(0.55f, 0.55f);
        _interface->add(_ninePatchWidget[3]);

        _ninePatchWidget[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ButtonTest.png"),60);
        _ninePatchWidget[1]->setPropPosition(0.847f,0.04);
        _ninePatchWidget[1]->setPropSize(0.45f,0.1f);
        _interface->add(_ninePatchWidget[1]);

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
        _stepManager = std::make_shared<journal::StepsManager>(gui.getAsset<sf::Font>("Assets/GUITest/arial.ttf"),_ninePatchWidget[0]);
      
        _descriptionManager = std::make_shared<journal::DescriptionManager>(gui.getAsset<sf::Font>("Assets/GUITest/arial.ttf"),_ninePatchWidget[2]);
        
        _questName = std::make_shared<journal::QuestName>(gui.getAsset<sf::Font>("Assets/GUITest/arial.ttf"),_ninePatchWidget[1]);


        _stepManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _descriptionManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _scroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _scroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _scroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _scroller->setPropSize(0.47f, 0.5f);
        _scroller->setPropPosition(0.26f, 0.85f);

        _doneScroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _doneScroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _doneScroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _doneScroller->setPropSize(0.47f, 0.5f);
        _doneScroller->setPropPosition(0.26f, 0.85f);

        _done = new TextWidget;
        _done->setFont(_font);
        _done->setString("Wykonane");
        _done->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
        _done->setCharacterSize(20);
        _done->setCallback(Widget::CallbackType::onPress, [this](auto){
                this->displayDoneList();
        });
        _done->setPropPosition(0.42f,0.57f);
        
        _doneSwitch->add(_done);

        _actual = new TextWidget;
        _actual->setFont(_font);
        _actual->setString("Obecne");
        _actual->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
        _actual->setCharacterSize(20);
        _actual->setCallback(Widget::CallbackType::onPress, [this](auto){
                this->displayNormalList();   
        });
        _actual->setPropPosition(0.42f,0.57f);
        
        _switch->add(_actual);

        addQuest(0);
        addStep(0);
        addStep(1);
        addStep(2);
        addStep(3);
        addStep(4);
        addStep(5);
        addDescription(0);
        addDescription(1);
        addDescription(2);
        addDescription(3);


        addQuest(1);
        addStep(0);
        addStep(1);
        addStep(2);
        addDescription(0);
        addDescription(1);

        addQuest(2);
        addStep(0);
        addDescription(0);

        addQuest(3);
        addStep(0);
        addStep(1);
        addDescription(0);
    
        finishQuest(2);


        loadFromJson(getSaveJson());
        //moveIterator(1);
      //  addStep(0);
        
        
        displayNormalList();

        LOG_INFO("QuestJournal module initialized");
    }

    QuestJournal::~QuestJournal()
    {
        LOG_INFO("QuestJournal module destructed");
    }

    void QuestJournal::addQuest(unsigned int questID)
    {
        std::shared_ptr<journal::Quest> quest= std::make_shared<journal::Quest>(_fileLoader, questID);
        quest->setQuestName(questID); ///
        quest->formatName();
        _quests.push_back(quest);
        
        it = _quests.end()-1;
        
        TextWidget* widget;
        widget = new TextWidget;
        widget->setString(quest->getQuestName());
        widget->setCharacterSize(30);
        widget->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
        widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){

                if(lastPressedTime+300>_journalClock.getElapsedTime().asMilliseconds())
                {
                    setFollowing(widget->getString());
                }
                lastPressedTime = _journalClock.getElapsedTime().asMilliseconds();
                refresh(widget->getString());
        }
        );
        widget->setFont(getModule<GUI>().getAsset<sf::Font>("Assets/GUITest/testfont.otf"));
        _list->add(widget);
        _normalTextWidgets.push_back(widget);
        
       // _stepManager->setQuest(quest);
        //_descriptionManager->setQuest(quest);
        //_questName->setQuest(quest);

    }
    

    void QuestJournal::addStep(unsigned int i)
    {
        (*it)->nextStep(i);
    }

    void QuestJournal::addDescription(unsigned int descriptionNumber)
    {
        (*it)->addQuestDescription(descriptionNumber);
    }

    void QuestJournal::moveIterator(const std::string& questName)
    {
        for(it = _quests.begin();it!=_quests.end();it++)
        {
            if((*it)->getQuestName() == questName)    
                return;
        }
    }

    void QuestJournal::moveIterator(unsigned int id)
    {
        std::string questName = _fileLoader->getQuestName(id);
        while(questName[0]!= ' ')
        {
            questName.erase(0,1);
        }
        while(questName[0] == ' ')
        {
            questName.erase(0,1);
        }  

        for(it = _quests.begin();it!=_quests.end();it++)
        {
            if((*it)->getQuestName() == questName)    
                return;
        }
    }
    void QuestJournal::refresh(const std::string& questName)
    {

        for(auto i :_doneTextWidgets)
            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));

        std::vector<std::shared_ptr<journal::Quest> >::iterator k = _quests.begin();
        for(auto i :_normalTextWidgets)
        {
            if((*k)->isFollowing())
            { 
                i->setColor(sf::Color(sf::Color(0, 153, 0 ,255)));
            }
            else    
                i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
            k++;
        } 
        for(auto i :_doneTextWidgets)
            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));

        k = _quests.begin();
        for(auto i : _normalTextWidgets)
        {
            if(i->getString() == questName)
            {
                if((*k)->isFollowing())
                     i->setColor(sf::Color(0, 255, 0,255));
                else
                    i->setColor(sf::Color::White);

                _stepManager->setQuest(*k);
                _descriptionManager->setQuest(*k);
                _questName->setQuest(*k);

       
                return;          
            }
            k++;
        }

        k = _doneQuests.begin();
        for(auto i : _doneTextWidgets)
        {
            if(i->getString() == questName)
            {
                i->setColor(sf::Color::White);
 
                _stepManager->setQuest(*k);
                _descriptionManager->setQuest(*k);
                _questName->setQuest(*k);
                return;
            }
            k++;
        }
       
    }

    void QuestJournal::refresh(unsigned int id)
    {
        std::string questName = _fileLoader->getQuestName(id);
        while(questName[0]!= ' ')
        {
            questName.erase(0,1);
        }
        while(questName[0] == ' ')
        {
            questName.erase(0,1);
        }  
        std::vector<std::shared_ptr<journal::Quest> >::iterator k = _quests.begin();
        for(auto i :_normalTextWidgets)
        {
            if((*k)->isFollowing())
                i->setColor(sf::Color(sf::Color(0, 153, 0 ,255)));

            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
            k++;
        } 
        for(auto i :_doneTextWidgets)
            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));

        k = _quests.begin();
        for(auto i : _normalTextWidgets)
        {
            if(i->getString() == questName)
            {
                i->setColor(sf::Color::White);

                _stepManager->setQuest(*k);
                _descriptionManager->setQuest(*k);
                _questName->setQuest(*k);
                return;          
            }
            k++;
        }

        k = _doneQuests.begin();
        for(auto i : _doneTextWidgets)
        {
            if(i->getString() == questName)
            {
                i->setColor(sf::Color::White);
 
                _stepManager->setQuest(*k);
                _descriptionManager->setQuest(*k);
                _questName->setQuest(*k);
                return;
            }
            k++;
        }
    }

    void QuestJournal::turnOFF()
    {
        _interface->invisible();
        _interface->deactivate();
        _isRunning = false;
    }

    void QuestJournal::turnON()
    {
        _interface->activate();
        _interface->visible();
        _isRunning = true;
    }

    void QuestJournal::finishQuest(const std::string& name)
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
                widget->setColor(sf::Color(135, 89, 247 ,255));
                widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                    refresh(widget->getString());
                });
                _doneList->add(widget);
                _doneTextWidgets.push_back(widget);

                _quests.erase(i);
                _list->clear();
                _normalTextWidgets.clear();
                
                for(auto k = _quests.begin();k != _quests.end();k++)
                {
                    widget = new TextWidget;
                    widget->setString((*k)->getQuestName());
                    widget->setFont(_font);
                    widget->setCharacterSize(25);
                    widget->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
                    widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                        if(lastPressedTime+300>_journalClock.getElapsedTime().asMilliseconds())
                        {
                            setFollowing(widget->getString());
                        }
                        lastPressedTime = _journalClock.getElapsedTime().asMilliseconds();
                        refresh(widget->getString());
                    });
                    _list->add(widget);
                    _normalTextWidgets.push_back(widget);
                }
                return;
            }
        }
    }

    void QuestJournal::finishQuest(unsigned int id)
    {
        std::string name = _fileLoader->getQuestName(id);
        while(name[0]!= ' ')
        {
            name.erase(0,1);
        }
        while(name[0] == ' ')
        {
            name.erase(0,1);
        }   

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
                widget->setColor(sf::Color(135, 89, 247 ,255));
                widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                    refresh(widget->getString());
                });
                _doneList->add(widget);
                _doneTextWidgets.push_back(widget);

                _quests.erase(i);
                _list->clear();
                _normalTextWidgets.clear();
                
                for(auto k = _quests.begin();k != _quests.end();k++)
                {
                    widget = new TextWidget;
                    widget->setString((*k)->getQuestName());
                    widget->setFont(_font);
                    widget->setCharacterSize(25);
                    widget->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
                    widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                        if(lastPressedTime+300>_journalClock.getElapsedTime().asMilliseconds())
                        {
                            setFollowing(widget->getString());
                        }
                        lastPressedTime = _journalClock.getElapsedTime().asMilliseconds();
                        refresh(widget->getString());
                    });
                    _list->add(widget);
                    _normalTextWidgets.push_back(widget);
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

        _questName->clear();
        _descriptionManager->clear();
        _stepManager->clear();

        for(auto i :_normalTextWidgets)
            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
        for(auto i :_doneTextWidgets)
            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));

        _actual->setColor(sf::Color::White);
        std::vector<std::shared_ptr<journal::Quest> >::iterator k = _quests.begin();
        for(auto i :_normalTextWidgets)
        {
            if((*k)->isFollowing())
                i->setColor(sf::Color(sf::Color(0, 153, 0 ,255)));

            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
            k++;
        } 

        
        _done->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
    }

    void QuestJournal::displayDoneList()
    {
        _scroller->deactivate();
        _scroller->invisible();

        _doneScroller->activate();
        _doneScroller->visible();

        _questName->clear();
        _descriptionManager->clear();
        _stepManager->clear();

        for(auto i :_normalTextWidgets)
            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
        for(auto i :_doneTextWidgets)
            i->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));

        _done->setColor(sf::Color::White);
        _actual->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
    }

    void QuestJournal::update()
    {
        if(!_isRunning)
        {   
            if(getModule<Input>().getManager().isPressed(Keyboard::J))
            {
                turnON();
            }
        }
    }

    nlohmann::json& QuestJournal::getSaveJson()
    {
        int k=0;
        for(auto i = _quests.begin();i!=_quests.end();i++)
        {
            _save["in_progress"]["Q"+std::to_string(k)]["ID"] = std::to_string((*i)->getID());
            _save["in_progress"]["Q"+std::to_string(k)]["name"] = (*i)->getQuestName();
            _save["in_progress"]["Q"+std::to_string(k)]["steps"] = (*i)->getSteps();
            _save["in_progress"]["Q"+std::to_string(k)]["descriptions"] = (*i)->getDescription();
            k++;
        }
        k=0;
        for(auto i = _doneQuests.begin();i!=_doneQuests.end();i++)
        {
            _save["done"]["Q"+std::to_string(k)]["ID"] = std::to_string((*i)->getID());
            _save["done"]["Q"+std::to_string(k)]["name"] = (*i)->getQuestName();
            _save["done"]["Q"+std::to_string(k)]["steps"] = (*i)->getSteps();
            _save["done"]["Q"+std::to_string(k)]["descriptions"] = (*i)->getDescription();
            k++;
        }
        return _save;
    }
    
    void QuestJournal::loadFromJson(nlohmann::json& json)
    {
        std::shared_ptr<journal::Quest> quest;
        int k=0;

        _quests.clear();
        _list->clear();
        _normalTextWidgets.clear();

        _doneQuests.clear();
        _doneList->clear();
        _doneTextWidgets.clear();
        
        while(json["in_progress"]["Q"+std::to_string(k)].empty() == false)
        {
            std::string n = json["in_progress"]["Q"+std::to_string(k)]["ID"];
            quest = std::make_shared<journal::Quest>(_fileLoader,atoi(n.c_str()) );
            n = json["in_progress"]["Q"+std::to_string(k)]["name"];
            quest->setQuestName(n);
            quest->addQuestSaveDescription(json["in_progress"]["Q"+std::to_string(k)]["descriptions"]);
            quest->addSaveSteps(json["in_progress"]["Q"+std::to_string(k)]["steps"]);

            _quests.push_back(quest);

            it = _quests.end()-1;
        
            TextWidget* widget;
            widget = new TextWidget;
            widget->setString(quest->getQuestName());
            widget->setCharacterSize(30);
            widget->setColor(sf::Color(sf::Color(135, 89, 247 ,255)));
            widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                if(lastPressedTime+300>_journalClock.getElapsedTime().asMilliseconds())
                {
                    setFollowing(widget->getString());
                }
                lastPressedTime = _journalClock.getElapsedTime().asMilliseconds();
                    refresh(widget->getString());
            }
            );
            widget->setFont(getModule<GUI>().getAsset<sf::Font>("Assets/GUITest/testfont.otf"));
            _list->add(widget);
            _normalTextWidgets.push_back(widget);
            k++;
        }
        k=0;
        while(json["done"]["Q"+std::to_string(k)].empty() == false)
        {
            std::string n = json["done"]["Q"+std::to_string(k)]["ID"];
            quest = std::make_shared<journal::Quest>(_fileLoader,atoi(n.c_str()) );
            n = json["done"]["Q"+std::to_string(k)]["name"];
            quest->setQuestName(n);
            quest->addQuestSaveDescription(json["done"]["Q"+std::to_string(k)]["descriptions"]);
            quest->addSaveSteps(json["done"]["Q"+std::to_string(k)]["steps"]);

            _quests.push_back(quest);

            it = _quests.end()-1;
        
            TextWidget* widget;

            _doneQuests.push_back(quest);
                
            widget = new TextWidget;
            widget->setString(quest->getQuestName());
            widget->setFont(_font);

            widget->setCharacterSize(25);
            widget->setColor(sf::Color(135, 89, 247 ,255));
            widget->setCallback(Widget::CallbackType::onPress,[this,widget](auto){
                refresh(widget->getString());
            });
            _doneList->add(widget);
            _doneTextWidgets.push_back(widget);
            k++;
        }
    }
    void QuestJournal::setFollowing(std::string name)
    {
        std::vector<std::shared_ptr<journal::Quest> >::iterator k = _quests.begin();
        for(auto i = _normalTextWidgets.begin(); i!= _normalTextWidgets.end() ; i++)
        {
            if((*i)->getString() == name)
            {
                (*i)->setColor(sf::Color(0, 153, 0,255));
                for(auto b = _quests.begin(); b != _quests.end() ; b++)
                {
                    (*b)->setFollowing(false);
                }
                (*k)->setFollowing(true);
            }
            k++;
        }
    }

    std::string QuestJournal::getFollowStep()
    {
        for(auto i : _quests)
        {
            if(i->isFollowing())
                return i->getLastStep();
        }
        return "";
    }
}