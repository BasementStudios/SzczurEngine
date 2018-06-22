#pragma once
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"


namespace rat
{
    class ImageWidget;
    class QuestJournal : public Module<GUI>
    {
        public:
            QuestJournal();
            ~QuestJournal();


        private:
            InterfaceWidget* _interface{nullptr};
            ImageWidget *_widget{nullptr};
    };
}