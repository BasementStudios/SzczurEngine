#include "SoundEditor.hpp"

#include <cmath>
#include <fstream>

#include <experimental/filesystem>
#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Sound/SoundAssets.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

#include <nlohmann/json.hpp>

namespace rat
{
    SoundEditor::SoundEditor(Sound& sound, bool& showingEditor)
        : _sound(sound), _assets(sound.getAssetsManager()), _isListDisplayed(showingEditor)
    {
        _currentEditing = _soundHolder.end();
    }

    void SoundEditor::render()
    {		
		ImGui::Begin("Sounds List", &_isListDisplayed);

		float width = ImGui::GetWindowContentRegionWidth() * 0.33;

		if (ImGui::Button("Save##SoundLists", { width,0 })) {
			for (auto it = _soundHolder.begin(); it != _soundHolder.end(); ++it) {
				save(it);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Load##SoundLists", { width,0 })) {
			_isLoadingDisplayed = true;
			_soundNames = std::move(_assets.getSoundNames());
		}

		ImGui::SameLine();

		if (ImGui::Button("Add##SoundLists", { width ,0 })) {
			add();
		}

		if (ImGui::CollapsingHeader("Sounds list##SoundLists"))
		{
			size_t index = 1;
			for (auto it = _soundHolder.begin(); it != _soundHolder.end();) {
				std::string name = it->getName();

				ImGui::Separator();

				ImGui::Bullet();

				ImGui::SameLine();
				ImGui::Text(name.c_str());
				
				ImGui::SameLine();
				ImGui::PushID(index * 1);
				if (ImGui::SmallButton("Open editor")) {
					if (_currentEditing != _soundHolder.end() && _currentEditing->getName() != name)
						_sound.stop();
					_currentEditing = it;
					_isEditorDisplayed = true;
				}
				ImGui::PopID();

				ImGui::PushID(index);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV( 2.f / 7.f, 0.6f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.f / 7.f, 0.7f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.f / 7.f, 0.8f, 0.7f));
				if (ImGui::SmallButton("Save")) {
					save(it);
				}
				ImGui::PopStyleColor(3);
				ImGui::PopID();

				ImGui::PushID(index);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.7f));
				if (ImGui::SmallButton("Delete")) {
					if (_currentEditing != _soundHolder.end() && _currentEditing->getName() == name) {
						_currentEditing = _soundHolder.end();
						_isEditorDisplayed = false;
					}
					_soundHolder.erase(it++);	
				}
				else{
					++it; //dealing with skiping iterator if we delete element from list
				}
				ImGui::PopStyleColor(3);
				ImGui::PopID();
				index++;
			}
		}

		ImGui::End();

		if (_isEditorDisplayed)
			showEditor();

		if (_isLoadingDisplayed)
			load();
    }

	void SoundEditor::showEditor() {
		ImGui::Begin("Sound Editor", &_isEditorDisplayed, ImGuiWindowFlags_AlwaysAutoResize);

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
			if (minDistance > 0) {
				_currentEditing->setMinDistance(minDistance);
			}
		}

		ImGui::Separator();


		ImGui::End();
	}

    void SoundEditor::save(Container_t::iterator it)
    {
        auto name = it->getName();

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
		if (!ImGui::Begin("Load Sound", &_isLoadingDisplayed))
		{
			ImGui::End();
		}
		else {
			static ImGuiTextFilter filter;
			//LOG_INFO(filter.InputBuf);
			filter.Draw();
			for (int i = 0; i < _soundNames.size(); i++) {
				if (filter.PassFilter(_soundNames[i].c_str())) {
					ImGui::Bullet();
					ImGui::SameLine();
					if (ImGui::Button(_soundNames[i].c_str()) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
						//load the file
						_soundHolder.push_back(SoundBase(_assets, _soundNames[i]));
						_soundHolder.back().load();

						_assets.load(_soundHolder.back().getFileName());
						_soundHolder.back().setBuffer(_assets.get(_soundHolder.back().getFileName()));
						_soundHolder.back().init();

						_isLoadingDisplayed = false;
						break;
					}
				}
			}
			ImGui::End();
		}

        /*static std::string loadingSoundName = "";

		if (!ImGui::Begin("Load Sound", &_isLoadingDisplayed))
		{
			ImGui::End();
		}
		else {
			ImGui::Text("Name: ");
			ImGui::SameLine();

			size_t size = loadingSoundName.length() + 100;
			char *newText = new char[size] {};
			strncpy(newText, loadingSoundName.c_str(), size);

			ImGui::PushItemWidth(300);
			if (ImGui::InputText("##LoadingSoundNameInput", newText, size)) {
				loadingSoundName = newText;
			}
			if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
				ImGui::SetKeyboardFocusHere(0);
			}
			ImGui::PopItemWidth();

			delete[] newText;

			ImGui::SetCursorPosX(260);

			if (ImGui::Button("CANCEL##LoadSound")) {
				loadingSoundName = "";
				_isLoadingDisplayed = false;
			}

			ImGui::SameLine();

			if (ImGui::Button(" OK ##LoadSound")) {

				_soundHolder.push_back(SoundBase(_assets, loadingSoundName));
				_soundHolder.back().load();
				
				auto fileName = _soundHolder.back().getFileName();

				if (fileName.empty()) {
					_soundHolder.pop_back();
				}
				else {
					_assets.load(fileName);
					_soundHolder.back().setBuffer(_assets.get(fileName));
					_soundHolder.back().init();
				}


				loadingSoundName = "";
				_isLoadingDisplayed = false;
			}

			ImGui::End();
		}*/
    
    }

    void SoundEditor::add()
    {
        auto currentPath = std::experimental::filesystem::current_path().string();
        auto path = FileDialog::getOpenFileName("", currentPath, "Sound files (*.flac)|*.flac");
        std::string filePath;
        
        if (path.find(currentPath) != std::string::npos && !path.empty()) {
            filePath = path.substr(currentPath.length() + 1, path.length());
            std::replace(filePath.begin(), filePath.end(), '\\', '/');

            _soundHolder.push_back(SoundBase(_assets, "Unnnamed"));
            _assets.load(filePath);
            _soundHolder.back().setBuffer(_assets.get(filePath));
            _soundHolder.back().init();
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