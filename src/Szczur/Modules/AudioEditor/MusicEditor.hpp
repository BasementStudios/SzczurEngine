#pragma once

#include <vector>
#include <string>

#include "Szczur/Modules/Music/Music.hpp"

namespace rat
{
    struct PlaylistHolder 
    {
        std::vector<std::string> playlistsNames;
        
        Music* music;

        void savePlaylists();
        void loadPlaylist();
        void addMusic(const std::string& playlistName);

        void addPlaylist(const std::string& name);
        
        std::string newMusicPath {""};
        std::string newMusicPlaylist {""};
        bool addingMusic {false};
    };

    class MusicEditor
    {

    private:

        Music& _music;

        bool _addingPlaylist {false};
        bool _addingEffect   {false};

        PlaylistHolder _playlistHolder;
        MusicAssets*   _musicAssets;

        std::string currentEditingMusicFile {""};
        std::string currentPlaylist         {""};

        bool loadingMusic {false};
        std::string loadingMusicPlaylist {""};

    public:

        MusicEditor(Music& music);

        void render();

        void save();
        void load();

    };

}