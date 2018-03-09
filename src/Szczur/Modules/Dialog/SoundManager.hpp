#include <SFML/Audio.hpp>

#include <memory>

#include "SoundBase.hpp"

#include <functional>
namespace rat
{
        class SoundManager
        {
        private:

                std::vector<std::shared_ptr<SoundBase>> _sounds;
                float _volumeGame {100};
                int _currentSoundID {0};

        public:

                void newSound(const std::string &fileName);
                void removeSound(const std::string &fileName);

                int getSound(const std::string &fileName);
                int getID(const std::string &fileName = "");

                void changeSound(const std::string &fileName);
                void chooseOption(int option,const std::string &fileName="");
                void addOption(int offset, int newOffset,  const std::string &newfileName="",const std::string &fileName="");

                void play(const std::string &fileName = "");

                void setVolume(float volume, const std::string &fileName = "");
                void setPitch(float pitch, const std::string &fileName = "");

                void setLoop(bool loop,const std::string &fileName);
                void changeLoop(const std::string &fileName);

                void setPlayingOffset(float seconds, const std::string &fileName = "");
                void setCallback(float seconds, const std::function<void(void)>& func);
                void eraseCallbacks();
                void setOffsetID(int offset, const std::string &fileName = "");

                float getOffset(const std::string &fileName = "");
                int getOffsetID(const std::string &fileName = "");

                void addOffset(float beginT,float endT=-1,const std::string &fileName=  "");

                void repeat(const std::string &fileName = "");
                void next(const std::string &fileName = "");
                bool soundEnd(const std::string &fileName = "");

                void restart(const std::string &fileName = "",int offset=0);
                sf::SoundSource::Status update(const std::string &fileName = "");

            private:
        };
}

