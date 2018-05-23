#include "DLGEditor.hpp"

#include <fstream>

#include <cmath>
#include <cstdlib>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    DLGEditor::DLGEditor(std::vector<CharacterData>& characters, const InputManager& inputManager)
        : _inputManager(inputManager), _characters(characters), _textManager(characters), _parts(_textManager.getContainer())
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

        _currentMajor = 0;
        _currentMinor = 0;
    }

    void DLGEditor::update()
    {
        show();
        playAudio();
        input();
    }

    void DLGEditor::input()
    {

        if (_renaming) {
            if (_inputManager.isPressed(InputCode(Keyboard::Return))) {
                rename();
            }
            if (_inputManager.isPressed(InputCode(Keyboard::Escape))) {
                _renaming = false;
                ImGui::CloseCurrentPopup();
            }
        }

        if(!_isWindowFocused) return;

		if (_inputManager.isPressed(InputCode(Keyboard::Space)) && !ImGui::IsAnyItemActive()) {
			if(_playing) {
                _dialogAudio.pause();
                _playing = false;
            }
            else {
                _dialogAudio.play();
                _playing = true;
            }
        }

        if (_inputManager.isKept(InputCode(Keyboard::LControl))) {
            if (_inputManager.isPressed(InputCode(Keyboard::Add))) {
                addMinor();
            }
            if (_inputManager.isPressed(InputCode(Keyboard::Subtract))) {
                removeMinor();
            }

            if (_inputManager.isPressed(InputCode(Keyboard::Tab))) {
                if (_inputManager.isKept(InputCode(Keyboard::LShift))) { 
                    if (_currentMinor > 0) {
                        --_currentMinor;
                    }
                    else {
                        _currentMinor = _parts[_currentMajor].size() - 1;  
                    }
                }
                else if(_currentMinor < _parts[_currentMajor].size() - 1) {
                    ++_currentMinor;
                }
                else {
                    _currentMinor = 0;
                }
            }
        }
    }

    void DLGEditor::save()
    {
        _textManager.save(_dialogPath + ".dlg");
    }

    DLGEditor::TextContainer_t& DLGEditor::getContainer()
    {
        return _parts;
    }

    void DLGEditor::setCurrentDialog(int major, int minor)
    { 
        _currentMajor = major; 
        _currentMinor = minor;
    }

    void DLGEditor::show()
    {
        textWindow();

        ImGui::Begin("Dlg Files Editor");
            _isWindowFocused = ImGui::IsWindowFocused();
            renameWindow();

            player();              ImGui::Separator();
            majorPartSelector();   ImGui::Separator();
            minorPartSelector();   ImGui::Separator();
            labelEditor();         ImGui::Separator();
            timeSelector();        ImGui::Separator();
            mainEditor();
        ImGui::End();
    }

    void DLGEditor::renameWindow()
    {
        if (_renaming) {
            ImGui::BeginPopupModal("Rename", NULL, ImGuiWindowFlags_NoResize);
                ImGui::Text("Name: "); ImGui::SameLine();

                size_t size = _renamingName.length() + 100;
                char *newText = new char[size] {};
                strncpy(newText, _renamingName.c_str(), size);
                ImGui::PushItemWidth(300);
                    if (ImGui::InputText("##NewNameInput", newText, size)) {
                        _renamingName = newText;
                    }
                    if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
                        ImGui::SetKeyboardFocusHere(0);
                    }
                ImGui::PopItemWidth();
                delete[] newText;


                ImGui::SetCursorPosX(260);

                if(ImGui::Button("CANCEL##RenamingWindow")) {
                    _renaming = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine(); 

                if (ImGui::Button(" OK ##RenamingWindow")) {
                    rename();
                }

            ImGui::EndPopup();
        }
    }

    void DLGEditor::textWindow()
    {
        if (_isTextWindow)
        {   
            ImGui::Begin("Text##DLGPopupWindow");
                size_t size = _wrappedText.length() + 200;
                char *newText = new char[size] {};
                strncpy(newText, _wrappedText.c_str(), size);
                ImGui::InputTextMultiline("##WrappedText", newText, size, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 65));
                _wrappedText = newText;
                if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
                    ImGui::SetKeyboardFocusHere(0);
                }
                delete[] newText;

                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130);

                if(ImGui::Button("CANCEL##TextWindow")) {
                    _isTextWindow = false;
                }

                ImGui::SameLine(); 

                if (ImGui::Button(" OK ##TextWindow")) {
                    for(auto i = 0; i < _wrappedText.length(); ++i) {
                        if (_wrappedText[i] == '\n' && _wrappedText[i+1] != '\n') {
                            _wrappedText.erase(_wrappedText.begin() + i);
                        }
                    }
                    _parts[_currentMajor][_currentMinor]->dialogs[_currentDialogPart] = _wrappedText;
                    _isTextWindow = false;
                }
            ImGui::End();
        }
    }

    void DLGEditor::rename()
    {
        if (_renameType == Major) {
            for(auto& it : _parts[_renamingMajor]) {
                 it->renameMajor(_renamingName);
            }
        }
        else {
             _parts[_renamingMajor][_renamingMinor]->renameMinor(_renamingName);
        }
        _renaming = false;
        ImGui::CloseCurrentPopup();
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
        ImGui::PushItemWidth(300);
            if (majorsCombo()) {
                _currentMinor = 0;   
            }
            ImGui::SameLine();
        ImGui::PopItemWidth();
        
        if (ImGui::Button("Rename##MajorPartSelector", ImVec2(70, 23))) {
            _renaming = true;
            ImGui::OpenPopup("Rename");
            _renamingMajor = _currentMajor;
            _renamingName = _parts[_currentMajor][0]->majorName;
            _renameType = Major;
        } 
        ImGui::SameLine();

        if (auto winWidth = ImGui::GetWindowWidth(); winWidth > 553) {
            ImGui::SetCursorPosX(winWidth - 160);
        }

        if (ImGui::Button("Add##MajorPartSelector", ImVec2(70, 23))) {
            _currentMajor = _parts.size();
            _currentMinor = 0;
            _textManager.add(_currentMajor, 0, NULL);
        } 
        ImGui::SameLine();

        if (ImGui::Button("Remove##MajorPartSelector", ImVec2(70, 23)) && _parts.size() > 1) {
            if (_currentMajor == _parts.size() - 1) {
                --_currentMajor;
            }
            _textManager.remove(_currentMajor);
        }
    }


    void DLGEditor::minorPartSelector()
    {
        ImGui::PushItemWidth(300);
            minorsCombo();
            ImGui::SameLine();
        ImGui::PopItemWidth();

        if (ImGui::Button("Rename##MinorPartSelector", ImVec2(70, 23))) {
            _renaming = true;
            ImGui::OpenPopup("Rename");
            _renamingMajor = _currentMajor;
            _renamingMinor = _currentMinor;
            _renamingName = _parts[_currentMajor][_currentMinor]->minorName;
            _renameType = Minor;
        } 
        ImGui::SameLine();

        if (auto winWidth = ImGui::GetWindowWidth(); winWidth > 553) {
            ImGui::SetCursorPosX(winWidth - 160);
        }

        if (ImGui::Button("Add##MinorPartSelector", ImVec2(70, 23))) {
            addMinor();
        } 
        ImGui::SameLine();

        if (ImGui::Button("Remove##MinorPartSelector", ImVec2(70, 23))) {
            removeMinor();
        }

    }

    void DLGEditor::labelEditor()
    {    
        size_t size = _parts[_currentMajor][_currentMinor]->label.length() + 100;
        char *newText = new char[size] {};
        strncpy(newText, _parts[_currentMajor][_currentMinor]->label.c_str(), size);

        if (ImGui::InputText(" - Label##LabelNameInput", newText, size)) {
            _parts[_currentMajor][_currentMinor]->label = newText;
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
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 19); 
        if (ImGui::Button("##DialogTimeSelectorStart-FromSliderToEndTime", ImVec2(50, 10))) {
           _endTestPlayerOffset = _sliderTime;
           _parts[_currentMajor][_currentMinor]->audioEndTime = _sliderTimeString;
        }

        ImGui::PushItemWidth(50);
            if (ImGui::InputText("##DialogTimeSelectorStart", tempStartTime, 6, 1)) {
                std::string timeString = tempStartTime;
                checkTimeString(timeString);
                _startTestPlayerOffset = toIntSeconds(tempStartTime);
                if (_startTestPlayerOffset > 0) _parts[_currentMajor][_currentMinor]->audioStartTime = timeString;
                else _parts[_currentMajor][_currentMinor]->audioStartTime = "00:00";
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
        if (ImGui::Button("TEST##DialogTimeSelector", ImVec2(ImGui::GetWindowWidth() - 160, 36))) {
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
            size_t size = _wrappedText.length() + 200;
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
                if (ImGui::GetWindowWidth() - 348 > 0) {
                    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 348); 
                    if (ImGui::InputText("##DLGDialogTextInput", newText, size)) {
                        _parts[_currentMajor][_currentMinor]->dialogs[i] = newText;
                    }; 
                    delete[] newText;
                    ImGui::SameLine();
                }
                else {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6);
                }
                if (ImGui::Button("...", ImVec2(32, 23))) {
                    _isTextWindow = true;

                    auto temp = _parts[_currentMajor][_currentMinor]->dialogs[i];
                    std::string s;

                    for(auto index = 0; index < temp.length(); ++index) {
                        s += temp[index];
                        if(temp[index] == '\n') {
                            s += "\n";
                        }
                    }

                    auto wordlen = [](const char * str){
                        int tempindex=0;
                        while(str[tempindex]!=' ' && str[tempindex]!=0 && str[tempindex]!='\n'){
                            ++tempindex;
                        }
                        return(tempindex);
                    };

                    int index=0;
                    int curlinelen = 0;
                    while(index > s.length()){
                        if(s[index] == '\n'){
                            curlinelen=0;
                        }
                        else if(s[index] == ' ') {
                            if(curlinelen+wordlen(&s[index+1]) >= 45){
                                s[index] = '\n';
                                curlinelen = 0;
                            }
                        }
                        curlinelen++;
                        index++;
                    }
                    _wrappedText = s;
                    _currentDialogPart = i;
                }
                ImGui::SameLine();
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

    void DLGEditor::addMinor()
    {
        ++_currentMinor;
        _textManager.add(_currentMajor, _currentMinor, NULL);

        LOG_INFO("New minor part added");
    }

    void DLGEditor::removeMinor()
    {
        if(_parts[_currentMajor].size() > 1) {
            if (_currentMinor == _parts[_currentMajor].size() - 1) {
                --_currentMinor;
            }
            _textManager.remove(_currentMajor, _currentMinor);
        }

        LOG_INFO("Minor part has been removed");
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

    bool DLGEditor::majorsCombo()
    {
        return ImGui::Combo("##majorNamesCombo", &_currentMajor, [](void* vec, int idx, const char** outText) { 
                auto& vector = *((DLGEditor::TextContainer_t*)vec);
                if (idx < 0 || idx >= (int)(vector.size())) { 
                    return false; 
                }
                *outText = vector[idx][0]->majorFullName.c_str();
                return true;
            }, (void*)&_parts, _parts.size());
    }

    bool DLGEditor::minorsCombo()
    {
        return ImGui::Combo("##minorNamesCombo", &_currentMinor, [](void* vec, int idx, const char** outText) { 
                auto& vector = *((std::vector<std::shared_ptr<DialogData>>*)vec);
                if (idx < 0 || idx >= (int)(vector.size())) { 
                    return false; 
                }
                *outText = vector[idx]->minorFullName.c_str();
                return true;
            }, (void*)&_parts[_currentMajor], _parts[_currentMajor].size());
    }
}
