#pragma once

#include <list>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/AudioEffects/AudioEffects.hpp"

#include "SoundBase.hpp"

namespace rat 
{
	class Sound : public Module<Script, AudioEffects>
	{ 

		using Status = SoundBase::Status;

    private:

        std::list<SoundBase*> _sounds;
        
        SoundAssets _soundAssets;

	public:

		Sound();
		~Sound();

		void initScript();

		void update();

        void addSound(SoundBase* sound);
        void removeSound(SoundBase* sound);

		void play();
		void pause();
		void stop();

		void setVolume(float volume);
		float getVolume() const;

		SoundBase* addNewSound(const std::string& name);

        SoundAssets& getAssetsManager();

	};

}