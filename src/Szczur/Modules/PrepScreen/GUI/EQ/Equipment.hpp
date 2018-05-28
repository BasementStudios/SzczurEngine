#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"


namespace rat
{
    class Equipment : public BaseBar
    {
    public:
        Equipment(/* args */);
        void initAssetsViaGUI(GUI& gui);

    private:
        WindowWidget* _border{nullptr};
    };

    
}
