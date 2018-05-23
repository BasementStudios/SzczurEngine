#include "AudioEditor.hpp"

#include <json.hpp>

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
            filePath = path.substr(currentPath.length() + 14, path.length() - 5 - currentPath.length() - 14);
        } 
        else {
            filePath = path;
        }
        std::replace(filePath.begin(), filePath.end(), '\\', '/');

        std::ifstream file("Assets/Music/" + filePath + ".json");
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
        auto path = "Assets/Music/";

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
                                                    song.getEffect<Reverb>().density(song.DEN);
                                                    song.getEffect<Reverb>().diffusion(song.DIFF);
                                                    song.getEffect<Reverb>().gain(song.G);
                                                    song.getEffect<Reverb>().gainHf(song.RGH);
                                                    song.getEffect<Reverb>().decayTime(song.DT);
                                                    song.getEffect<Reverb>().decayHfRatio(song.DHF);
                                                    song.getEffect<Reverb>().reflectionsGain(song.RG);
                                                    song.getEffect<Reverb>().reflectionsDelay(song.RD);
                                                    song.getEffect<Reverb>().lateReverbGain(song.LRG);
                                                    song.getEffect<Reverb>().lateReverbDelay(song.LRD);
                                                    song.getEffect<Reverb>().airAbsorptionGainHf(song.AAGH);
                                                    song.getEffect<Reverb>().roomRolloffFactor(song.RRF);
                                                    song.getEffect<Reverb>().decayHfLimit(song.DHL);
                                                    break;
                                                case AudioEffect::EffectType::Echo:
                                                    song.getEffect<Echo>().delay(song.DL);
                                                    song.getEffect<Echo>().lrDelay(song.LDL);
                                                    song.getEffect<Echo>().damping(song.DG);
                                                    song.getEffect<Echo>().feedback(song.FB);
                                                    song.getEffect<Echo>().spread(song.SP);
                                                    break;
                                                case AudioEffect::EffectType::Equalizer: 
                                                    song.getEffect<Equalizer>().lowGain(song.LG);
                                                    song.getEffect<Equalizer>().lowCutoff(song.LC);
                                                    song.getEffect<Equalizer>().lowMidGain(song.M1G);
                                                    song.getEffect<Equalizer>().lowMidCenter(song.M1C);
                                                    song.getEffect<Equalizer>().lowMidWidth(song.M1W);
                                                    song.getEffect<Equalizer>().highMidGain(song.M2G);
                                                    song.getEffect<Equalizer>().highMidCenter(song.M2C);
                                                    song.getEffect<Equalizer>().highMidWidth(song.M2W);
                                                    song.getEffect<Equalizer>().highGain(song.HG);
                                                    song.getEffect<Equalizer>().highCutoff(song.HC);
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
                static int bpm = 60;
                static int fadeTime = 0;
                static float volume = 100;
                static float LC = 200.0;
                static float LG = 1.0;
                static float HC = 6000.0;
                static float HG = 1.0;
                static float M1C = 500.0;
                static float M1W = 1.0;
                static float M1G = 1.0;
                static float M2C = 3000.0;
                static float M2W = 1.0;
                static float M2G = 1.0;
                static float DL = 0.1;
                static float LDL = 0.1;
                static float DG = 0.5;
                static float FB = 0.5;
                static float SP = -1.0;
                static float DEN = 1.0;
                static float DIFF = 1.0;
                static float G = 0.32;
                static float RGH = 0.89;
                static float DT = 1.49;
                static float DHF = 0.83;
                static float RG = 0.05;
                static float RD = 0.007;
                static float LRG = 1.26;
                static float LRD = 0.011;
                static float AAGH = 0.994;
                static float RRF = 0.0;
                static int DHL = 1;

                auto& song = musicAssets->get(currentEditingMusicFile);

                bpm = song._bpm;
                fadeTime = song._fadeTime;
                volume = song.getVolume();
                LC = song.LC;
                LG = song.LG;
                HC = song.HC;
                HG = song.HG;
                M1C = song.M1C;
                M1W = song.M1W;
                M1G = song.M1G;
                M2C = song.M2C;
                M2W = song.M2W;
                M2G = song.M2G;
                DL = song.DL;
                LDL = song.LDL;
                DG = song.DG;
                FB = song.FB;
                SP = song.SP;
                DEN = song.DEN;
                DIFF = song.DIFF;
                G = song.G;
                RGH = song.RGH;
                DT = song.DT;
                DHF = song.DHF;
                RG = song.RG;
                RD = song.RD;
                LRG = song.LRG;
                LRD = song.LRD;
                AAGH = song.AAGH;
                RRF = song.RRF;
                DHL = song.DHL;

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
                if(ImGui::InputInt("BPM", &bpm)) {
                    song._bpm = bpm;
                }
                if(ImGui::InputInt("Fade Time", &fadeTime)) {
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
                                    song.DEN = 1.0;
                                    song.DIFF = 1.0;
                                    song.G = 0.32;
                                    song.RGH = 0.89;
                                    song.DT = 1.49;
                                    song.DHF = 0.83;
                                    song.RG = 0.05;
                                    song.RD = 0.007;
                                    song.LRG = 1.26;
                                    song.LRD = 0.011;
                                    song.AAGH = 0.994;
                                    song.RRF = 0.0;
                                    song.DHL = 1;
                                    song.cleanEffect<Reverb>();
                                    song.effectsTypes[i] = AudioEffect::EffectType::None;
                                }   
                                if (ImGui::SliderFloat("Density##R", &DEN, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().density(DEN);
                                    song.DEN = DEN;
                                }                
                                if (ImGui::SliderFloat("Diffusion##R", &DIFF, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().diffusion(DIFF);
                                    song.DIFF = DIFF;
                                }
                                if (ImGui::SliderFloat("Gain##R", &G, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().gain(G);
                                    song.G = G;
                                }
                                if (ImGui::SliderFloat("Gain Hf##R", &RGH, 0.0, 1.0)) {
                                    song.getEffect<Reverb>().gainHf(RGH);
                                    song.RGH = RGH;
                                }
                                if (ImGui::SliderFloat("Decay Time##R", &DT, 0.0, 20.0)) {
                                    song.getEffect<Reverb>().decayTime(DT);
                                    song.DT = DT;
                                }
                                if (ImGui::SliderFloat("Decay HF Ratio##R", &DHF, 0.0, 2.0)) {
                                    song.getEffect<Reverb>().decayHfRatio(DHF);
                                    song.DHF = DHF;
                                }
                                if (ImGui::SliderFloat("Reflections Gain##R", &RG, 0.0, 3.16)) {
                                    song.getEffect<Reverb>().reflectionsGain(RG);
                                    song.RG = RG;
                                }
                                if (ImGui::SliderFloat("Reflections Delay##R", &RD, 0.0, 0.3)) {
                                    song.getEffect<Reverb>().reflectionsDelay(RD);
                                    song.RD = RD;
                                }
                                if (ImGui::SliderFloat("Late Reverb Gain##R", &LRG, 0.0, 10.0)) {
                                    song.getEffect<Reverb>().lateReverbGain(LRG);
                                    song.LRG = LRG;
                                }
                                if (ImGui::SliderFloat("Late Reverb Delay##R", &LRD, 0.0, 0.1)) {
                                    song.getEffect<Reverb>().lateReverbDelay(LRD);
                                    song.LRD = LRD;
                                }    
                                if (ImGui::SliderFloat("Air Absorption Gain HF##R", &AAGH, 0.892, 1.0)) {
                                    song.getEffect<Reverb>().airAbsorptionGainHf(AAGH);
                                    song.AAGH = AAGH;
                                }                   
                                if (ImGui::SliderFloat("Room Rolloff Factor##R", &RRF, 0.0, 10.0)) {
                                    song.getEffect<Reverb>().roomRolloffFactor(RRF);
                                    song.RRF = RRF;
                                }                           
                                if (ImGui::SliderInt("Decay HF Limitv", &DHL, 0, 1)) {
                                    song.getEffect<Reverb>().decayHfLimit(DHL);
                                    song.DHL = DHL;
                                }
                                break;
                            case AudioEffect::EffectType::Echo:    
                                ImGui::Text("Echo:");
                                ImGui::SameLine();
                                if (ImGui::Button("Clean##EffectTypeEcho##E")) {
                                    song.DL = 0.1;
                                    song.LDL = 0.1;
                                    song.DG = 0.5;
                                    song.FB = 0.5;
                                    song.SP = -1.0;
                                    song.cleanEffect<Echo>();
                                    song.effectsTypes[i] = AudioEffect::EffectType::None;
                                } 
                                if (ImGui::SliderFloat("Delay##E", &DL, 0.0, 0.207)) {
                                    song.getEffect<Echo>().delay(DL);
                                    song.DL = DL;
                                }
                                if (ImGui::SliderFloat("LR Delay##E", &LDL, 0.0, 0.404)) {
                                    song.getEffect<Echo>().lrDelay(LDL);
                                    song.LDL = LDL;
                                }
                                if (ImGui::SliderFloat("Damping##E", &DG, 0.0, 0.99)) {
                                    song.getEffect<Echo>().damping(DG);
                                    song.DG = DG;
                                }                                       
                                if (ImGui::SliderFloat("Feedback##E", &FB, 0.0, 1.0)) {
                                    song.getEffect<Echo>().feedback(FB);
                                    song.FB = FB;
                                }
                                if (ImGui::SliderFloat("Spread##E", &SP, -1.0, 1.0)) {
                                    song.getEffect<Echo>().spread(SP);
                                    song.SP = SP;
                                }          
                                break;
                            case AudioEffect::EffectType::Equalizer:
                                ImGui::Text("Equalizer:");
                                ImGui::SameLine();
                                if (ImGui::Button("Clean##EffectTypeEqualizer")) {
                                    song.LC = 200.0;
                                    song.LG = 1.0;
                                    song.HC = 6000.0;
                                    song.HG = 1.0;
                                    song.M1C = 500.0;
                                    song.M1W = 1.0;
                                    song.M1G = 1.0;
                                    song.M2C = 3000.0;
                                    song.M2W = 1.0;
                                    song.M2G = 1.0;
                                    song.cleanEffect<Equalizer>();
                                    song.effectsTypes[i] = AudioEffect::EffectType::None;
                                } 
                                ImGui::Text("LOW-SHELF:");
                                if (ImGui::SliderFloat("Gain##EL", &LG, 0.126, 10)) {
                                    song.getEffect<Equalizer>().lowGain(LG);
                                    song.LG = LG;
                                }
                                if (ImGui::SliderFloat("Cutoff##EL", &LC, 50, 8000)) {
                                    song.getEffect<Equalizer>().lowCutoff(LC);
                                    song.LC = LC;
                                }
                                ImGui::Text("LOW MID BAND:");
                                if (ImGui::SliderFloat("Gain##ELM", &M1G, 0.126, 7.943)) {
                                    song.getEffect<Equalizer>().lowMidGain(M1G);
                                    song.M1G = M1G;
                                }
                                if (ImGui::SliderFloat("Center##ELM", &M1C, 200, 3000)) {
                                    song.getEffect<Equalizer>().lowMidCenter(M1C);
                                    song.M1C = M1C;
                                }
                                if (ImGui::SliderFloat("Witdh##ELM", &M1W, 0.1, 1)) {
                                    song.getEffect<Equalizer>().lowMidWidth(M1W);
                                    song.M1W = M1W;
                                }		
                                ImGui::Text("HIGH MID BAND:");
                                if (ImGui::SliderFloat("Gain##ELH", &M2G, 0.126, 7.943)) {
                                    song.getEffect<Equalizer>().highMidGain(M2G);
                                    song.M2G = M2G;
                                }
                                if (ImGui::SliderFloat("Center##ELH", &M2C, 1000, 8000)) {
                                    song.getEffect<Equalizer>().highMidCenter(M2C);
                                    song.M2C = M2C;
                                }
                                if (ImGui::SliderFloat("Witdh##ELH", &M2W, 0.1, 1)) {
                                    song.getEffect<Equalizer>().highMidWidth(M2W);
                                    song.M2W = M2W;
                                }
                                ImGui::Text("HIGH-SHELF:");
                                if (ImGui::SliderFloat("Gain##EH", &HG, 0.126, 7.943)) {
                                    song.getEffect<Equalizer>().highGain(HG);
                                    song.HG = HG;
                                }
                                if (ImGui::SliderFloat("Cutoff##EH", &HC, 4000, 16000)) {
                                    song.getEffect<Equalizer>().highCutoff(HC);
                                    song.HC = HC;
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