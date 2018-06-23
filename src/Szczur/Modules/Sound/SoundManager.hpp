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

        bool newSound(const std::string& fileName, const std::string& name);

        void eraseSounds();
        void eraseSingleSoundByID(int i);
        void eraseSingleSound(const std::string& name);

        int getSoundID(const std::string& name) const;

        void play(const std::string& name = "");
        void pause(const std::string& name = "");
        void stop(const std::string& name = "");

        void setVolume(float volume, const std::string& name = "");
        float getVolume(const std::string& name) const;
        
        void setPitch(float pitch, const std::string& name = "");
        float getPitch(const std::string& name) const;

        void setLoop(bool loop, const std::string& name);
        void changeLoop(const std::string& name = "");
        bool getLoop(const std::string& name);

        int getSize() const;
                
        void setOffset(const std::string& name, Second_t beginT, Second_t endT);
        Second_t getLength(const std::string& name) const;

        Second_t getBeginTime(const std::string& name) const;
        Second_t getEndTime(const std::string& name) const;            

        std::string getName(int i) const;

        void load(const std::string& fileName);

        template <typename T>
		T& getEffect(const std::string& name) {
            return _sounds[getSoundID(name)]->getEffect<T>();
        }

        template <typename T>
    	void cleanEffect(const std::string& name) {
            _sounds[getSoundID(name)]->cleanEffect<T>();
        }

    };
}
