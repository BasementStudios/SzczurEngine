#include "MusicAssets.hpp"

#include <string>

namespace rat
{
    MusicAssets::MusicAssets()
    {
        LOG_INFO("Default musics assets path: ", MUSIC_DEFAULT_PATH);
    }

    void MusicAssets::load(const std::string& fileName) 
	{
        if (!_musicHolder.count(fileName)) {
            auto path = getPath(fileName);
            if (_musicHolder[fileName].openFromFile(path)) {
                LOG_INFO("Music Assets: ", path, " loaded");
            }
            else {
                LOG_INFO("Music Assets Error: Problem with loading asset:", path);
            }
        }
        _musicHolder[fileName].init(fileName);
        _musicHolder[fileName].incrementCounter();
	}

    void MusicAssets::unload(const std::string& fileName) 
	{
        LOG_INFO("Music Assets: ", getPath(fileName), " unloaded");
		_musicHolder.erase(fileName);
    }

    RatMusic& MusicAssets::get(const std::string& fileName)
	{
		return _musicHolder[fileName];
	}

    std::string MusicAssets::getPath(const std::string& fileName) const 
	{
		auto path = MUSIC_DEFAULT_PATH + fileName;
        path += ".flac";

        return path;
	}

}