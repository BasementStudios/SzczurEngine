#include "AudioEditor.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include <experimental/filesystem>

#include <nlohmann/json.hpp>
#include <imgui.h>

namespace rat
{

	AudioEditor::AudioEditor()
        : _musicEditor(getModule<Music>()), _soundEditor(getModule<SoundManager>())
    {
		
	}

	void AudioEditor::render()
	{ 
		ImGui::Begin("Audio Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Checkbox("Sound Editor", &_showSoundEditor);
			ImGui::Checkbox("Music Editor", &_showMusicEditor);
			ImGui::Separator();
			if (ImGui::SliderFloat("Global Volume", &_globalVolume, 0, 100)) {
                getModule<Listener>().setGlobalVolume(_globalVolume);
            }
		ImGui::End();

		if (_showSoundEditor)
			_soundEditor.render();

		if (_showMusicEditor)
			_musicEditor.render();			
	}

}
