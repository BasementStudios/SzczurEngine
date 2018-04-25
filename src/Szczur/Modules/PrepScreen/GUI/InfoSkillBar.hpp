#pragma once

#include "BaseBar.hpp"

#include "Szczur/Modules/GUI/TextWidget.hpp"

#include <string>

namespace rat
{
    class InfoSkillArea;

    class InfoSkillBar : public BaseBar
    {
    public:
        InfoSkillBar(InfoSkillArea& parentArea);

        void setName(const std::string& name);
        template<typename T>
        void setValue(T& value);
    private:
        InfoSkillArea& _parentArea;

        ImageWidget
        TextWidget* _value{nullptr};

    };

    template<typename T>
    void InfoSkillBar::setValue(T& value)
    {
        _value->setString(std::to_string(value));
    }
}