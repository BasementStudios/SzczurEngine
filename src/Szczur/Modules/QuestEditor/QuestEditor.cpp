#include "QuestEditor.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    QuestEditor::QuestEditor()
    {
        LOG_INFO(": Initializing Quest Editor module...");
        init();
        LOG_INFO(": Module Quest Editor initialized");
    }
    QuestEditor::~QuestEditor()
    {
        LOG_INFO(" : Module Quest Editor destructed");
    }
    void QuestEditor::init()
    {
        _editor = std::make_unique<QuestNodeEditor>();
    }

    void QuestEditor::update(float dt)
    {
        _editor->update();
    }
}
