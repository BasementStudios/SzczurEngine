#pragma once

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
    };

    class MusicEditor
    {

    private:

        Music& _music;

        bool _addingPlaylist {false};
        bool _addingEffect   {false};

        PlaylistHolder _playlistHolder;
        MusicAssets*   _musicAssets;

        std::string currentEditingMusicFile = "";
        std::string currentPlaylist         = "";

    public:

        MusicEditor(Music& music);

        void render();

        void save();
        void load();

    };

}