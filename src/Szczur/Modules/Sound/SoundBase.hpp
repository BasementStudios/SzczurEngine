#pragma once

#include <string>

#include "SFML/Audio/SoundBuffer.hpp"

#include "RatSound.hpp"

namespace rat
{
    class SoundBase
    {
        using Second_t = float;
    
    private:
            
        struct
        {
            Second_t beginTime;
            Second_t endTime;
        } offset;

        Second_t _length;

        float _volume {100};
        float _pitch;

        std::string _fileName;

        Second_t playingTime {0};
            
        sf::SoundBuffer buffer;
        RatSound sound;

    public:

        bool init(const std::string& fileName);

        void play();
        void stop();
        void pause();

        float getVolume() const;
        void setVolume(float volume);
        void setBaseVolume(float volume);

        float getPitch() const;
        void setPitch(float pitch);

        bool getLoop() const;
        void setLoop(bool loop);

        void setOffset(Second_t beginT, Second_t endT);
        Second_t getLength() const;

        const std::string getName() const;

        template <typename T>
		T& getEffect() {
            return sound.getEffect<T>();
        }

        template <typename T>
    	void cleanEffect() {
            sound.cleanEffect<T>();
        }

    private:

        bool loadBuffer();
        std::string getPath() const;

    };
}
