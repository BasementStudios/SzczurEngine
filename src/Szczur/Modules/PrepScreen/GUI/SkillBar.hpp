#pragma once

#include "Szczur/Modules/GUI/Widget.hpp"
#include "../Skill.hpp"

namespace rat
{
    class ImageWidget;
    class TextWidget;
    class Widget;

    class SkillBar
    {
    public:
        SkillBar(PPContainer& source);
        void setSkill(Skill* skill);
        void setParent(Widget* parent);
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& pos);
        void setBarTexture(sf::Texture* mainTexture, sf::Texture* lockTexture); 

        bool isBought() const{
            return _isBought;
        }

    private:
        Skill* _skill{nullptr};
        PPContainer& _source;

        bool _isBought{false};
        bool _isActive{true};

        //std::vector<ImageWidget*> _coloredPPs;
        ImageWidget* _bar{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _nameText{nullptr};

        sf::Texture* _textureBar{nullptr};
        sf::Texture* _textureLocked{nullptr};
    };
}