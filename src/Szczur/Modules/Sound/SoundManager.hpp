#pragma once

#include<memory>
#include <fstream>

#include <SFML/Audio.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/AudioEffects/AudioEffects.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "SoundBase.hpp"

namespace rat
{
    class SoundManager : public Module<Script, AudioEffects>
    {
        using Second_t = float;
        
    private:

        std::vector<std::unique_ptr<SoundBase>> _sounds;
        float _volumeGame = 100;

    public:

        SoundManager();
        ~SoundManager();

        void initScript();

        bool newSound(const std::string& fileName);

        void eraseSounds();
        void eraseSingleSoundByID(int i);
        void eraseSingleSound(const std::string& fileName);

        int getSoundID(const std::string& fileName) const;

        void play(const std::string& fileName = "");
        void pause(const std::string& fileName = "");
        void stop(const std::string& fileName = "");

        void setVolume(float volume, const std::string& fileName = "");
        float getVolume(const std::string& fileName) const;
        float getGlobalVolume() const;
        
        void setPitch(float pitch, const std::string& fileName = "");
        float getPitch(const std::string& fileName) const;

        void setLoop(bool loop, const std::string& fileName);
        void changeLoop(const std::string& fileName = "");
        bool getLoop(const std::string& fileName);

        int getSize() const;
                
        void setOffset(const std::string& fileName, Second_t beginT, Second_t endT);
        Second_t getLength(const std::string& fileName) const;

        Second_t getBeginTime(const std::string& fileName) const;
        Second_t getEndTime(const std::string& fileName) const;            

        std::string getName(int i) const;

        void load(const std::string& fileName);

        template <typename T>
		T& getEffect(const std::string& fileName) {
            return _sounds[getSoundID(fileName)]->getEffect<T>();
        }

        template <typename T>
    	void cleanEffect(const std::string& fileName) {
            _sounds[getSoundID(fileName)]->cleanEffect<T>();
        }

    };
}
