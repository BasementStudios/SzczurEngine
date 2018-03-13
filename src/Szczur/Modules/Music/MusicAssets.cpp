#include "MusicAssets.hpp"

namespace rat
{
    void MusicAssets::load(const std::string& fileName) 
	{
        if (!_musicHolder.count(fileName)) {
            if (_musicHolder[fileName].openFromFile(getPath(fileName))) {
                LOG_INFO("Music Assets: ", fileName, " loaded");
            }
            else {
                LOG_INFO("ERROR with loading asset:", getPath(fileName));
            }
        }
        _musicHolder[fileName].incrementCounter();
	}

    void MusicAssets::unload(const std::string& fileName) 
	{
        LOG_INFO("Music Assets: ", fileName, " unloaded");
		_musicHolder.erase(fileName);
    }

    RatMusic& MusicAssets::get(const std::string& fileName)
	{
		return _musicHolder[fileName];
	}

    inline std::string MusicAssets::getPath(const std::string& fileName) const 
	{
		return DEFAULT_PATH + fileName + ".flac"; 
	}
}