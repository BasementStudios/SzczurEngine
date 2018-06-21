#include "AudioEditor.hpp"

#include <Json/json.hpp>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include <experimental/filesystem>

namespace rat
{

	AudioEditor::AudioEditor()
        : _musicEditor(getModule<Music>())
    {
		_soundEditor.init();
	}

	void AudioEditor::render()
	{ 
		static std::string newPlaylistName;
		
		ImGui::Begin("Audio Editor");
			ImGui::Checkbox("Sound Editor", &_showSoundEditor);
			ImGui::Checkbox("Music Editor", &_showMusicEditor);
		ImGui::End();

		if (_showSoundEditor) {
			ImGui::Begin("Sound Manager");
						
				ImGui::InputText("Path to file .flac", file, BUFFER);
				ImGui::InputText("Sound name", soundName, BUFFER);
				if (ImGui::Button("Add sound")){
					addSound(soundName,file);
				}   

				if (ImGui::Button("Save Sounds")){
					_soundEditor.save(saveFileName);
				}
				ImGui::SameLine();
				ImGui::InputText("Save Sounds", saveFileName, BUFFER);
				
				if (ImGui::Button("Load Sounds")){
					_soundEditor.load(loadFileName);
				}
				ImGui::SameLine();
				ImGui::InputText("Load sounds from file", loadFileName, BUFFER);
				
				ImGui::Checkbox("Delete Sounds", &deleteField);
				ImGui::SameLine(); 
				if (deleteField && ImGui::Button("Delete all sounds")) {
					_soundEditor.clear();
				} 
			
			ImGui::End();
			_soundEditor.render();
		}

		if (_showMusicEditor) {
			_musicEditor.render();
		}

	}

	void AudioEditor::addSound(const std::string& soundName,const std::string& soundFileName)
	{
		_soundEditor.addSound(soundName,soundFileName);
	}

	void AudioEditor::save(const std::string& fileName)
	{
		_soundEditor.save(fileName);
	}
	
	void AudioEditor::load(const std::string& fileName)
	{
		_soundEditor.load(fileName);
	}
}
