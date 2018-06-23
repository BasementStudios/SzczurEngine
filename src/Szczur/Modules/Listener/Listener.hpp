#pragma once

#include "Szczur/Modules/Script/Script.hpp"

#include <SFML/Audio/Listener.hpp>

namespace rat 
{
	class Listener : public Module<Script>
	{ 
		
    public:

        Listener();
        ~Listener();

        void initScript();

        void setGlobalVolume(float volume);
        float getGlobalVolume();

        void setPosition(float x, float y, float z);
        sf::Vector3f getPosition();

        void setDirection(float x, float y, float z);
        sf::Vector3f getDirection();

        void setUpVector(float x, float y, float z);
        sf::Vector3f getUpVector();

	};
}