#include "DialogStruct.hpp"

namespace rat {
    DialogStruct::DialogStruct()
    {

    }

    DialogStruct::~DialogStruct() {
        for(auto it : _texts)
            delete it;
    }

    std::string* DialogStruct::add(std::string* text) {
        if(text) {
            _texts.push_back(text);
            _current = _texts.begin();
        }
        return text;
    }

    std::string* DialogStruct::next() {
        auto temp = _current;
        ++_current;
        if(_current == _texts.end())
            _current = _texts.begin();
        return *temp;
    }

    void DialogStruct::restart() {
        _current = _texts.begin();
    }
}