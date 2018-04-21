#include "DLGTextManager.hpp"

#include <regex>

#include "Szczur/Utility/Logger.hpp"

namespace rat 
{

    DLGTextManager::DLGTextManager(std::vector<CharacterData>& characters)
        : _characters(characters)
    {

    }

    DLGTextManager::~DLGTextManager() 
    {
        for(auto& it1 : _texts)
            for(auto& it2 : it1.second)
                delete it2.second;
    }

    void DLGTextManager::save(const std::string& path)
    {
        std::ofstream dlg;
        dlg.open(path, std::ios::trunc);
        if (dlg.is_open()) {
            for(auto major : _texts) {
                for (auto it : major.second) {
                    dlg << "[" << it.second->id << "][" << it.second->minorId << "]";
                    dlg << "[" << it.second->audioStartTime << "-" << it.second->audioEndTime << "]";
                    dlg << "[" << (it.second->label.empty() ? "-" : it.second->label) << "]";
                    dlg << "{\n\0";
                        for (unsigned int i = 0; i < it.second->dialogLines; ++i) {
                            dlg << "\t[" << it.second->dialogTime[i] << "]";
                            dlg << "[" << (_characters.size() ? _characters[it.second->chosenCharacter[i]].name : "-") << "]";
                            dlg << (it.second->dialogs[i].empty() ? "_" : it.second->dialogs[i]) << "\n\0";
                        } 
                    dlg << "}\n\0\n\0";
                }
            }
        }
        dlg.close();

        LOG_INFO("Dlg saved into: ", path);
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

        if (str.empty()) {
            std::ofstream dlg;
            dlg.open(path);
            if (dlg.is_open()) {
                dlg << "[1][1][00:00-00:00][-]{\n\0" << "\t[00:00][_]_\n\0" << "}\n\0";
            }
            dlg.close();
            LOG_INFO("Generated new dlg file: ", path);

            add(1, 1, NULL);
        }
        else {
            std::regex word_regex( R"(\[(\d+)\][\s]*\[(\d+)\][\s]*\[(\d+)\:(\d+)\-(\d+)\:(\d+)\][\s]*\[(.+)\][\s]*\{\n?([\d\D][^\}]+))"s);
            for(auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it!=std::sregex_iterator(); ++it) {
                DialogData* obj = new DialogData {
                    static_cast<unsigned>(std::stoi(it->str(1u))),
                    static_cast<unsigned>(std::stoi(it->str(2u))),
                    it->str(3u) + ":" + it->str(4u),
                    it->str(5u) + ":" + it->str(6u),
                    (it->str(7u) == "-" ? "" : it->str(7u))
                };

                obj->interpretText(it->str(8u), _characters);
                
                add(obj->id, obj->minorId, obj);
            }
            LOG_INFO("Dialog data loaded from: ", path);
        }

    }

    void DLGTextManager::add(const size_t key1, const size_t key2, DialogData* dialog) 
    {
        if (dialog) {
            if (auto it1 = _texts.find(key1); it1 != _texts.end()) {
                if (auto it2 = it1->second.find(key2); it2 != it1->second.end()) {
                    delete it2->second;
                }
                it1->second.insert_or_assign(key2, dialog);
            }
            else {
                _texts[key1] = {std::make_pair(key2, dialog)};
            }
        }
        else {
            std::string label = "";
            if (auto it = _texts[key1].begin(); it != _texts[key1].end()) {
                label = it->second->label;
            }

            DialogData* obj = new DialogData {
                key1,
                key2,
                "00:00",
                "00:00",
                label,
                {""},
                {"00:00"},
                {0},
                1
            };

            add(key1, key2, obj);
        }
    }

    DLGTextManager::TextContainer_t& DLGTextManager::getContainer() 
    {
        return _texts;
    }

}