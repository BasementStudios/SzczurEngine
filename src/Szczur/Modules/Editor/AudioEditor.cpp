#include "AudioEditor.hpp"

#include <json.hpp>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include <experimental/filesystem>

namespace rat
{
	void PlaylistHolder::loadPlaylist()
	{
        Json j;

        auto currentPath = std::experimental::filesystem::current_path().string();
        auto path = FileDialog::getOpenFileName("", currentPath, "JSON files (*.json)|*.json");
        std::string filePath;
        size_t start = path.find(currentPath);
        if (start != -1) {
            filePath = path.substr(currentPath.length() + 7, path.length() - 5 - currentPath.length() - 7);
        } 
        else {
            filePath = path;
        }
        std::replace(filePath.begin(), filePath.end(), '\\', '/');

        std::ifstream file("music/" + filePath + ".json");
        if (file.is_open()) {
            file >> j;
        }
        file.close();

        for (auto it = j.begin(); it != j.end(); ++it) {
            if(std::find(std::begin(playlistsNames), std::end(playlistsNames), it.key()) == std::end(playlistsNames)) {
                music->addPlaylist(it.key(), it.value());
                playlistsNames.push_back(it.key());
            }
        }

        LOG_INFO("Playlist loaded from: ", filePath);
	}

    void PlaylistHolder::savePlaylists()
	{
        auto path = "music/";
        json j;

        for(auto& playlist : playlistsNames) {
            std::vector<std::string> songs;
            for(auto& it : music->getPlaylist(playlist)) {
                songs.push_back(it->getName());
            }
            j[playlist] = songs;
            std::ofstream file(path + playlist + ".json", std::ios::trunc);
            if (file.is_open()) {
                file << j;
            }
            file.close();
        }
	}

    void PlaylistHolder::addMusic(const std::string& playlistName)
	{
        auto currentPath = std::experimental::filesystem::current_path().string();
        auto path = FileDialog::getOpenFileName("", currentPath, "Music (*.flac)|*.flac");
        std::string filePath;
        size_t start = path.find(currentPath);
        if (start != -1) {
            filePath = path.substr(currentPath.length() + 7, path.length() - 5 - currentPath.length() - 7);
        } 
        else {
            filePath = path;
        }
        std::replace(filePath.begin(), filePath.end(), '\\', '/');
        music->addToPlaylist(playlistName, filePath);
	}

	AudioEditor::AudioEditor()
	{
		_soundEditor.init();
        _playlistHolder.music = &getModule<Music>();
        musicAssets = _playlistHolder.music->getAssetsManager();
	}

	void AudioEditor::render()
	{ 
		static std::string newPlaylistName;
		
		
		ImGui::Begin("Audio Editor");

            //Save everything
            if(ImGui::Button("Save")){
				_playlistHolder.savePlaylists();
			};

            ImGui::Separator();

			ImGui::Checkbox("Sound Editor", &_showSoundEditor);
			ImGui::Checkbox("Music Editor", &_showMusicEditor);
		ImGui::End();

		if(_showSoundEditor) {
			ImGui::Begin("Hello, world2!");
						
				ImGui::InputText("Path to file .flac",file,BUFFER);
				ImGui::InputText("Sound name",soundName,BUFFER);
				if(ImGui::Button("Add sound")){
					addSound(soundName,file);
				}   

				if(ImGui::Button("Save Sounds")){
					_soundEditor.save(saveFileName);
				}
				ImGui::SameLine();
				ImGui::InputText("Save Sounds",saveFileName,BUFFER);
				
				if(ImGui::Button("Load Sounds")){
					_soundEditor.load(loadFileName);
				}
				ImGui::SameLine();
				ImGui::InputText("Load sounds from file",loadFileName,BUFFER);
				
				ImGui::Checkbox("Delete Sounds",&deleteField);
				ImGui::SameLine(); 
				if(deleteField && ImGui::Button("Delete all sounds")){
					_soundEditor.clear();
				} 
			
			ImGui::End();
			_soundEditor.render();
		}

		if(_showMusicEditor) {
			ImGui::Begin("Music Editor");

                if(ImGui::Button("Save")){
					_playlistHolder.savePlaylists();
				}

                ImGui::Separator();
						
				if(ImGui::Button("Load Playlist")){
					_playlistHolder.loadPlaylist();
				}

				if(ImGui::Button("Add Playlist")){
					_addingPlaylist = true;
				}

                ImGui::Separator();

				if (ImGui::TreeNode("Playlists")) {
					for(auto playlist : _playlistHolder.playlistsNames) {
						if (ImGui::TreeNode(playlist.c_str())) {
                            for(auto it : _playlistHolder.music->getPlaylist(playlist)) {
                                if(ImGui::Button(it->getName().c_str())){
                                    currentEditingMusicFile = it->getName();
                                }
                                ImGui::SameLine();
                                auto name = " - ##" + playlist + it->getName();
                                if(ImGui::Button(name.c_str())){
                                    _playlistHolder.music->removeFromPlaylist(playlist, it->getName());
                                    ImGui::TreePop();
                                    ImGui::TreePop();
                                    ImGui::End();
                                    return;
                                }
                            }
                            if(ImGui::Button(" + ")){
                                _playlistHolder.addMusic(playlist);
                            }
                            ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			
			ImGui::End();

            ImGui::Begin("Music Editor ##Song", NULL);
                static int bpm = 60;
                static int fadeTime = 0;
                static float volume = 100;

                auto& song = musicAssets->get(currentEditingMusicFile);

                bpm = song._bpm;
                fadeTime = song._fadeTime;
                volume = song.getVolume();

                auto nameText = "Name: " + currentEditingMusicFile;
                ImGui::Text(nameText.c_str());
                ImGui::SameLine();
                if (ImGui::Button("SAVE##MUSIC")) {
                    song.saveToJson();
                }
                ImGui::Separator();
                if (ImGui::Button("PLAY##MUSIC")) {
                    song.play();
                }
                ImGui::SameLine();
                if (ImGui::Button("PAUSE##MUSIC")) {
                    song.pause();
                }
                ImGui::SameLine();
                if (ImGui::Button("STOP##MUSIC")) {
                    song.stop();
                }   
                ImGui::Separator();
                if(ImGui::InputInt("BPM", &bpm)) {
                    song._bpm = bpm;
                }
                if(ImGui::InputInt("Fade Time", &fadeTime)) {
                    song._fadeTime = fadeTime;
                }
                if(ImGui::SliderFloat(" Volume", &volume, 0, 100)) {
                    song.setVolume(volume);
                }
                ImGui::Separator();
            ImGui::End();
		}

		if(_addingPlaylist) {
			ImGui::Begin("AddPlaylist", NULL);
				ImGui::Text("Name: "); ImGui::SameLine();
				size_t size = newPlaylistName.length() + 100;
				char *newText = new char[size] {};
				strncpy(newText, newPlaylistName.c_str(), size);
				ImGui::PushItemWidth(300);
					if (ImGui::InputText("##NewNameInput", newText, size)) {
						newPlaylistName = newText;
					}
					if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
						ImGui::SetKeyboardFocusHere(0);
					}
				ImGui::PopItemWidth();
				delete[] newText;
				
                ImGui::SetCursorPosX(260);

				if(ImGui::Button("CANCEL##AddPlaylist")) {
					newPlaylistName = "";
					_addingPlaylist = false;
				}

				ImGui::SameLine(); 

				if (ImGui::Button(" OK ##AddPlaylist")) {
					_playlistHolder.addPlaylist(newPlaylistName);
					newPlaylistName = "";
					_addingPlaylist = false;
				}

			ImGui::End();
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