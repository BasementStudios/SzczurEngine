#pragma once

#include <string>

#include <SFML/Audio.hpp>

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
            }offset;

            Second_t _length;
            float _volume{100};
            float _pitch;
            std::string _name;
            std::string _fileName;
            sf::SoundBuffer buffer;
            sf::Sound sound;
            Second_t playingTime{0};

        public:

            bool init(const std::string &name,const std::string &filename);

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

            void setOffset(Second_t beginT,Second_t endT);
            Second_t getLength() const;

            const std::string getName() const;

        private:

            bool loadBuffer();
            std::string getPath() const;

        };
}
