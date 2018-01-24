#pragma once

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Hash.hpp"

#include "DialogStruct.hpp"

namespace rat {
    class DialogManager {
    public:
        using DialogContainer_t = boost::container::flat_map<Hash32_t, DialogStruct*>;

        DialogManager();
        ~DialogManager();

        DialogStruct* add(const std::string& key, DialogStruct* dialog);
        void selectDialog(const std::string& key);
        std::string* next();
    private:
        DialogContainer_t _dialogs;
        DialogContainer_t::iterator _current;
    };
}