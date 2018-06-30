#pragma once
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "StepsManager.hpp"
#include "DescriptionManager.hpp"
#include "QuestName.hpp"
#include "Quest.hpp"

namespace rat
{
    class TextWidget; class ImageWidget;
    class QuestJournal : public Module<Window,GUI>
    {
        public:
            QuestJournal();
            ~QuestJournal();

            void addQuest(const unsigned int &i);
            void addStep(const unsigned int &i);

            void addDescription(const unsigned int &descriptionNumber);

            void moveIterator(std::string questName);

            void refresh(std::string questName);

            void finishQuest(std::string name);

            void displayDoneList();
            void displayNormalList();

            void turnON();
            void turnOFF();

        private:

            ImageWidget *_doneSwitch;
            ImageWidget *_switch;
            ImageWidget *_ButtonWidget;

            std::vector<std::shared_ptr<TextWidget> > _widgets;
            ListWidget *_list;
            ListWidget * _doneList;
            ScrollAreaWidget *_scroller;
            ScrollAreaWidget *_doneScroller;

            InterfaceWidget* _interface{nullptr};
            std::shared_ptr<journal::FileLoader> _fileLoader;

            std::shared_ptr<journal::StepsManager> _stepManager;
            std::shared_ptr<journal::DescriptionManager> _descriptionManager;
            std::shared_ptr<journal::QuestName> _questName;

            std::vector<std::shared_ptr<journal::Quest> > _quests;
            
            std::vector<std::shared_ptr<journal::Quest> > _doneQuests;



            std::vector<std::shared_ptr<journal::Quest> >::iterator it;
            
            sf::Font *_font;
    };
}


