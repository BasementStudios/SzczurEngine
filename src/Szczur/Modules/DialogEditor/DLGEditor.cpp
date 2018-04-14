#include "DLGEditor.hpp"

#include <fstream>
#include <iostream>

#include <string>
#include <cmath>
#include <stdlib.h>


namespace rat
{
    DLGEditor::DLGEditor(std::vector<std::string>& characters)
        : _characters(characters), _textManager(characters), _parts(_textManager.getContainer())
    {
        
    }

    void DLGEditor::load(const std::string& path)
    {
        _dialogPath =  path;
        _dialogPath += "/dialog";

        _buffer.loadFromFile(_dialogPath + ".flac");
        _dialogAudio.setBuffer(_buffer);
        _dialogAudio.setLoop(true);
        _audioDuration = _buffer.getDuration().asSeconds();

        loadData();
    }

    void DLGEditor::loadData() 
    {
        _textManager.load(_dialogPath + ".dlg"); 

        _currentMajor = _parts.begin()->first;
        _currentMinor = _parts[_currentMajor].begin()->first;
    }

    void DLGEditor::update()
    {
        show();
        playAudio();
    }

    void DLGEditor::save()
    {
        std::ofstream dlg;
        dlg.open(_dialogPath + ".dlg", std::ios::trunc);
        if (dlg.is_open()) {
            for(auto major : _parts) {
                for (auto it : major.second) {
                    dlg << "[" << it.second->id << "][" << it.second->minorId << "]";
                    dlg << "[" << it.second->audioStartTime << "-" << it.second->audioEndTime << "]";
                    dlg << "[" << (it.second->label.empty() ? "-" : it.second->label) << "]";
                    dlg << "{\n\0";
                        for (unsigned int i = 0; i < it.second->dialogLines; ++i) {
                            dlg << "\t[" << it.second->dialogTime[i] << "]";
                            dlg << "[" << _characters[it.second->chosenCharacter[i]] << "]";
                            dlg << (it.second->dialogs[i].empty() ? "_" : it.second->dialogs[i]) << "\n\0";
                        } 
                    dlg << "}\n\0\n\0";
                }
            }
        }
        dlg.close();
    }

    DLGEditor::TextContainer_t& DLGEditor::getContainer()
    {
        return _parts;
    }

    void DLGEditor::show()
    {
        ImGui::Begin("Dlg Files Editor");
            player();              ImGui::Separator();
            majorPartSelector();   ImGui::Separator();
            minorPartSelector();   ImGui::Separator();
            labelEditor();         ImGui::Separator();
            timeSelector();        ImGui::Separator();
            mainEditor();
        ImGui::End();
    }

        
    void DLGEditor::player()
    { 
        _sliderTime = _dialogAudio.getPlayingOffset().asSeconds();
        _sliderTimeString = toTime(static_cast<int>(_sliderTime));
        if (ImGui::SliderFloat("##DialogAudioPlayer", &_sliderTime, 0, _audioDuration, _sliderTimeString.c_str())) {
            if (_dialogAudio.getStatus() == sf::SoundSource::Status::Stopped) {
                _dialogAudio.play();
                _dialogAudio.pause();
            }
            _dialogAudio.setPlayingOffset(sf::seconds(_sliderTime));
            _playTest = false;
        }
        ImGui::SameLine();
        ImVec2 buttonSize {(ImGui::GetWindowWidth() - 40) / 100 * 33, 23};
        if (_playing) {
            if (ImGui::Button("PAUSE##DialogAudioPlayer", buttonSize)) {
                _dialogAudio.pause();
                _playing = false;
                _playTest = false;
            }
        } 
        else {
            if (ImGui::Button("PLAY##DialogAudioPlayer", buttonSize)) {
                _dialogAudio.play();
                _playing = true;
                _playTest = false;
            }
        }
    }

    void DLGEditor::majorPartSelector()
    {
        for (auto it : _parts) { 
            auto name = std::to_string(it.first) + "##DialogMajorPartSelector";
            bool changeColor = _currentMajor == it.first;
            if (changeColor) { 
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.6f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.8f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.7f)); 
            }
            if (ImGui::Button(name.c_str(), ImVec2(30, 27))) {
                _currentMajor = it.first;
                _currentMinor = it.second.begin()->first;
            }
            if (changeColor) { ImGui::PopStyleColor(3); }
            ImGui::SameLine();
        }
        if (ImGui::GetWindowWidth() - ImGui::GetCursorPosX() - 80 >= 0) ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80);
        if (ImGui::Button("-##DialogMajorPartSelector", ImVec2(30, 27))) {
            size_t major = (--_parts.end())->first;

            if (major == _parts.begin()->first) 
				return;

            if (_currentMajor == major) { 
				--_currentMajor; 
				_currentMinor = _parts[_currentMajor].begin()->first;
			}

            for (auto it : _parts[major]) {
                delete it.second;
            }

            _parts.erase(major);
        } 
        ImGui::SameLine();
        if (ImGui::Button("+##DialogMajorPartSelector", ImVec2(30, 27))) {
            size_t major = (--_parts.end())->first;
            ++major;
            DialogData* temp = new DialogData {
                major,
                1,
                "00:00",
                "00:00",
                "",
                {""},
                {"00:00"},
                {0},
                1
            };
            _parts[major] = {std::make_pair(1, temp)};
        }
    }

    void DLGEditor::minorPartSelector()
    {
        auto major = _parts.find(_currentMajor);
        for (auto it : major->second) { 
            auto name = std::to_string(it.first) + "##DialogMinorPartSelector";
            bool changeColor = _currentMinor == it.first;
            if (changeColor) { 
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.6f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.8f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.7f)); 
            }
            if (ImGui::Button(name.c_str(), ImVec2(30, 27))) {
                _currentMinor = it.first;
            }
            if (changeColor) { ImGui::PopStyleColor(3); }
            ImGui::SameLine();
        }
        if (ImGui::GetWindowWidth() - ImGui::GetCursorPosX() - 80 >= 0) ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80);
        if (ImGui::Button("-##DialogMinorPartSelector", ImVec2(30, 27))) {
            size_t minor = (--_parts[_currentMajor].end())->first;
            if (minor == _parts[_currentMajor].begin()->first) { return; }
            if (_currentMinor == minor) { --_currentMinor; }
            delete _parts[_currentMajor][minor];
            _parts[_currentMajor].erase(minor);
        } 
        ImGui::SameLine();
        if (ImGui::Button("+##DialogMinorPartSelector", ImVec2(30, 27))) {
            size_t minor = (--_parts[_currentMajor].end())->first;
            ++minor;
            DialogData* temp = new DialogData {
                _currentMajor,
                minor,
                "00:00",
                "00:00",
                "",
                {""},
                {"00:00"},
                {0},
                1
            };
            _parts[_currentMajor].insert_or_assign(minor, temp);
        }
    }

    void DLGEditor::labelEditor()
    {
        std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
        size_t clipSize = clipText.length();
        
        size_t size = _parts[_currentMajor][_currentMinor]->label.length() + clipSize + 30;
        char *newText = new char[size] {};
        strncpy(newText, _parts[_currentMajor][_currentMinor]->label.c_str(), size);

        ImGui::InputText(" - Label##LabelNameInput", newText, size);
       
        auto major = _parts.find(_currentMajor);
        for (auto it : major->second) { 
            it.second->label = newText;
        }

		delete[] newText;
    }

    void DLGEditor::timeSelector()
    {
        char tempStartTime[6] = "00:00";
        char tempEndTime[6] = "00:00";
        strncpy(tempStartTime, _parts[_currentMajor][_currentMinor]->audioStartTime.c_str(), 6);
        strncpy(tempEndTime, _parts[_currentMajor][_currentMinor]->audioEndTime.c_str(), 6);

        if (ImGui::Button("##DialogTimeSelector-FromSliderToStartTime", ImVec2(50, 10))) {
            _startTestPlayerOffset = toIntSeconds(_sliderTimeString);
            _parts[_currentMajor][_currentMinor]->audioStartTime = toTime(_sliderTime);
        } 
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16); 
        if (ImGui::Button("##DialogTimeSelectorStart-FromSliderToEndTime", ImVec2(50, 10))) {
           _endTestPlayerOffset = toIntSeconds(_sliderTimeString);
           _parts[_currentMajor][_currentMinor]->audioEndTime = toTime(_sliderTime);
        }

        ImGui::PushItemWidth(50);
            if (ImGui::InputText("##DialogTimeSelectorStart", tempStartTime, 6, 1)) {
                _startTestPlayerOffset = toIntSeconds(tempStartTime);
                if (_startTestPlayerOffset != 0) _parts[_currentMajor][_currentMinor]->audioStartTime = tempStartTime; 
                else _parts[_currentMajor][_currentMinor]->audioStartTime = "00:00";
            } 
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::Text("-");
        ImGui::SameLine();
        ImGui::PushItemWidth(50);
        if (ImGui::InputText("##DialogTimeSelectorEnd", tempEndTime, 6, 1)) {
            _endTestPlayerOffset = toIntSeconds(tempEndTime);
            if (_endTestPlayerOffset != 0) _parts[_currentMajor][_currentMinor]->audioEndTime = tempEndTime; 
            else _parts[_currentMajor][_currentMinor]->audioEndTime = "00:00";
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14); 
        if (ImGui::Button("TEST##DialogTimeSelector", ImVec2(ImGui::GetWindowWidth() - 152, 36))) {
            _playTest = true;
            _playing  = true;
            _startPlayerOffset = _startTestPlayerOffset;
            _endPlayerOffset   = _endTestPlayerOffset;
            _dialogAudio.play();
            _dialogAudio.setPlayingOffset(sf::seconds(_startPlayerOffset));
        }
    }

    void DLGEditor::mainEditor()
    {
        char dialogTime[6] = "";
        strncpy(dialogTime, _parts[_currentMajor][_currentMinor]->dialogTime[0].c_str(), 6);

        for (unsigned int i = 0; i < _parts[_currentMajor][_currentMinor]->dialogLines; ++i) {
            strcpy(dialogTime, _parts[_currentMajor][_currentMinor]->dialogTime[i].c_str());
            std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
            size_t clipSize = clipText.length();
            size_t size = _parts[_currentMajor][_currentMinor]->dialogs[i].length() + clipSize + 25;
			char *newText = new char[size] {};

            strncpy(newText, _parts[_currentMajor][_currentMinor]->dialogs[i].c_str(), size);

            ImGui::PushID(i);
                if (ImGui::Button("##FromPlayerToDLGInput", ImVec2(20, 22))) {
                    strcpy(dialogTime, _sliderTimeString.c_str());
                }
                ImGui::SameLine();
                ImGui::PushItemWidth(50); ImGui::InputText("##DLGTimeInput", dialogTime, 6, 1); ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::GetWindowWidth() - 292 > 0) {
                    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 292); 
                    ImGui::InputText("##DLGDialogTextInput", newText, size); 
                    ImGui::SameLine();
                }
                else {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6);
                }
                ImGui::PushItemWidth(150); 
                combo("##CharacterCombo", &_parts[_currentMajor][_currentMinor]->chosenCharacter[i], _characters, _characters.size());
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("P##PlayTestOneLineOfDialog", ImVec2(20, 22))) {
                    _playTest = true;
                    _playing  = true;
                    _startPlayerOffset = toIntSeconds(dialogTime);
                    if(i+1 == _parts[_currentMajor][_currentMinor]->dialogLines) {
                        _endPlayerOffset = toIntSeconds(_parts[_currentMajor][_currentMinor]->audioEndTime);
                    } 
                    else {
                        _endPlayerOffset = toIntSeconds(_parts[_currentMajor][_currentMinor]->dialogTime[i + 1]);
                    }
                    _dialogAudio.play();
                    _dialogAudio.setPlayingOffset(sf::seconds(_startPlayerOffset));
                }
            ImGui::PopID();

            _parts[_currentMajor][_currentMinor]->dialogTime[i] = dialogTime;
            _parts[_currentMajor][_currentMinor]->dialogs[i] = newText;

			delete[] newText;
        }

        if (ImGui::GetWindowWidth() - ImGui::GetCursorPosX() - 80 >= 0) ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80);
        if (ImGui::Button("-##RemoveDialog", ImVec2(30, 27))) {
            --(_parts[_currentMajor][_currentMinor]->dialogLines);
            _parts[_currentMajor][_currentMinor]->dialogs.pop_back();
            _parts[_currentMajor][_currentMinor]->dialogTime.pop_back();
            _parts[_currentMajor][_currentMinor]->chosenCharacter.pop_back();
        }
        ImGui::SameLine();
        if (ImGui::Button("+##AddDialog", ImVec2(30, 27))) {
            ++_parts[_currentMajor][_currentMinor]->dialogLines;
            _parts[_currentMajor][_currentMinor]->dialogs.push_back("");
            _parts[_currentMajor][_currentMinor]->dialogTime.push_back(dialogTime);
            _parts[_currentMajor][_currentMinor]->chosenCharacter.push_back(0);
        }
    }
 
    void DLGEditor::playAudio()
    {
        if (_playTest) {
            int offset = _dialogAudio.getPlayingOffset().asSeconds();
            if (offset >= _endPlayerOffset) {
                _dialogAudio.pause();
                _playTest = false;
                _playing = false;
            }
        }
    }

    std::string DLGEditor::toTime(int secI)
    {
        auto minI = secI / 60;
        secI -= minI * 60;

        auto minS = std::to_string(minI);
        auto secS = std::to_string(secI);

        return (minI >= 10 ? minS : "0" + minS) + ":" + (secI >= 10 ? secS : "0" + secS);
    }

    int DLGEditor::toIntSeconds(const std::string& timeString)
    {
        auto semicolonPos = timeString.find(':');
        if (semicolonPos == std::string::npos || semicolonPos != 2) return 0;

        return (atoi(&timeString[0]) * 60) + atoi(&timeString[3]);
    }

    bool DLGEditor::combo(const char* label, int* currentItem, const std::vector<std::string>& items, int itemsCount, int heightInItems)
    {
        return ImGui::Combo(label, currentItem, [](void* vec, int idx, const char** outText) { 
                auto& vector = *((std::vector<std::string>*)vec);
                if (idx < 0 || idx >= (int)(vector.size())) { 
                    return false; 
                }
                *outText = vector[idx].c_str();
                return true;
            }, (void*)&items, itemsCount, heightInItems);
    }
}