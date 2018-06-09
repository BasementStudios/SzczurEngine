#include "AudioEditor.hpp"

#include <Json/json.hpp>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include <experimental/filesystem>

namespace rat
{
	void PlaylistHolder::loadPlaylist()
	{
        nlohmann::json j;

        auto currentPath = std::experimental::filesystem::current_path().string();
        auto path = FileDialog::getOpenFileName("", currentPath, "JSON files (*.json)|*.json");
        std::string filePath;
        size_t start = path.find(currentPath);
        if (start != -1) {
            filePath = path.substr(currentPath.length() + 24, path.length() - 5 - currentPath.length() - 24);
        } 
        else {
            filePath = path;
        }
        std::replace(filePath.begin(), filePath.end(), '\\', '/');

        std::ifstream file("Assets/Music/Playlists/" + filePath + ".json");
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
        auto path = "Assets/Music/Playlists/";

        for(auto& playlist : playlistsNames) {
            nlohmann::json j;
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
            filePath = path.substr(currentPath.length() + 14, path.length() - 5 - currentPath.length() - 14);
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
			ImGui::Checkbox("Sound Editor", &_showSoundEditor);
			ImGui::Checkbox("Music Editor", &_showMusicEditor);
		ImGui::End();

		if(_showSoundEditor) {
			ImGui::Begin("Sound Manager");
						
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
                        if (ImGui::SmallButton((" - ##RemovePlaylist" + playlist).c_str())) {
                            
                        }
                        ImGui::SameLine();
						if (ImGui::TreeNode(playlist.c_str())) {
                            for(auto it : _playlistHolder.music->getPlaylist(playlist)) {
                                if(ImGui::Button(it->getName().c_str())){
                                    currentEditingMusicFile = it->getName();
                                    currentPlaylist = playlist;
                                }
                                ImGui::SameLine();
                                auto name = "PLAY##" + playlist + it->getName();
                                if(ImGui::Button(name.c_str())){
                                    currentEditingMusicFile = it->getName();
                                    currentPlaylist = playlist;
                                    getModule<Music>().setPlayingMode(currentPlaylist, Music::PlayingMode::Random);
                                    getModule<Music>().play(currentPlaylist, currentEditingMusicFile);
                                    auto& song = musicAssets->get(currentEditingMusicFile);
                                    for (unsigned int i = 0; i < MAX_AUX_FOR_SOURCE; ++i) {
                                        if(song.effectsTypes[i] != AudioEffect::EffectType::None) {
                                            switch(song.effectsTypes[i]) {
                                                case AudioEffect::EffectType::Reverb:
                                                    song.getEffect<Reverb>().density(song.reverbData.density);
                                                    song.getEffect<Reverb>().diffusion(song.reverbData.diffusion);
                                                    song.getEffect<Reverb>().gain(song.reverbData.gain);
                                                    song.getEffect<Reverb>().gainHf(song.reverbData.gainHf);
                                                    song.getEffect<Reverb>().decayTime(song.reverbData.decayTime);
                                                    song.getEffect<Reverb>().decayHfRatio(song.reverbData.decayHfRatio);
                                                    song.getEffect<Reverb>().reflectionsGain(song.reverbData.reflectionsGain);
                                                    song.getEffect<Reverb>().reflectionsDelay(song.reverbData.reflectionsDelay);
                                                    song.getEffect<Reverb>().lateReverbGain(song.reverbData.lateReverbGain);
                                                    song.getEffect<Reverb>().lateReverbDelay(song.reverbData.lateReverbDelay);
                                                    song.getEffect<Reverb>().airAbsorptionGainHf(song.reverbData.airAbsorptionGainHf);
                                                    song.getEffect<Reverb>().roomRolloffFactor(song.reverbData.roomRolloffFactor);
                                                    song.getEffect<Reverb>().decayHfLimit(song.reverbData.decayHfLimit);
                                                    break;
                                                case AudioEffect::EffectType::Echo:
                                                    song.getEffect<Echo>().delay(song.echoData.delay);
                                                    song.getEffect<Echo>().lrDelay(song.echoData.lrDelay);
                                                    song.getEffect<Echo>().damping(song.echoData.damping);
                                                    song.getEffect<Echo>().feedback(song.echoData.feedback);
                                                    song.getEffect<Echo>().spread(song.echoData.spread);
                                                    break;
                                                case AudioEffect::EffectType::Equalizer: 
                                                    song.getEffect<Equalizer>().lowGain(song.eqData.lowGain);
                                                    song.getEffect<Equalizer>().lowCutoff(song.eqData.lowCutoff);
                                                    song.getEffect<Equalizer>().lowMidGain(song.eqData.lowMidGain);
                                                    song.getEffect<Equalizer>().lowMidCenter(song.eqData.lowMidCenter);
                                                    song.getEffect<Equalizer>().lowMidWidth(song.eqData.lowMidWidth);
                                                    song.getEffect<Equalizer>().highMidGain(song.eqData.highMidGain);
                                                    song.getEffect<Equalizer>().highMidCenter(song.eqData.highMidCenter);
                                                    song.getEffect<Equalizer>().highMidWidth(song.eqData.highMidWidth);
                                                    song.getEffect<Equalizer>().highGain(song.eqData.highGain);
                                                    song.getEffect<Equalizer>().highCutoff(song.eqData.highCutoff);
                                                    break;
                                                default: 
                                                    break;
                                            }
                                        }
                                    }
                                }
                                ImGui::SameLine();
                                name = " - ##" + playlist + it->getName();
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

                static int bpm;
                static int fadeTime;
                static float volume = 100;

                EqualizerData eqData;
                ReverbData    reverbData;
                EchoData      echoData;

                //TODO: fix: if music didn't loaded and volume changed it crashes
                auto& song = musicAssets->get(currentEditingMusicFile);

                bpm = static_cast<int>(song._bpm);
                fadeTime = static_cast<int>(song._fadeTime);
                volume = song.getVolume();

                auto nameText = "Name: " + currentEditingMusicFile;
                ImGui::Text(nameText.c_str());

                ImGui::SameLine();

                if (ImGui::Button("SAVE##MUSIC")) {
                    song.saveToJson();
                }

                ImGui::Separator();

                if (ImGui::Button("PLAY##MUSIC")) {
                    getModule<Music>().setPlayingMode(currentPlaylist, Music::PlayingMode::Single);
                    getModule<Music>().play(currentPlaylist, currentEditingMusicFile);
                }
                ImGui::SameLine();
                if (ImGui::Button("PAUSE##MUSIC")) {
                    getModule<Music>().pause();
                }
                
                ImGui::SameLine();
                if (ImGui::Button("STOP##MUSIC")) {
                    getModule<Music>().stop();
                }   

                ImGui::Separator();

                if (ImGui::InputInt("BPM", &bpm)) {
                    song._bpm = bpm;
                }
                if (ImGui::InputInt("Fade Time", &fadeTime)) {
                    song._fadeTime = fadeTime;
                }

                if(ImGui::SliderFloat("Volume", &volume, 0, 100)) {
                    getModule<Music>().setVolume(currentPlaylist, volume, currentEditingMusicFile);
                }
                
                for(auto i = 0; i < MAX_AUX_FOR_SOURCE; ++i) {
                    if(song.effectsTypes[i] != AudioEffect::EffectType::None) {
                        ImGui::Separator();
                        switch(song.effectsTypes[i]) {
                            case AudioEffect::EffectType::Reverb:
                                ImGui::Text("Reverb:");
                                ImGui::SameLine();
                                if (ImGui::Button("Clean##EffectTypeReverb")) {
                                    song.reverbData.reset();
                                    song.cleanEffect<Reverb>();
                                    song.effectsTypes[i] = AudioEffect::EffectType::None;
                                }   
                                if (ImGui::SliderFloat("Density##R", &song.reverbData.density, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().density(song.reverbData.density);
                                }                
                                if (ImGui::SliderFloat("Diffusion##R", &song.reverbData.diffusion, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().diffusion(song.reverbData.diffusion);
                                }
                                if (ImGui::SliderFloat("Gain##R", &song.reverbData.gain, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().gain(song.reverbData.gain);
                                }
                                if (ImGui::SliderFloat("Gain Hf##R", &song.reverbData.gainHf, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().gainHf(song.reverbData.gainHf);
                                }
                                if (ImGui::SliderFloat("Decay Time##R", &song.reverbData.decayTime, 0.0, 20.0)) {
                                    song.getEffect<Reverb>().decayTime(song.reverbData.decayTime);
                                }
                                if (ImGui::SliderFloat("Decay HF Ratio##R", &song.reverbData.decayHfRatio, 0.0, 2.0)) {
                                    song.getEffect<Reverb>().decayHfRatio(song.reverbData.decayHfRatio);
                                }
                                if (ImGui::SliderFloat("Reflections Gain##R", &song.reverbData.reflectionsGain, 0.0, 3.16)) {
                                    song.getEffect<Reverb>().reflectionsGain(song.reverbData.reflectionsGain);
                                }
                                if (ImGui::SliderFloat("Reflections Delay##R", &song.reverbData.reflectionsDelay, 0.0, 0.3)) {
                                    song.getEffect<Reverb>().reflectionsDelay(song.reverbData.reflectionsDelay);
                                }
                                if (ImGui::SliderFloat("Late Reverb Gain##R", &song.reverbData.lateReverbGain, 0.0, 10.0)) {
                                    song.getEffect<Reverb>().lateReverbGain(song.reverbData.lateReverbGain);
                                }
                                if (ImGui::SliderFloat("Late Reverb Delay##R", &song.reverbData.lateReverbDelay, 0.0, 0.1)) {
                                    song.getEffect<Reverb>().lateReverbDelay(song.reverbData.lateReverbDelay);
                                }    
                                if (ImGui::SliderFloat("Air Absorption Gain HF##R", &song.reverbData.airAbsorptionGainHf, 0.892, 1.0)) {
                                    song.getEffect<Reverb>().airAbsorptionGainHf(song.reverbData.airAbsorptionGainHf);
                                }                   
                                if (ImGui::SliderFloat("Room Rolloff Factor##R", &song.reverbData.roomRolloffFactor, 0.0, 10.0)) {
                                    song.getEffect<Reverb>().roomRolloffFactor(song.reverbData.roomRolloffFactor);
                                }                           
                                if (ImGui::SliderInt("Decay HF Limit##R", (int*)&song.reverbData.decayHfLimit, 0, 1)) {
                                    song.getEffect<Reverb>().decayHfLimit(song.reverbData.decayHfLimit);
                                }
                                break;
                            case AudioEffect::EffectType::Echo:    
                                ImGui::Text("Echo:");
                                ImGui::SameLine();
                                if (ImGui::Button("Clean##EffectTypeEcho##E")) {
                                    song.echoData.reset();
                                    song.cleanEffect<Echo>();
                                    song.effectsTypes[i] = AudioEffect::EffectType::None;
                                } 
                                if (ImGui::SliderFloat("Delay##E", &song.echoData.delay, 0.0, 0.207)) {
                                    song.getEffect<Echo>().delay(song.echoData.delay);
                                }
                                if (ImGui::SliderFloat("LR Delay##E", &song.echoData.lrDelay, 0.0, 0.404)) {
                                    song.getEffect<Echo>().lrDelay(song.echoData.lrDelay);
                                }
                                if (ImGui::SliderFloat("Damping##E", &song.echoData.damping, 0.0, 0.99)) {
                                    song.getEffect<Echo>().damping(song.echoData.damping);
                                }                                       
                                if (ImGui::SliderFloat("Feedback##E", &song.echoData.feedback, 0.0, 1.0)) {
                                    song.getEffect<Echo>().feedback(song.echoData.feedback);
                                }
                                if (ImGui::SliderFloat("Spread##E", &song.echoData.spread, -1.0, 1.0)) {
                                    song.getEffect<Echo>().spread(song.echoData.spread);
                                }          
                                break;
                            case AudioEffect::EffectType::Equalizer:
                                ImGui::Text("Equalizer:");
                                ImGui::SameLine();
                                if (ImGui::Button("Clean##EffectTypeEqualizer")) {
                                    song.eqData.reset();
                                    song.cleanEffect<Equalizer>();
                                    song.effectsTypes[i] = AudioEffect::EffectType::None;
                                } 
                                ImGui::Text("LOW-SHELF:");
                                if (ImGui::SliderFloat("Gain##EL", &song.eqData.lowGain, 0.126, 10)) {
                                    song.getEffect<Equalizer>().lowGain(song.eqData.lowGain);
                                }
                                if (ImGui::SliderFloat("Cutoff##EL", &song.eqData.lowCutoff, 50, 8000)) {
                                    song.getEffect<Equalizer>().lowCutoff(song.eqData.lowCutoff);
                                }
                                ImGui::Text("LOW MID BAND:");
                                if (ImGui::SliderFloat("Gain##ELM", &song.eqData.lowMidGain, 0.126, 7.943)) {
                                    song.getEffect<Equalizer>().lowMidGain(song.eqData.lowMidGain);
                                }
                                if (ImGui::SliderFloat("Center##ELM", &song.eqData.lowMidCenter, 200, 3000)) {
                                    song.getEffect<Equalizer>().lowMidCenter(song.eqData.lowMidCenter);
                                }
                                if (ImGui::SliderFloat("Witdh##ELM", &song.eqData.lowMidWidth, 0.1, 1)) {
                                    song.getEffect<Equalizer>().lowMidWidth(song.eqData.lowMidWidth);
                                }		
                                ImGui::Text("HIGH MID BAND:");
                                if (ImGui::SliderFloat("Gain##ELM", &song.eqData.highMidGain, 0.126, 7.943)) {
                                    song.getEffect<Equalizer>().highMidGain(song.eqData.highMidGain);
                                }
                                if (ImGui::SliderFloat("Center##ELM", &song.eqData.highMidCenter, 1000, 8000)) {
                                    song.getEffect<Equalizer>().highMidCenter(song.eqData.highMidCenter);
                                }
                                if (ImGui::SliderFloat("Witdh##ELM", &song.eqData.highMidWidth, 0.1, 1)) {
                                    song.getEffect<Equalizer>().highMidWidth(song.eqData.highMidWidth);
                                }
                                ImGui::Text("HIGH-SHELF:");
                                if (ImGui::SliderFloat("Gain##EH", &song.eqData.highGain, 0.126, 7.943)) {
                                    song.getEffect<Equalizer>().highGain(song.eqData.highGain);
                                }
                                if (ImGui::SliderFloat("Cutoff##EH", &song.eqData.highCutoff, 4000, 16000)) {
                                    song.getEffect<Equalizer>().highCutoff(song.eqData.highCutoff);
                                }
                                break;
                            default: 
                                break;
                        }
                    }
                }

                ImGui::Separator();

                if(song.lastFreeSlot() > -1) {
                    if (ImGui::Button("Add Effect##MUSIC")) {
                        _addingEffect = true;
                    } 
                }

            ImGui::End();
		}

		if(_addingPlaylist) {
			ImGui::Begin("AddPlaylist", NULL);

				ImGui::Text("Name: "); 
                ImGui::SameLine();

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

        if(_addingEffect) {
            static int element = 0;

			ImGui::Begin("AddEffect", NULL);

                ImGui::Combo("Effect##AddEffect", &element, "Reverb\0Echo\0Equalizer\0", 3);
				
                ImGui::SetCursorPosX(200);

				if(ImGui::Button("CANCEL##AddEffect")) {
					_addingEffect = false;
                    element = 0;
				}

				ImGui::SameLine(); 

				if (ImGui::Button(" OK ##AddEffect")) {
                    auto& song = musicAssets->get(currentEditingMusicFile);

                    auto isUsing = [&](AudioEffect::EffectType type) -> bool {
                        for (unsigned int i = 0; i < MAX_AUX_FOR_SOURCE; ++i) {
                            if (song.effectsTypes[i] == type) {
                                return true;
                            }
                        }
                        return false;
                    };

                    auto set = [&](AudioEffect::EffectType type) {
                        if(!isUsing(type)) {
                            song.effectsTypes[song.lastFreeSlot()] = type;
                            song.getEffect<Reverb>();
                        }
                    };

                    switch(element) {
                        case 0:
                            set(AudioEffect::EffectType::Reverb);
                            break;
                        case 1:
                            set(AudioEffect::EffectType::Echo);
                            break;
                        case 2:
                            set(AudioEffect::EffectType::Equalizer);
                            break;
                        default: 
                            break;
                    }
					_addingEffect = false;
                    element = 0;
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
