#pragma once

#include <vector>
#include <map>

#include "Szczur/Modules/PrepScreen/GlyphTypes.hpp"

namespace sf { class Texture; }

namespace rat
{
    class PrepScreen; class PrepSkill; class GUI;
    class ImageWidget; class Widget; class TextWidget;

    class SkillSlot
    {
    public:
        SkillSlot(PrepScreen& prepScreen);
        void setParent(Widget* parent);
        void setSkill(const PrepSkill* skill);
        void removeSkill();
        bool hasSkill() const { return _skill != nullptr; }
        void loadAssetsFromGUI(GUI& gui);
        void recalculateAvailability();
    protected:
        Widget* _base{nullptr};
        ImageWidget* _background{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _ppCost{nullptr};

    protected:
        std::vector<ImageWidget*> _glyphes;
        std::map<GlyphID, sf::Texture*> _glyphesTexs;
        size_t _glyphesAmount = 0;

    protected:
        PrepScreen& _prep;
        const PrepSkill* _skill{nullptr};

    private:
        bool _isKnownAsBought{false};
        bool _canBeBought{false};
        bool _justSet{false};
        void _onClick();
        void _onHoverIn();
        void _onHoverOut();

        void _onSale();
        void _onBought();
    };
}