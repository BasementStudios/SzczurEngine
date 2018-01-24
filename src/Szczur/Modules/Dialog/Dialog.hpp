#pragma once

#include <iostream>

#include "Szczur/Utility/Module.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat {
    class Dialog : public Module<GUI> {
    public:
        template<typename Tuple>
        Dialog(Tuple&& tuple, const std::string& file);

    private:

        Interface* _interface;

    };
}

#include "Dialog.tpp"