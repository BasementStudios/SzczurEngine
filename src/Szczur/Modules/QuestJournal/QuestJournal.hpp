#pragma once
#include <Json/json.hpp>
#include <cstdlib>
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "StepsManager.hpp"
#include "DescriptionManager.hpp"
#include "QuestName.hpp"
#include "Quest.hpp"

namespace rat
{
    class TextWidget; class ImageWidget; class WindowWidget;
    class QuestJournal : public Module<Window, GUI, Input>
    {
        public:
            QuestJournal();
            ~QuestJournal();

            void addQuest(unsigned int questID);
            void addStep(unsigned int i);

            void addDescription(unsigned int descriptionNumber);

            void moveIterator(const std::string& questName);
            void moveIterator(unsigned int id);

            void refresh(const std::string& questName);
            void refresh(unsigned int id);

            void finishQuest(const std::string& name);
            void finishQuest(unsigned int id);

            void displayDoneList();
            void displayNormalList();

            void update();

            void turnON();
            void turnOFF();

            nlohmann::json& getSaveJson();
            void loadFromJson(nlohmann::json& json);

        private:


            nlohmann::json _save;

            bool _isRunning = false;

            TextWidget *_done;
            TextWidget *_actual;

            WindowWidget *_doneSwitch;
            WindowWidget *_switch;
            ImageWidget *_ButtonWidget;

            WindowWidget *_ninePatchWidget[4];

            std::vector<TextWidget*>  _normalTextWidgets;
            std::vector<TextWidget* > _doneTextWidgets;

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

            std::vector<std::string> _texturePath;

            std::vector<std::shared_ptr<journal::Quest> >::iterator it;
            
            sf::Font *_font;
    };
}


