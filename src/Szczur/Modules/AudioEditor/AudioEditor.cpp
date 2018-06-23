#include "AudioEditor.hpp"

#include <Json/json.hpp>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include <experimental/filesystem>

#include <ImGui/imgui.h>

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
                getModule<Music>().setVolume(_globalVolume);
				getModule<SoundManager>().setVolume(_globalVolume);
            }
		ImGui::End();

		if (_showSoundEditor)
			_soundEditor.render();

		if (_showMusicEditor)
			_musicEditor.render();			
	}

}
