#include "TextStruct.hpp"
#include <sstream>
#include <iostream>
#include <regex>

namespace rat {
    TextStruct::TextStruct(size_t id, size_t minorId, size_t voiceStart, size_t voiceEnd) :
    _id(id),
    _minorId(minorId),
    _voiceStart(voiceStart),
    _voiceEnd(voiceEnd)
    {

    }

    TextStruct::~TextStruct() {

    }


    void TextStruct::setId(size_t id) {
        _id = id;
    }
    
    size_t TextStruct::getId() const {
        return _id;
    }

    void TextStruct::setMinorId(size_t minorId) {
        _minorId = minorId;
    }
    
    size_t TextStruct::getMinorId() const {
        return _minorId;
    }

    void TextStruct::setVoiceStart(size_t voiceStart) {
        _voiceStart = voiceStart;
    }
    
    size_t TextStruct::getVoiceStart() const {
        return _voiceStart;
    }

    void TextStruct::setVoiceEnd(size_t voiceEnd) {
        _voiceEnd = voiceEnd;
    }
    
    size_t TextStruct::getVoiceEnd() const {
        return _voiceEnd;
    }
    void TextStruct::interpretText(const std::string& text) {
        using namespace std::string_literals;
        std::stringstream stream;
        stream << text;
        std::string temp;
        std::regex word_regex( R"([\s]*\[(\d+)\:(\d+)\][\s]*\[(\w+)\](.*))"s);
        while(std::getline(stream, temp)) {
            for(auto it = std::sregex_iterator(temp.begin(), temp.end(), word_regex); it!=std::sregex_iterator(); ++it) {
                _texts[static_cast<Key_t>(std::stoi(it->str(1))*60+std::stoi(it->str(2)))] = 
                    {std::make_pair(it->str(3), it->str(4))};
            }
        }
    }

    void TextStruct::forEach(std::function<void(Texts_t::iterator)> func) {
        for(auto it = _texts.begin(); it != _texts.end(); ++it)
            std::invoke(func, it);
    }

    void TextStruct::setLabel(const std::string& label) {
        _label = label;
    }

    const std::string& TextStruct::getLabel() const {
        return _label;
    }

}