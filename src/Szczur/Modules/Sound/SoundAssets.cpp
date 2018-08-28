#include "SoundAssets.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

    bool SoundAssets::load(const std::string& fileName) 
	{
        if (!_buffersHolder.count(fileName)) {
            if (_buffersHolder[fileName].loadFromFile(fileName)) {
                _buffersHolder[fileName].setName(fileName);
                LOG_INFO("[Sound Assets] ", fileName, " loaded");
                return true;
            }
            else {
                LOG_INFO("[Sound Assets Error] Problem with loading asset: ", fileName);
                _buffersHolder.erase(fileName);
                return false;
            }    
        }
        return true;
	}

    void SoundAssets::unload(SoundBuffer* buffer) 
	{
        auto fileName = buffer->getName();

        LOG_INFO("[Sound Assets] ", fileName, " unloaded");
		_buffersHolder.erase(fileName);
    }

    SoundBuffer* SoundAssets::get(const std::string& fileName)
	{ 
        if (_buffersHolder.count(fileName)) {
            _buffersHolder[fileName].increment();
		    return &_buffersHolder[fileName];
        }
            
        return nullptr;
	}

}