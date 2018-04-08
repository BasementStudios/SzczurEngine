#include "DLGTextManager.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <memory>

namespace rat 
{

    DLGTextManager::DLGTextManager(std::vector<std::string>& characters)
        : _characters(characters)
    {

    }

    DLGTextManager::~DLGTextManager() 
    {
        for(auto& it1 : _texts)
            for(auto& it2 : it1.second)
                delete it2.second;
    }

    void DLGTextManager::load(const std::string& path) 
    {
        for(auto& it1 : _texts)
            for(auto& it2 : it1.second)
                delete it2.second;
        _texts.clear();

        using namespace std::string_literals;
        std::ifstream file(path);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        std::regex word_regex( R"(\[(\d+)\][\s]*\[(\d+)\][\s]*\[(\d+)\:(\d+)\-(\d+)\:(\d+)\][\s]*\[(.+)\][\s]*\{\n?([\d\D][^\}]+))"s);
        for(auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it!=std::sregex_iterator(); ++it) {
            DialogData* obj = new DialogData {
                std::stoi(it->str(1u)),
                std::stoi(it->str(2u)),
                it->str(3u) + ":" + it->str(4u),
                it->str(5u) + ":" + it->str(6u),
                (it->str(7u) == "-" ? "" : it->str(7u))
            };

            obj->interpretText(it->str(8u), _characters);
            
            add(obj->id, obj->minorId, obj);
        }
    }

    void DLGTextManager::add(const size_t key1, const size_t key2, DialogData* dialog) 
    {
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
    }

    DLGTextManager::TextContainer_t& DLGTextManager::getContainer() 
    {
        return _texts;
    }

}