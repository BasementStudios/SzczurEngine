#include "DialogManager.hpp"

namespace rat {
    DialogManager::DialogManager() {
        auto a = add("1", new DialogStruct);
        a->add(new std::string("Witaj przybyszu, czym moge sluzyc twojej laskawosci :)"));
        a->add(new std::string("Niestety nie moge tego dla ciebie zrobic, porozmawiaj z kolega przy przystani"));
        a->add(new std::string("Jestem sobie asdasdasd asd asd asdasdasdasd asd asd asdasdasd asdasd asd asd asd asd asd "));
        a->add(new std::string("Antropogenika to cudowna dziedzina nauk kompletnie zbędnych gatunkowi ludzkiemu"));
        a->add(new std::string("Generalnie to powinienes umrzec na zaraze, ale jestes mi potrzebny wiec lepiej pozostan wsrod zywych :)"));
    }

    DialogManager::~DialogManager() {
        for(auto& it : _dialogs)
            delete it.second;
    }

    DialogStruct* DialogManager::add(const std::string& key, DialogStruct* dialog) {
        if(dialog) {
            Hash32_t k = fnv1a_32(key.begin(), key.end());
            if(auto it = _dialogs.find(k); it != _dialogs.end())
                delete it->second;
            _dialogs.insert_or_assign(k, dialog);
            _current = _dialogs.begin();
        }
        return dialog;
    }

    void DialogManager::selectDialog(const std::string& key) {
        if(auto it = _dialogs.find( fnv1a_32(key.begin(), key.end()) ); it != _dialogs.end()) {
            it->second->restart();
            _current = it;
        }
    }

    std::string* DialogManager::next() {
        return _current->second->next();
    }
}