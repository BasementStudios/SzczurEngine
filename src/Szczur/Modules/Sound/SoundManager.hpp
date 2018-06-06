#pragma once

#include<memory>

#include <SFML/Audio.hpp>

#include "SoundBase.hpp"

#include <fstream>

namespace rat
{
    class SoundManager
    {
        using Second_t = float;
    private:

        std::vector<std::unique_ptr<SoundBase>> _sounds;
        float _volumeGame=100;

    public:

        bool newSound(const std::string &name,const std::string &fileName);
        void eraseSounds();
        void eraseSingleSound(int i);
        void eraseSingleSound(const std::string &fileName);

        int getSound(const std::string &fileName) const;

        void play();
        void pause();
        void stop();
        void play(const std::string &fileName);
        void pause(const std::string &fileName);
        void stop(const std::string &fileName);

        void setVolume(float volume, const std::string &fileName="");
        void setPitch(float pitch, const std::string &fileName="");
        void setLoop(bool loop,const std::string &fileName);
        void changeLoop(const std::string &fileName);
        void changeLoop();
        bool getLoop(const std::string &fileName);
        int getSize() const;
                
        void setOffset(const std::string &fileNam,Second_t beginT,Second_t endT);
        Second_t getLength(const std::string &fileName) const;
                
        std::string getName(int i) const;

        void load(const std::string &fileName);
    };
}
