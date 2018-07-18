#pragma once

#include <vector>
#include <map>

#include "Szczur/Modules/PrepScreen/GlyphTypes.hpp"

namespace sf { class Texture; }

namespace rat
{
    class PrepScreen; class Skill; class GUI;
    class ImageWidget; class Widget; class TextWidget;

    class SkillSlot
    {
    public:
        SkillSlot(PrepScreen& prepScreen);
        void setParent(Widget* parent);
        void setSkill(const Skill* skill);
        void removeSkill();
        bool hasSkill() const { return _skill != nullptr; }
        void loadAssetsFromGUI(GUI& gui);
        void recalculateAvailability();
    protected:
        ImageWidget* _background{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _ppCost{nullptr};

    protected:
        std::vector<ImageWidget*> _glyphes;
        std::map<GlyphID, sf::Texture*> _glyphesTexs;

    protected:
        PrepScreen& _prep;
        const Skill* _skill{nullptr};

    private:
        bool _isKnownAsBought{false};
        bool _canBeBought{false};
        void _onClick();
        void _onHoverIn();
        void _onHoverOut();
    };
}