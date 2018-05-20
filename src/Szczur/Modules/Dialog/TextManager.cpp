#include "TextManager.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <memory>

namespace rat {
    TextManager::TextManager() {
    //_finishedMinor(false) {
        
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
        //  \[(\d+)\][\s]*\[(\d+)\][\s]*\[(\d+)\:(\d+)\-(\d+)\:(\d+)\][\s]*\[(.+)\][\s]*\{\n?([\d\D][^\}]+)
        //std::regex word_regex( R"(\[(\d+)\][\s]*\[(\d+)\][\s]*\[(\d+)\:(\d+)\-(\d+)\:(\d+)\][\s]*\[(.+)\][\s]*\{\n?([\d\D][^\}]+))"s);
        std::regex word_regex( R"(\[[\s]*(\d+)[\s]*\:*[\s]*(.*)[\s]*\][\s]*\[[\s]*(\d+)[\s]*\:*[\s]*(.*)[\s]*\][\s]*\[(\d+)\:(\d+)\-(\d+)\:(\d+)\][\s]*\[(.+)\][\s]*\{\n?([\d\D][^\}]+))"s);
        for(auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it!=std::sregex_iterator(); ++it) {
            Type_t* obj = new Type_t;
            obj->setId( std::stoi(it->str(1u)) );
            obj->setIdName( it->str(2u) );

            obj->setMinorId( std::stoi(it->str(3u)) );
            obj->setMinorIdName( it->str(4u) );

            obj->setVoiceStart( std::stoi(it->str(5u)) * 60 );
            obj->setVoiceStart( std::stoi(it->str(6u)) + obj->getVoiceStart() );

            obj->setVoiceEnd( std::stoi(it->str(7u)) * 60 );
            obj->setVoiceEnd( std::stoi(it->str(8u)) + obj->getVoiceEnd() );

            obj->setLabel(it->str(9u));

            obj->interpretText(it->str(10u));

            add(obj->getId(), obj->getMinorId(), obj);
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

    void TextManager::set(const Key_t major, const Key_t minor) {
        
        if(auto it = _texts.find(major); it != _texts.end()) {
            if(auto it2 = it->second.find(minor); it2 != it->second.end()) 
                _minorCurrent = it2;
            else
                LOG_ERROR("Given Minor was NOT found")
        }
        else
            LOG_ERROR("Given Major was NOT found");
            
    }

    /*void TextManager::next() {
        auto temp = _current;
        ++_current;
        if(_current == _texts.end())
            _current = _texts.begin();
        _minorCurrent = _current->second.begin();
        _finishedMinor = false;
    }*/

    /*void TextManager::setMinor(const Key_t key) {
        _minorCurrent = _current->second.find(key);
        _finishedMinor = false;
    }*/

    /*void TextManager::nextMinor() {
        auto temp = _minorCurrent;
        ++_minorCurrent;
        if(_minorCurrent == _current->second.end()) {
            _finishedMinor = true;
            _minorCurrent = _current->second.begin();
        }
    }*/

    TextManager::Type_t* TextManager::getStruct() const {
        return _minorCurrent->second;
    }

    /*bool TextManager::isMinorFinished() {
        return _finishedMinor;
    }*/

    const std::string& TextManager::getLabel(const Key_t major, const Key_t minor) const {
        if(auto it = _texts.find(major); it != _texts.end()) {
            if(auto it2 = it->second.find(minor); it2 != it->second.end()) {
                return it2->second->getLabel();
            }
        }
        LOG_ERROR("Given ID was NOT found")
        return "Id not found";
    }
}