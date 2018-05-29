#pragma once

#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>
#include <memory>

namespace rat
{
    class QuestTitle : public BaseBar
    {
    public:
        QuestTitle();

        void setFont(sf::Font* font);

        void setReq(const std::string& title);
        void setReq(const std::string& title, int current, int maximum);
        void setReq(const std::string& title, bool req);

        virtual void setSize(const sf::Vector2u& size) override;
        virtual void setSize(unsigned int width, unsigned int height) override;

    private:
        std::vector<std::unique_ptr<QuestTitle>> _subTexts;
        
        size_t _localIndex = 0;

        TextWidget* _title{nullptr};
        TextWidget* _req{nullptr};

        sf::Font* _font{nullptr};

        static constexpr float _padding = 3.f; 
        static constexpr float _nextChildShiftToRight = 5.f;
        static constexpr unsigned int reqWidth = 20u;
    };
}