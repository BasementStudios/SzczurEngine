#pragma once

#include <SFML/Audio/Listener.hpp>
#include <glm/glm.hpp>

#include "Szczur/Modules/Script/Script.hpp"

namespace rat 
{
	class Listener : public Module<Script>
	{ 

        float _offsetZ {0};

    public:

        Listener();
        ~Listener();

        void initScript();

        void setGlobalVolume(float volume);
        float getGlobalVolume() const;

        void setPosition(float x, float y, float z);
        glm::vec3 getPosition() const;

        void setDirection(float x, float y, float z);
        glm::vec3 getDirection() const;

        void setUpVector(float x, float y, float z);
        glm::vec3 getUpVector() const;

        void setOffsetZ(float offsetZ);
        float getOffsetZ() const;

	};
}