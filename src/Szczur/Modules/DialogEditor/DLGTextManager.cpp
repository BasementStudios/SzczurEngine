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
            for(auto& it2 : it1)
                delete it2;
    }

    void DLGTextManager::save(const std::string& path)
    {
        std::ofstream dlg;
        dlg.open(path, std::ios::trunc);
        if (dlg.is_open()) {
            for(auto major : _texts) {
                for (auto it : major) {
                    dlg << "[" << it->id << (it->majorName.empty() ? "" : (":" +  it->majorName)) << "][" << it->minorId << (it->minorName.empty() ? "" : (":" +  it->minorName)) << "]";
                    dlg << "[" << it->audioStartTime << "-" << it->audioEndTime << "]";
                    dlg << "[" << (it->label.empty() ? "-" : it->label) << "]";
                    dlg << "{\n\0";
                        for (unsigned int i = 0; i < it->dialogLines; ++i) {
                            dlg << "\t[" << it->dialogTime[i] << "]";
                            dlg << "[" << (_characters.size() ? _characters[it->chosenCharacter[i]].name : "-") << "]";
                            dlg << (it->dialogs[i].empty() ? "_" : it->dialogs[i]) << "\n\0";
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
            for(auto& it2 : it1)
                delete it2;
        _texts.clear();

        using namespace std::string_literals;
        std::ifstream file(path);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        if (str.empty()) {
            std::ofstream dlg;
            dlg.open(path);
            if (dlg.is_open()) {
                dlg << "[0][0][00:00-00:00][-]{\n\0" << "\t[00:00][_]_\n\0" << "}\n\0";
            }
            dlg.close();
            LOG_INFO("Generated new dlg file: ", path);

            add(0, 0, NULL);
        }
        else {
            auto majorNr = 0;
            auto minorNr = 0;
            auto prevMajor = -1;

            std::regex word_regex( R"(\[[\s]*(\d+)[\s]*\:*[\s]*(.*)[\s]*\][\s]*\[[\s]*(\d+)[\s]*\:*[\s]*(.*)[\s]*\][\s]*\[(\d+)\:(\d+)\-(\d+)\:(\d+)\][\s]*\[(.+)\][\s]*\{\n?([\d\D][^\}]+))"s);
            for(auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it!=std::sregex_iterator(); ++it) {

                auto majorID = std::stoi(it->str(1u));
                if(majorNr < majorID && majorID != prevMajor && prevMajor != -1) {
                    majorNr++;
                    minorNr = 0;
                }
                prevMajor = majorID;

                DialogData* obj = new DialogData {
                    static_cast<unsigned>(majorNr),
                    it->str(2u),
                    static_cast<unsigned>(minorNr),
                    it->str(4u),
                    it->str(5u) + ":" + it->str(6u),
                    it->str(7u) + ":" + it->str(8u),
                    (it->str(9u) == "-" ? "" : it->str(9u))
                };
                obj->interpretText(it->str(10u), _characters);
                
                add(majorNr, minorNr, obj);

                minorNr++;
            }
            LOG_INFO("Dialog data loaded from: ", path);
        }

    }

    void DLGTextManager::add(const size_t key1, const size_t key2, DialogData* dialog) 
    {
        if (dialog) {
            if (_texts.size() > key1) {
                if(_texts[key1].size() <= key2) {
                    _texts[key1].push_back(dialog);
                }
                else {
                    for(auto i = key2; i < _texts[key1].size(); ++i) {
                        ++(_texts[key1][i]->minorId);
                        _texts[key1][i]->renameMinor(_texts[key1][i]->minorName);
                    }
                    _texts[key1].insert(_texts[key1].begin() + key2, dialog);
                }
            }
            else {
                _texts.push_back({dialog});             
            }
        }
        else {
            std::string majorName = "";
            if (_texts.size() > key1) {
                majorName = _texts[key1][0]->majorName;
            }

            DialogData* obj = new DialogData {
                key1,
                majorName,
                key2,
                "",
                "00:00",
                "00:00",
                "",
                {""},
                {"00:00"},
                {0},
                1,
                ("[" + std::to_string(key1) + "] " + majorName),
                ("[" + std::to_string(key2) + "]")
            };
            add(key1, key2, obj);
        }
    }

    void DLGTextManager::remove(const size_t key1)
    {
        for(auto i = key1 + 1; i < _texts.size(); ++i) {
            for(auto& it : _texts[i]) {
                --(it->id);
                it->renameMajor(it->majorName);
            }
        }

        for(auto& it : _texts[key1])
            delete it;
        _texts.erase(_texts.begin() + key1);
    }

    void DLGTextManager::remove(const size_t key1, const size_t key2)
    {
        for(auto i = key2 + 1; i < _texts[key1].size(); ++i) {
            --(_texts[key1][i]->minorId);
            _texts[key1][i]->renameMinor(_texts[key1][i]->minorName);
        }

        delete _texts[key1][key2];
        _texts[key1].erase(_texts[key1].begin() + key2);
    }

    DLGTextManager::TextContainer_t& DLGTextManager::getContainer() 
    {
        return _texts;
    }

}