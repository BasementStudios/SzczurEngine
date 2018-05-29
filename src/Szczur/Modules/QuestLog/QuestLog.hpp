#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include <memory>

#include "Quest/Quest.hpp"
#include "GUI/QuestGUI.hpp"


namespace rat 
{
    class Widget;
    class QuestLog : public Module<Input, Window, Script, GUI> 
    {
    public:
        QuestLog();
        void init();

        void update(float dt);
        void render();

        void setTitle(const std::string& name);

        void _resetWidget(Widget*);

        QuestGUI* getGUI();
    private:
        std::unique_ptr<Quest> _quest;
        Widget* _widget{nullptr};
        std::unique_ptr<QuestGUI> _title;
    };

}