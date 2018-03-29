//#include "Szczur/Utility/Modules/Module.hpp"
#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Utility/Logger.hpp"

//#include "Szczur/Modules/PrepScreen/PPContainer.hpp"
#include "Szczur/Modules/PrepScreen/PPCost.hpp"

namespace rat
{

class PrepScreen : public Module <Window, Input, Script>
{
public:
    void init()
    {
        Script& script = getModule<Script>();
        script.scriptFile("D:/Sczur/SzczurEngine/src/Szczur/Modules/PrepScreen/PrepScreenGUI/PSGUI.lua");
    }

    PrepScreen()
    {
        LOG_INFO(this, " : Module PrepScreen initing...");
        init();
        LOG_INFO(this, " : Module PrepScreen inited.");
    }

};
}