#pragma once
#include "Szczur/Modules/GUI/GUI.hpp"


#include "StepsManager.hpp"
#include "DescriptionManager.hpp"
#include "QuestName.hpp"

namespace rat
{
    class QuestJournal : public Module<Window,GUI>
    {
        public:
            QuestJournal();
            ~QuestJournal();

        private:
            InterfaceWidget* _interface{nullptr};
            std::shared_ptr<FileLoader> _fileLoader;
            std::shared_ptr<Quest> _quest;

            std::shared_ptr<StepsManager> _stepManager;
            std::shared_ptr<DescriptionManager> _descriptionManager;
            std::shared_ptr<QuestName> _questName;
          
    };
}


