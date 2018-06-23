#pragma once

#include <memory>

#include "Szczur/Utility/Modules/Module.hpp"

#include "Editor/QuestNodeEditor.hpp"

namespace rat
{
    class QuestEditor : public Module<>
    {
    public:


    public:
        QuestEditor();
        ~QuestEditor();
        void init();
        void update(float dt);
    private:
        std::unique_ptr<QuestNodeEditor> _editor;
    };
}