#include "MusicAssets.hpp"

#include <string>

namespace rat
{

    void MusicAssets::load(const std::string& name) 
	{
        if (!_musicHolder.count(name)) {
            if (_musicHolder[name].load(name)) {
                LOG_INFO("Music Assets: ", name, " loaded");
            }
            else {
                LOG_INFO("[Music Assets Error] Problem with loading asset: ", name);
            }    
        }

        _musicHolder[name].incrementCounter();
	}

    void MusicAssets::unload(const std::string& name) 
	{
        LOG_INFO("Music Assets: ", name, " unloaded");
		_musicHolder.erase(name);
    }

    RatMusic& MusicAssets::get(const std::string& name)
	{
		return _musicHolder[name];
	}

}