#include "QuestJournal.hpp"
namespace rat
{
    QuestJournal::QuestJournal()
    {
        LOG_INFO("QuestJournal module initialized");

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
        _quest = std::make_shared<Quest>(_fileLoader);
        _questName = std::make_shared<QuestName>(gui.getAsset<sf::Font>("Assets/GUITest/testfont.otf"),_interface);

        _quest->nextStep(1);
        _quest->nextStep(2);

        _quest->setQuestName(0);

        _stepManager->setQuest(_quest);
        _questName->setQuest(_quest);
        _descriptionManager->setQuest(_quest);
        _stepManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _descriptionManager->setScrollTextures(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"),
                                    gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
        _quest->nextStep(2);
        _quest->nextStep(3);
        _stepManager->refresh();
        _quest->addQuestDescription(1,5);
        _descriptionManager->refresh();
    }

    QuestJournal::~QuestJournal()
    {
        LOG_INFO("QuestJournal module destructed");
    }

}