#include "TextManager.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <memory>

namespace rat {
    TextManager::TextManager() {
        
    }

    TextManager::~TextManager() {
        for(auto& it : _texts)
            delete it.second;
    }

    void TextManager::load(const std::string& path) {
        for(auto& it : _texts)
            delete it.second;
        _texts.clear();

        using namespace std::string_literals;
        //std::fstream file(path, std::ios::in);
        std::ifstream file(path);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        std::regex word_regex( R"(\d+|[^\[\]\d\:\-\n]+)"s);
        int i = 0;
        size_t id = 0u;
        std::unique_ptr<TextStruct> ptr = std::make_unique<TextStruct>();
        for(auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it!=std::sregex_iterator(); ++it) {
            switch(i) {
                case 0:
                    id=std::stoi(it->str());
                    ptr->setId(id);
                break;
                case 1:
                    ptr->setVoiceStart(std::stoi(it->str()));
                break;
                case 2:
                    ptr->setVoiceStart(60u*ptr->getVoiceStart() + std::stoi(it->str()));
                break;
                case 3:
                    ptr->setVoiceLength(std::stoi(it->str()));
                break;
                case 4:
                    ptr->setVoiceLength(60u*ptr->getVoiceLength() + std::stoi(it->str()));
                break;
                case 5:
                    ptr->setSpeaker(it->str());
                break;
                case 6:
                    ptr->setText(it->str());
                    add(id, ptr.release());
                    ptr = std::make_unique<TextStruct>();
                break;
            }
            i++;
            if(i>=7)
                i = 0;
        }
    }

    TextManager::Type_t* TextManager::add(const Key_t key, Type_t* dialog) {
        if(dialog) {
            if(auto it = _texts.find(key); it != _texts.end())
                delete it->second;
            _texts.insert_or_assign(key, dialog);
            _current = _texts.begin();
        }
        return dialog;
    }

    const std::string& TextManager::set(const Key_t key) {
        if(auto it = _texts.find(key); it != _texts.end()) {
            _current = it;
            return _current->second->getText();
        }    
    }

    const std::string& TextManager::next() {
        auto temp = _current;
        _current++;
        if(_current == _texts.end())
            _current = _texts.begin();
        return temp->second->getText();
    }

    const std::string& TextManager::getText() const {
        return _current->second->getText();
    }

    TextManager::Type_t* TextManager::getStruct() const {
        return _current->second;
    }
}