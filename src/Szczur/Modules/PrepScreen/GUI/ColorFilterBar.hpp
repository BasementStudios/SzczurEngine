/*
#pragma once 

#include <vector>
#include <string>

#include "BaseBar.hpp"

namespace rat
{
    class ColorFilterArea;
    class Widget; class ImageWidget;
    class GUI;

    class ColorFilterBar : public BaseBar
    {
    public:
        ColorFilterBar(ColorFilterArea& parentArea);
        void initAssetsViaGUI(GUI& gui);
        void setGlyph(const std::string& color);
        void activate();
        void deactivate();
        void unchoose();
        float getDimension() const;
    private:
        ColorFilterArea& _parentArea;

        float _backPadding{4.f};
        float _dim{25.f};

        ImageWidget* _icon{nullptr};
        ImageWidget* _backIcon{nullptr};
        std::string _color{""};
        bool _isChosen{false};

        void _onClick();
    };
    
}
*/