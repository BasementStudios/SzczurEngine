#include "TextManager.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <memory>

namespace rat {
    TextManager::TextManager() :
    _finishedMinor(false) {
        
    }

    TextManager::~TextManager() {
        for(auto& it1 : _texts)
            for(auto& it2 : it1.second)
                delete it2.second;
    }

    void TextManager::load(const std::string& path) {
        for(auto& it1 : _texts)
            for(auto& it2 : it1.second)
                delete it2.second;
        _texts.clear();

        using namespace std::string_literals;
        std::ifstream file(path);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        std::regex word_regex( R"(\d+|[^\[\]\d\:\-\n]+)"s);
        int i = 0;
        size_t id = 0u;
        size_t minor = 0u;
        std::unique_ptr<TextStruct> ptr = std::make_unique<TextStruct>();
        for(auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it!=std::sregex_iterator(); ++it) {
            switch(i) {
                case 0:
                    id=std::stoi(it->str());
                    ptr->setId(id);
                break;
                case 1:
                    minor = std::stoi(it->str());
                    ptr->setMinorId(minor);
                break;
                case 2:
                    ptr->setVoiceStart(std::stoi(it->str()));
                break;
                case 3:
                    ptr->setVoiceStart(60u*ptr->getVoiceStart() + std::stoi(it->str()));
                break;
                case 4:
                    ptr->setVoiceEnd(std::stoi(it->str()));
                break;
                case 5:
                    ptr->setVoiceEnd(60u*ptr->getVoiceEnd() + std::stoi(it->str()));
                break;
                case 6:
                    ptr->setSpeaker(it->str());
                break;
                case 7:
                    ptr->setText(it->str());
                    add(id, minor, ptr.release());
                    ptr = std::make_unique<TextStruct>();
                break;
            }
            i++;
            if(i>=8)
                i = 0;
        }
    }

    TextManager::Type_t* TextManager::add(const Key_t key1, const Key_t key2, Type_t* dialog) {
        if(dialog) {
            if(auto it1 = _texts.find(key1); it1 != _texts.end()) {
                if(auto it2 = it1->second.find(key2); it2 != it1->second.end()) {
                    delete it2->second;
                }
                it1->second.insert_or_assign(key2, dialog);
            }
            else {
                _texts[key1] = {std::make_pair(key2, dialog)};
            }
        }
        return dialog;
    }

    void TextManager::set(const Key_t key) {
        _current = _texts.find(key);
        if(_current != _texts.end())
            _minorCurrent = _current->second.begin();
        _finishedMinor = false;
    }

    void TextManager::next() {
        auto temp = _current;
        ++_current;
        if(_current == _texts.end())
            _current = _texts.begin();
        _minorCurrent = _current->second.begin();
        _finishedMinor = false;
    }

    const std::string& TextManager::setMinor(const Key_t key) {
        _minorCurrent = _current->second.find(key);
        _finishedMinor = false;
    }

    const std::string& TextManager::nextMinor() {
        auto temp = _minorCurrent;
        ++_minorCurrent;
        if(_minorCurrent == _current->second.end()) {
            _finishedMinor = true;
            _minorCurrent = _current->second.begin();
        }
        return temp->second->getText();
    }

    const std::string& TextManager::getText() const {
        return _minorCurrent->second->getText();
    }

    TextManager::Type_t* TextManager::getStruct() const {
        return _minorCurrent->second;
    }

    bool TextManager::isMinorFinished() {
        return _finishedMinor;
    }
}