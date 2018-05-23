#pragma once
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Sound/SoundManager.hpp"
#include "Szczur/Modules/Music/Music.hpp"
#include "SoundEditor.hpp"


#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>
#include <fstream>
namespace rat
{
    struct PlaylistHolder 
    {
        std::vector<std::string> playlistsNames;
        
        Music* music;

        void savePlaylists();
        void loadPlaylist();
        void addMusic(const std::string& playlistName);

        void addPlaylist(const std::string& name) {
            playlistsNames.push_back(name);
            music->addPlaylist(name, {});  
        };
    };

    class AudioEditor : public Module <Window, Input, Music>
    {
        const int BUFFER=256;      
        SoundEditor _soundEditor;
        bool deleteField=false;
        char file[256]={};
        char soundName[256]={};
        char saveFileName[256]={};
        char loadFileName[256]={};

        bool _showSoundEditor{false};
        bool _showMusicEditor{false};

        bool _addingPlaylist{false};
        bool _addingEffect{false};

        PlaylistHolder _playlistHolder;
        MusicAssets* musicAssets;

        std::string currentEditingMusicFile = "";


    public:
        AudioEditor();
        void render();

        void addSound(const std::string& soundName,const std::string& soundFileName);
        
        void save(const std::string& fileName);
        void load(const std::string& fileName);
    
    };

}