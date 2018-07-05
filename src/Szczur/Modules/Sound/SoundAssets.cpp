#include "SoundAssets.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

    void SoundAssets::load(const std::string& fileName) 
	{
        if (!_buffersHolder.count(fileName)) {
            if (_buffersHolder[fileName].loadFromFile(fileName)) {
                LOG_INFO("Sound Assets: ", fileName, " loaded");
            }
            else {
                LOG_INFO("[Sound Assets Error] Problem with loading asset: ", fileName);
            }    
        }
	}

    void SoundAssets::unload(const std::string& fileName) 
	{
        LOG_INFO("Sound Assets: ", fileName, " unloaded");
		_buffersHolder.erase(fileName);
    }

    sf::SoundBuffer& SoundAssets::get(const std::string& fileName)
	{
		return _buffersHolder[fileName];
	}

}