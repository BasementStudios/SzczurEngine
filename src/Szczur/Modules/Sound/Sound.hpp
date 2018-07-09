#pragma once

#include <list>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/AudioEffects/AudioEffects.hpp"

#include "SoundBase.hpp"
#include "SoundAssets.hpp"

namespace rat 
{
	class Sound : public Module<Script, AudioEffects>
	{ 

    private:

        std::list<SoundBase*> _sounds;
        
        SoundAssets _soundAssets;

	public:

		Sound();
		~Sound();

		void initScript();

        void addSound(SoundBase* sound);
        void removeSound(SoundBase* sound);

		void play();
		void pause();
		void stop();

		void setVolume(float volume);
		float getVolume() const;

        SoundAssets& getAssetsManager();

	};

}