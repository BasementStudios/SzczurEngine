#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <string>
#include <vector>

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    class TextWidget; class TextAreaWidget; class WindowWidget;

    class InfoBar : public BaseBar
    {
    public:
        InfoBar();

        void setName(const std::string& name);
        void setInfo(const std::string& info);
        void initAssetsViaGUI(GUI& gui);

        const std::string getName() const;
    private:
        WindowWidget* _window{nullptr};
        TextWidget* _name{nullptr};

        Widget* _infoArea{nullptr};
        std::vector<TextAreaWidget*> _infos;

        sf::Font* _font{nullptr};

        void _initInfoBar(TextAreaWidget* info);
        void _calcInfosPos();
    };

}