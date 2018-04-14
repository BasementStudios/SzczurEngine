#pragma once

#include "Szczur/Modules/GUI/Widget.hpp"
#include "../Skill.hpp"

#include "CostBar.hpp"

namespace rat
{
    class ImageWidget;
    class TextWidget;
    class Widget;
    class GUI;

    class GrayPPBar;

    class SkillBar
    {
    public:
        SkillBar(GrayPPBar& source);
        void setSkill(Skill* skill);
        void setParent(Widget* parent);
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& pos);
        void setBarTexture(sf::Texture* mainTexture, sf::Texture* lockTexture);
        const std::string& getIconPath() const;
        void setIconTexture(sf::Texture* icon); 
        void setFont(sf::Font* font);

        void loadAssetsFromGUI(GUI& gui);

        void activate();
        void deactivate();


        bool isBought() const{
            return _isBought;
        }
    private:
        Skill* _skill{nullptr};
        GrayPPBar& _sourceBar;

        bool _isBought{false};
        bool _isActive{true};

        void _onClick();

        std::vector<ImageWidget*> _coloredPPs;
        Widget* _infoBar{nullptr};
        ImageWidget* _bar{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _nameText{nullptr};

        CostBar _costBar;

        sf::Texture* _textureBar{nullptr};
        sf::Texture* _textureLocked{nullptr};

        static sf::Vector2f _size; 
    };
}