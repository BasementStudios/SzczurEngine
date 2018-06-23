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
            Second_t beginTime {0};
            Second_t endTime {0};
        } offset;

        Second_t _length {0};

        std::string _name {""};

        float _volume {100};
        float _pitch {1};

        std::string _fileName {""};

        Second_t playingTime {0};
            
        sf::SoundBuffer buffer;
        RatSound sound;

    public:

        bool init(const std::string& fileName, const std::string& name);

        void play();
        void stop();
        void pause();

        float getVolume() const;
        void setVolume(float volume);

        float getPitch() const;
        void setPitch(float pitch);

        bool isRelativeToListener() const;
        void setRelativeToListener(bool relative);

        float getAttenuation() const; 
        void setAttenuation(float attenuation);

        float getMinDistance() const; 
        void setMinDistance(float minDistance);

        bool getLoop() const;
        void setLoop(bool loop);

        void setOffset(Second_t beginT, Second_t endT);
        Second_t getLength() const;

        Second_t getBeginTime() const;
        Second_t getEndTime() const;  

        const std::string getName() const;
        std::string getFileName() const;

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
