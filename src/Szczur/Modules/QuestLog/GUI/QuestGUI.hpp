#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include "QuestInfoBar/QuestTitle.hpp"

#include <vector>
#include <memory>

namespace rat
{
    class QuestGUI : public BaseBar
    {
    public:
        QuestGUI();

        void setQuestName(const std::string& name);
        void resetSubtitles();
        void setTitle(const std::string& name);
        
        QuestTitle* addSubtitle(const std::string& name);
        QuestTitle* addSubtitle(const std::string& name, int current, int maximum);
        QuestTitle* addSubtitle(const std::string& name, bool isSuited);
        void popSubtitle();

        void setFont(sf::Font* font);

    private:
        QuestTitle _title;
        std::vector<std::unique_ptr<QuestTitle>> _subTitles;

        size_t _madeAmount{0};
        size_t _activeAmount{0};
        QuestTitle* _getSubTitle(size_t index);

        sf::Font* _font{nullptr};

        static constexpr float _titlePadding{5.f};
    };
}