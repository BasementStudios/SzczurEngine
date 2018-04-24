#include "DLGEditor.hpp"

#include <fstream>

#include <cmath>
#include <cstdlib>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    DLGEditor::DLGEditor(std::vector<CharacterData>& characters)
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
        _textManager.save(_dialogPath + ".dlg");
    }

    DLGEditor::TextContainer_t& DLGEditor::getContainer()
    {
        return _parts;
    }

    void DLGEditor::setCurrentMajor(int major)
    { 
        _currentMajor = major; 
        _currentMinor = _parts[_currentMajor].begin()->first;
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
        _sliderTime       = _dialogAudio.getPlayingOffset().asSeconds();
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
            bool isCurrenMajor = _currentMajor == it.first;
            if (isCurrenMajor) { 
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.6f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.8f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.5f, 0.79f, 0.7f)); 
            }
            if (ImGui::Button(name.c_str(), ImVec2(30, 27)) && !isCurrenMajor) {
                _currentMajor = it.first;
                _currentMinor = it.second.begin()->first;
            }
            if (isCurrenMajor) { ImGui::PopStyleColor(3); }
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
            LOG_INFO("Major part has been removed");
        } 
        ImGui::SameLine();
        if (ImGui::Button("+##DialogMajorPartSelector", ImVec2(30, 27))) {
            size_t major = (--_parts.end())->first;
            ++major;
            _textManager.add(major, 1, NULL);
            LOG_INFO("New major part added");
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
            LOG_INFO("Minor part has been removed");
        } 
        ImGui::SameLine();
        if (ImGui::Button("+##DialogMinorPartSelector", ImVec2(30, 27))) {
            size_t minor = (--_parts[_currentMajor].end())->first;
            ++minor;
            _textManager.add(_currentMajor, minor, NULL);
            LOG_INFO("New minor part added");
        }
    }

    void DLGEditor::labelEditor()
    {    
        size_t size = _parts[_currentMajor][_currentMinor]->label.length() + 100;
        char *newText = new char[size] {};
        strncpy(newText, _parts[_currentMajor][_currentMinor]->label.c_str(), size);

        if (ImGui::InputText(" - Label##LabelNameInput", newText, size)) {
            auto major = _parts.find(_currentMajor);
            for (auto it : major->second) { 
                it.second->label = newText;
            }
        };
       
		delete[] newText;
    }

    void DLGEditor::timeSelector()
    {
        char tempStartTime[6] = "00:00";
        char tempEndTime[6] = "00:00";
        strncpy(tempStartTime, _parts[_currentMajor][_currentMinor]->audioStartTime.c_str(), 6);
        strncpy(tempEndTime, _parts[_currentMajor][_currentMinor]->audioEndTime.c_str(), 6);

        if (ImGui::Button("##DialogTimeSelector-FromSliderToStartTime", ImVec2(50, 10))) {
            _startTestPlayerOffset = _sliderTime;
            _parts[_currentMajor][_currentMinor]->audioStartTime = _sliderTimeString;
        } 
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16); 
        if (ImGui::Button("##DialogTimeSelectorStart-FromSliderToEndTime", ImVec2(50, 10))) {
           _endTestPlayerOffset = _sliderTime;
           _parts[_currentMajor][_currentMinor]->audioEndTime = _sliderTimeString;
        }

        ImGui::PushItemWidth(50);
            if (ImGui::InputText("##DialogTimeSelectorStart", tempStartTime, 6, 1)) {
                std::string timeString = tempStartTime;
                checkTimeString(timeString);
                _startTestPlayerOffset = toIntSeconds(tempStartTime);
                _parts[_currentMajor][_currentMinor]->audioStartTime = tempStartTime;
            } 
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::Text("-");
        ImGui::SameLine();
        ImGui::PushItemWidth(50);
        if (ImGui::InputText("##DialogTimeSelectorEnd", tempEndTime, 6, 1)) {
            std::string timeString = tempEndTime;
            checkTimeString(timeString);
            _endTestPlayerOffset = toIntSeconds(timeString);
            if (_endTestPlayerOffset > 0) _parts[_currentMajor][_currentMinor]->audioEndTime = timeString;
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
            size_t size = _parts[_currentMajor][_currentMinor]->dialogs[i].length() + 100;
			char *newText = new char[size] {};

            strncpy(newText, _parts[_currentMajor][_currentMinor]->dialogs[i].c_str(), size);

            ImGui::PushID(i);
                if (ImGui::Button("##FromPlayerToDLGInput", ImVec2(20, 22))) {
                   _parts[_currentMajor][_currentMinor]->dialogTime[i] = _sliderTimeString;
                }
                ImGui::SameLine();
                ImGui::PushItemWidth(50); 
                if (ImGui::InputText("##DLGTimeInput", dialogTime, 6, 1)) {
                    std::string timeString = dialogTime;
                    checkTimeString(timeString);
                    _parts[_currentMajor][_currentMinor]->dialogTime[i] = timeString;
                }
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::GetWindowWidth() - 292 > 0) {
                    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 292); 
                    if (ImGui::InputText("##DLGDialogTextInput", newText, size)) {
                        _parts[_currentMajor][_currentMinor]->dialogs[i] = newText;
                    }; 
                    delete[] newText;
                    ImGui::SameLine();
                }
                else {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6);
                }
                ImGui::PushItemWidth(150); 
                charactersCombo(&_parts[_currentMajor][_currentMinor]->chosenCharacter[i]);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("P##PlayTestOneLineOfDialog", ImVec2(20, 22))) {
                    _playTest = true;
                    _playing  = true;
                    _startPlayerOffset = toIntSeconds(dialogTime);
                    if (i+1 == _parts[_currentMajor][_currentMinor]->dialogLines) {
                        _endPlayerOffset = toIntSeconds(_parts[_currentMajor][_currentMinor]->audioEndTime);
                    } 
                    else {
                        _endPlayerOffset = toIntSeconds(_parts[_currentMajor][_currentMinor]->dialogTime[i + 1]);
                    }
                    _dialogAudio.play();
                    _dialogAudio.setPlayingOffset(sf::seconds(_startPlayerOffset));
                }
            ImGui::PopID();
        }

        if (ImGui::GetWindowWidth() - ImGui::GetCursorPosX() - 80 >= 0) ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80);
        if (ImGui::Button("-##RemoveDialog", ImVec2(30, 27)) && _parts[_currentMajor][_currentMinor]->dialogLines > 1) {
            --_parts[_currentMajor][_currentMinor]->dialogLines;
            _parts[_currentMajor][_currentMinor]->dialogs.pop_back();
            _parts[_currentMajor][_currentMinor]->dialogTime.pop_back();
            _parts[_currentMajor][_currentMinor]->chosenCharacter.pop_back();
            LOG_INFO("Dialog part has been removed");
        }
        ImGui::SameLine();
        if (ImGui::Button("+##AddDialog", ImVec2(30, 27))) {
            ++_parts[_currentMajor][_currentMinor]->dialogLines;
            _parts[_currentMajor][_currentMinor]->dialogs.push_back("");
            _parts[_currentMajor][_currentMinor]->dialogTime.push_back(dialogTime);
            _parts[_currentMajor][_currentMinor]->chosenCharacter.push_back(0);
            LOG_INFO("Added dialog part");
        }
    }
 
    void DLGEditor::playAudio()
    {
        if (_playTest) {
            if (_dialogAudio.getPlayingOffset().asSeconds() >= _endPlayerOffset) {
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

    int DLGEditor::toIntSeconds(const std::string& timeString) const
    {
        return (atoi(&timeString[0]) * 60) + atoi(&timeString[3]);
    }

    void DLGEditor::checkTimeString(std::string& timeString)
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

    bool DLGEditor::charactersCombo(int* currentItem) const
    {
        return ImGui::Combo("##CharacterCombo", currentItem, [](void* vec, int idx, const char** outText) { 
                auto& vector = *((std::vector<CharacterData>*)vec);
                if (idx < 0 || idx >= (int)(vector.size())) { 
                    return false; 
                }
                *outText = vector[idx].name.c_str();
                return true;
            }, (void*)&_characters, _characters.size());
    }
}