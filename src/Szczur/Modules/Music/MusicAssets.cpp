#include "MusicAssets.hpp"

#include <string>

namespace rat
{
    MusicAssets::~MusicAssets()
    {
        for(auto it : _musicHolder) {
            delete it;
        }
    }

    RatMusic* MusicAssets::load(const std::string& name) 
	{
        _musicHolder.emplace_back(new RatMusic());
        
        if(_musicHolder.back()->load(name)) {
            LOG_INFO("[Music Assets] ", name, " loaded");
            return _musicHolder.back();
        }
        else {
            LOG_INFO("[Music Assets Error] Problem with loading asset: ", name);
            return nullptr;
        }  
	}

    void MusicAssets::unload(RatMusic* music) 
	{
        _musicHolder.remove_if([=](auto it){
            auto same = (it == music);
            if (same) {
                LOG_INFO("[Music Assets] ", music->getName(), " unloaded");
               delete music;
            }
            return same; 
        });
    }
}