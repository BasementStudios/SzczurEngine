#include "SoundEditor.hpp"

#include <cmath>
#include <fstream>

#include <experimental/filesystem>
#include "Szczur/Modules/FileSystem/FileDialog.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

#include <nlohmann/json.hpp>

namespace rat
{
    SoundEditor::SoundEditor(Sound& sound)
        : _sound(sound), _assets(sound.getAssetsManager())
    {
        _currentEditing = _soundHolder.end();
    }

    void SoundEditor::render()
    { 
        if(_loadingSound)
            load();

        ImGui::Begin("Sounds List", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

            if (ImGui::Button("Save##SoundLists")) {
               for(auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
                    save(it);
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Load##SoundLists")) {
                for(auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
                    save(it);
                }
                _loadingSound = true;
                getJsonData();
            }

            ImGui::SameLine();

            if (ImGui::Button("Add##SoundLists")) {
                add();
            }

            ImGui::Separator();

            if (ImGui::TreeNode("Sounds List")) { 
                for(auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
                    auto name = it->getName();
                    if (ImGui::Button((name).c_str())) {
                        if (_currentEditing != _soundHolder.end() && _currentEditing->getName() != name)
                            _sound.stop();
                        _currentEditing = it;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Save")) {
                        save(it);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("-")) {
                        _soundHolder.erase(it);
                        if (_currentEditing->getName() == name)
                            _currentEditing = _soundHolder.end();
                        break;
                    }
                }
                ImGui::TreePop();   
            }


        ImGui::End();

        if (_currentEditing != _soundHolder.end()) {
            ImGui::Begin("Sound Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

                ImGui::Text(("Path: " + _currentEditing->getFileName()).c_str());

                ImGui::Separator();

                auto currName = _currentEditing->getName();

                ImGui::Text("Name: "); 
                ImGui::SameLine();

                size_t size = currName.length() + 100;
                char *newText = new char[size] {};
                strncpy(newText, currName.c_str(), size);

                ImGui::PushItemWidth(300);
                    if (ImGui::InputText("##SoundNameInput", newText, size)) {
                        currName = newText;
                        _currentEditing->setName(currName);
                    }
                ImGui::PopItemWidth();

                delete[] newText;

                ImGui::Separator();

                bool relative = _currentEditing->isRelativeToListener();
                ImGui::Checkbox("Relative To Listener", &relative);
                _currentEditing->setRelativeToListener(relative);

                ImGui::Separator();

                if (ImGui::Button("PLAY##SoundEditor")) {
                    _currentEditing->play();
                }
                ImGui::SameLine();
                if (ImGui::Button("PAUSE##SoundEditor")) {
                    _sound.pause();
                }
                
                ImGui::SameLine();
                if (ImGui::Button("STOP##SoundEditor")) {
                    _sound.stop();
                }   

                ImGui::Separator();

                char beginTime[6] = "00:00"; 
                char endTime[6] = "00:00";

                strncpy(beginTime, toTime(_currentEditing->getBeginTime()).c_str(), 6);
                strncpy(endTime, toTime(_currentEditing->getEndTime()).c_str(), 6);

                ImGui::PushItemWidth(50); 
                if (ImGui::InputText("##SongBeginTimeSelector", beginTime, 6, 1)) {
                    std::string timeString = beginTime;
                    checkTimeString(timeString);
                    _currentEditing->setOffset(toFloatSeconds(timeString), _currentEditing->getEndTime());
                }
                ImGui::PopItemWidth();
                ImGui::SameLine();
                ImGui::Text("-");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                if (ImGui::InputText("##SongEndTimeSelector", endTime, 6, 1)) {
                    std::string timeString = endTime;
                    checkTimeString(timeString);
                    _currentEditing->setOffset(_currentEditing->getBeginTime(), toFloatSeconds(timeString));
                }
                ImGui::PopItemWidth();
                
                ImGui::SameLine();
                ImGui::Text("Offset");

                ImGui::Separator();

                float volume = _currentEditing->getVolume();
                if (ImGui::SliderFloat("Volume##SoundEditor", &volume, 0, 100)) {
                    _currentEditing->setVolume(volume);
                }

                ImGui::Separator();

                float pitch = _currentEditing->getPitch();
                if (ImGui::InputFloat("Pitch##SoundEditor", &pitch, 0.0f, 0.0f, 2)) {
                    _currentEditing->setPitch(pitch);
                }

                ImGui::Separator();

                float attenuation = _currentEditing->getAttenuation();
                if (ImGui::SliderFloat("Attenuation##SoundEditor", &attenuation, 0, 100)) {
                    _currentEditing->setAttenuation(attenuation);
                }

                ImGui::Separator();

                float minDistance = _currentEditing->getMinDistance();
                if (ImGui::InputFloat("Minimum Distance##SoundEditor", &minDistance, 0.0f, 0.0f, 2)) {
                    if(minDistance > 0) {
                        _currentEditing->setMinDistance(minDistance);
                    }
                }

                ImGui::Separator();


            ImGui::End();
        }
    }

    void SoundEditor::save(Container_t::iterator it)
    {
        auto name = it->getName();

        if(name == "Unnnamed")
            return;

        nlohmann::json j;

        std::ifstream ifile(SOUND_DATA_FILE_PATH);
        if (ifile.is_open()) {
            ifile >> j;
            ifile.close();
        }

        std::ofstream ofile(SOUND_DATA_FILE_PATH, std::ios::trunc);
        if (ofile.is_open()) {
            j[name]["Path"] = it->getFileName();
            j[name]["BeginTime"] = it->getBeginTime();
            j[name]["EndTime"] = it->getEndTime();
            j[name]["Volume"] = it->getVolume();
            j[name]["Pitch"] = it->getPitch();
            j[name]["Attenuation"] = it->getAttenuation();
            j[name]["MinDistance"] = it->getMinDistance();
            j[name]["Relative"] = it->isRelativeToListener();

            ofile << std::setw(4) << j << std::endl;
            ofile.close();
        }
    }

    void SoundEditor::load()
    {
        ImGui::Begin("Load Sound");

            for(unsigned int i = 0; i < _loadingNames.size(); ++i) {
                ImGui::Checkbox(_loadingNames[i].c_str(), &_choosedNames[i]);
            }

            if (ImGui::Button("CANCEL##LoadSound")) {
                _loadingNames.clear();
                delete[] _choosedNames;
                _loadingSound = false;
            }

            ImGui::SameLine(); 

            if (ImGui::Button(" OK ##LoadSound")) {
                updateSoundHolder();
                _loadingNames.clear();
                delete[] _choosedNames;
                _loadingSound = false;
            }

        ImGui::End();
    
    }

    void SoundEditor::add()
    {
        auto currentPath = std::experimental::filesystem::current_path().string();
        auto path = FileDialog::getOpenFileName("", currentPath, "Sound files (*.flac)|*.flac");
        std::string filePath;
        
        if (path.find(currentPath) != std::string::npos && !path.empty()) {
            filePath = path.substr(currentPath.length() + 1, path.length());
            std::replace(filePath.begin(), filePath.end(), '\\', '/');

            _soundHolder.push_back(SoundBase("Unnnamed"));
            _assets.load(filePath);
            _soundHolder.back().setBuffer(_assets.get(filePath));
            _soundHolder.back().init();
            _soundHolder.back().setFileName(filePath);
        }
    }

    void SoundEditor::updateSoundHolder()
    {
        _soundHolder.clear();
        _currentEditing = _soundHolder.end();

        for (int i = _loadingNames.size() - 1; i >= 0; --i) {   
            if (_choosedNames[i]) {
                _soundHolder.push_back(SoundBase(_loadingNames[i]));
                _soundHolder.back().load();

                auto fileName = _soundHolder.back().getFileName();

                if(fileName.empty()) {
                    _soundHolder.pop_back();
                } 
                else {
                    _assets.load(fileName);
                    _soundHolder.back().setBuffer(_assets.get(fileName));
                    _soundHolder.back().init();
                }        
            }   
        }
    }

    void SoundEditor::getJsonData()
    {
        nlohmann::json j;

        std::ifstream ifile(SOUND_DATA_FILE_PATH);
        if (ifile.is_open()) {
            ifile >> j;
            ifile.close();
        }

        for (auto it = j.begin(); it != j.end(); ++it)
            _loadingNames.push_back(it.key());

        _choosedNames = new bool[_loadingNames.size()] {false};

        for (unsigned int i = 0; i < _loadingNames.size(); ++i) {
            for(auto& it : _soundHolder) {
                if(_loadingNames[i] == it.getName())
                    _choosedNames[i] = true;
            }
        }
    }

    std::string SoundEditor::toTime(float secF)
    {
        int minI = std::floor(secF);
        int secI = (secF - minI) * 100;

        auto minS = std::to_string(minI);
        auto secS = std::to_string(secI);

        return (minI >= 10 ? minS : "0" + minS) + ":" + (secI >= 10 ? secS : "0" + secS);
    }

    float SoundEditor::toFloatSeconds(const std::string& timeString) const
    {
        return atoi(&timeString.at(0)) + (float(atoi(&timeString.at(3))) / 100);
    }

    void SoundEditor::checkTimeString(std::string& timeString)
    {
        auto semicolonPos = timeString.find(':');
        if (semicolonPos == std::string::npos || semicolonPos != 2 || timeString.length() != 5) {
            timeString = "00:00";
        } 
        else if (timeString[3] >= 54) {
            timeString[3] -= 6;
            timeString[1] += 1;
        }
    }

}