#include "SoundEditor.hpp"

#include <Json/json.hpp>

#include <cmath>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include <experimental/filesystem>

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>
#include <fstream>

namespace rat
{
    SoundEditor::SoundEditor(SoundManager& soundManager)
        : _soundManager(soundManager)
    {
        
    }

    void SoundEditor::render()
    { 
        ImGui::Begin("Sounds List", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

            if (ImGui::Button("Save##SoundLists")) {
                for(auto& it : _soundNames) {
                    save(it);
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Load##SoundLists")) {
                load();
            }

            ImGui::Separator();

            if (ImGui::TreeNode("Sounds List")) { 
                for(auto it = _soundNames.begin(); it != _soundNames.end(); ++it) {
                    if (ImGui::Button((*it).c_str())) {
                        if (_currentEditing != *it)
                            _soundManager.stop();
                        _currentEditing = *it;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Save")) {
                        save(*it);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("-")) {
                        _soundManager.eraseSingleSound(*it);
                        _soundNames.erase(it);
                        if (_currentEditing == *it)
                            _currentEditing = "";
                        break;
                    }
                }
                ImGui::TreePop();   
            }


        ImGui::End();

        if (!_currentEditing.empty()) {
            ImGui::Begin("Sound Editor");

                ImGui::Text(("Name: " + _currentEditing).c_str());

                ImGui::Separator();

                if (ImGui::Button("PLAY##SoundEditor")) {
                    _soundManager.play(_currentEditing);
                }
                ImGui::SameLine();
                if (ImGui::Button("PAUSE##SoundEditor")) {
                    _soundManager.pause();
                }
                
                ImGui::SameLine();
                if (ImGui::Button("STOP##SoundEditor")) {
                    _soundManager.stop();
                }   

                ImGui::Separator();

                char beginTime[6] = "00:00"; 
                char endTime[6] = "00:00";

                strncpy(beginTime, toTime(_soundManager.getBeginTime(_currentEditing)).c_str(), 6);
                strncpy(endTime, toTime(_soundManager.getEndTime(_currentEditing)).c_str(), 6);

                ImGui::PushItemWidth(50); 
                if (ImGui::InputText("##SongBegineTimeSelector", beginTime, 6, 1)) {
                    std::string timeString = beginTime;
                    checkTimeString(timeString);
                    _soundManager.setOffset(_currentEditing, toFloatSeconds(timeString), _soundManager.getEndTime(_currentEditing));
                }
                ImGui::PopItemWidth();
                ImGui::SameLine();
                ImGui::Text("-");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                if (ImGui::InputText("##SongEndTimeSelector", endTime, 6, 1)) {
                    std::string timeString = endTime;
                    checkTimeString(timeString);
                    _soundManager.setOffset(_currentEditing, _soundManager.getBeginTime(_currentEditing), toFloatSeconds(timeString));
                }
                ImGui::PopItemWidth();
                
                ImGui::SameLine();
                ImGui::Text("Offset");

                ImGui::Separator();

                float volume = _soundManager.getVolume(_currentEditing);
                if (ImGui::SliderFloat("Volume##SoundEditor", &volume, 0, 100)) {
                    _soundManager.setVolume(volume, _currentEditing);
                }

                ImGui::Separator();

                float pitch = _soundManager.getPitch(_currentEditing);
                if (ImGui::InputFloat("Pitch##SoundEditor", &pitch, 0.0f, 0.0f, 2)) {
                    _soundManager.setPitch(pitch, _currentEditing);
                }

                ImGui::Separator();



            ImGui::End();
        }
    }

    void SoundEditor::save(const std::string& fileName)
    {
        nlohmann::json j;
        std::ofstream file("Assets/Sounds/Data/" + fileName + ".json", std::ios::trunc);

        j["BeginTime"] = _soundManager.getBeginTime(fileName);
        j["EndTime"] = _soundManager.getEndTime(fileName);
        j["Volume"] = _soundManager.getVolume(fileName);
        j["Pitch"] = _soundManager.getPitch(fileName);

        if (file.is_open()) {
            file << j;
            file.close();
        }
    }

    void SoundEditor::load()
    {
        auto currentPath = std::experimental::filesystem::current_path().string();
        auto path = FileDialog::getOpenFileName("", currentPath, "Sound (*.flac)|*.flac");
        std::string filePath;
        size_t start = path.find(currentPath);

        if (start != -1) {
            filePath = path.substr(currentPath.length() + 15, path.length() - 5 - currentPath.length() - 15);
        } 
        else {
            filePath = path;
        }

        std::replace(filePath.begin(), filePath.end(), '\\', '/');
       
        for (auto& it : _soundNames) {
            if (it == filePath)
                return;
        }

        _soundManager.load(filePath);
        _soundNames.push_back(filePath);
    
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