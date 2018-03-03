#include <string>

#include <SFML/Audio.hpp>

namespace rat
{
        class SoundBase
        {
        private:

            struct offset
            {
                float beginTime;
                float endTime;

                struct option
                {
                    int offsetNumber=-1;
                    std::string _file="";
                };
                std::vector<option> _options;

            };

            float _volume {100};
            float _pitch;
            float  _offset {0};
            int _length;
            unsigned int _currentOffsetID {0};
            std::string _name;
            sf::SoundBuffer buffer;
            sf::Sound sound;

            std::vector<offset> _offsets;

        public:

            bool init(const std::string &filename);

            void play();

            float getVolume();
            void setVolume(float volume);
            void setBaseVolume(float volume);

            void setPlayingOffset(float seconds);
            void setOffset(int offset);
            void repeat();
            void next();
            const std::string chooseOption(int option);
            int getOptionOffset(int option);
            void addOption(int offset, int newOffset,  const std::string &fileName="");

            float getOffset();
            int getOffsetID();
            void addOffset(float beginT, float endT);
            bool soundEnd();

            float getPitch();
            void setPitch(float pitch);

            bool getLoop();
            void setLoop(bool loop);

            sf::SoundSource::Status update();
            void restart(int offset=0);

            const std::string& getName();

        private:

            bool loadBuffer();
            std::string getPath();

        };
}

