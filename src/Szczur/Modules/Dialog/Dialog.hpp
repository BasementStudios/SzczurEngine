#pragma once

#include <iostream>

#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
//#include "Szczur/Json.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Input/Input.hpp"

#include "DialogManager.hpp"

#include "DialogGUI.hpp"

namespace rat {
    class Dialog : public Module<GUI, Script, Input> {
    public:
        using Key_t = Hash32_t;
        //using Dialogs_t = boost::container::flat_map<Key_t, DialogManager*>;

        Dialog();

        ~Dialog();

        

        void update(float deltaTime=1.f/60.f);

        DialogManager* load(const std::string& path);

        void unload();

		bool isDialogPlaying();

    private:
        DialogGUI _dialogGUI;
        DialogManager* _dialogManager = nullptr;

		bool _isDialogPlaying = false;

        void _initScript();
    };
}
