#pragma once
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "FileLoader.hpp"
namespace rat
{
    class ImageWidget;
    class QuestJournal : public Module<Window,GUI>
    {
        public:
            QuestJournal();
            ~QuestJournal();

        private:
            InterfaceWidget* _interface{nullptr};
            std::shared_ptr<FileLoader> _fileLoader;
    };
}


