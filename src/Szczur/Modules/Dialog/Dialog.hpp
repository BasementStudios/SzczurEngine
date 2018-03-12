#pragma once

#include <iostream>

#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Json.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

#include "DialogManager.hpp"

#include "DialogGUI.hpp"

namespace rat {
    class Dialog : public Module<GUI> {
    public:
        using Key_t = Hash32_t;
        using Dialogs_t = boost::container::flat_map<Key_t, DialogManager*>;

        template<typename Tuple>
        Dialog(Tuple&& tuple, const std::string& file);

        ~Dialog();

        void update(float deltaTime=1.f/60.f);

        DialogManager* load(const std::string& path);

        bool unload(const std::string& path);

    private:
        DialogGUI _dialogGUI;
        Interface* _interface;
        TextAreaWidget* _area;
        Dialogs_t _dialogs;

    };
}

#include "Dialog.tpp"