#include <string>

#include <iostream>

#include <SFML/Audio.hpp>

#include <functional>

#include <memory>

namespace rat
{
    class SoundManager
    {
    public:

        using Second_t = size_t;

    private:

        struct offset
        {
            Second_t beginTime;
            Second_t endTime;

            struct option
            {
                int offsetNumber{-1};
                std::string _file{""};
            };
            std::vector<option> _options;
        };

        struct callback
        {
            Second_t time;
            std::function<void(void)> cback;
        };

        float _volume {100};
        float _pitch {1};

        Second_t  _offset {0};
        Second_t _pauseTime{0};

        int _length;

        unsigned int _currentOffsetID {0};
        unsigned int _currentCallback {0};

        std::string _name;
        sf::SoundBuffer buffer;
        sf::Sound sound;

        std::vector<std::shared_ptr<offset>> _offsets;
        std::vector<std::shared_ptr<callback>> _callbacks;

    public:

        void skip();

        bool load(const std::string &filename);

        void play();

        float getVolume();
        void setVolume(float volume);
        void setBaseVolume(float volume);

        void setPlayingOffset(Second_t seconds);
        void setOffsetID(int offset);

        void pause();
        void unPause();
        void repeat();
        void next();

        const std::string chooseOption(int option);
        int getOptionOffset(int option);
        void addOption(int offset, int newOffset,  const std::string &fileName="");

        Second_t getOffset();
        int getOffsetID();
        void addOffset(Second_t beginT, Second_t endT);

        void addCallback(Second_t seconds, const std::function<void(void)>& cback);
        void eraseCallbacks();

        bool soundEnd();

        float getPitch() const;
        void setPitch(float pitch);

        bool getLoop() const;
        void setLoop(bool loop);

        sf::SoundSource::Status update();
        void restart(int offset=0);

        const std::string& getName() const;

    private:

        bool loadBuffer();
        std::string getPath() const;

    };
}
