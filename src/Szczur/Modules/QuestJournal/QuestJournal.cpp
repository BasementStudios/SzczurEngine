#include "QuestJournal.hpp"
namespace rat
{
    QuestJournal::QuestJournal()
    {
        LOG_INFO("QuestJournal module initialized");

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
       
        _interface = gui.addInterface();
        _interface->setSizingWidthToHeightProportion(1.f);
        _fileLoader = std::make_shared<FileLoader>();
        _stepManager = std::make_shared<StepsManager>(gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf"),_interface);
        _descriptionManager = std::make_shared<DescriptionManager>(gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf"),_interface);
        
        _questName = std::make_shared<QuestName>(gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf"),_interface);

        _stepManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _descriptionManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
      
    }

    QuestJournal::~QuestJournal()
    {
        LOG_INFO("QuestJournal module destructed");
    }

    void QuestJournal::addQuest(const unsigned int &i)
    {
        std::shared_ptr<Quest> quest= std::make_shared<Quest>(_fileLoader);

        quest->setQuestName(i);

        _quests.push_back(quest);
        it = _quests.end()-1;
    }

    void QuestJournal::addStep(const unsigned int &i)
    {
        (*it)->nextStep(i);
    }

    void QuestJournal::addDescription(const unsigned int &a,const unsigned int &b)
    {
        (*it)->addQuestDescription(a,b);
    }

    void QuestJournal::moveIterator(std::string questName)
    {
        for(it = _quests.begin();it!=_quests.end();it++)
        {
            if((*it)->getQuestName() == questName)    
                return;
        }
    }

}