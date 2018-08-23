#include "Listener.hpp"

namespace rat
{

    Listener::Listener()
    {
        initScript();
		LOG_INFO(this, " : Module Listener constructed");
    }

    Listener::~Listener()
    {
        LOG_INFO(this, " : Module Listener destructed");
    }

    void Listener::initScript()
    {
        Script& script = getModule<Script>();
		auto module = script.newModule("Listener");

		module.set_function("setGlobalVolume", &Listener::setGlobalVolume, this);
		module.set_function("getGlobalVolume", &Listener::getGlobalVolume, this);
		module.set_function("getPosition", &Listener::getPosition, this);
        module.set_function("setDirection", &Listener::setDirection, this);
		module.set_function("getDirection", &Listener::getDirection, this);
        module.set_function("setUpVector", &Listener::setUpVector, this);
		module.set_function("getUpVector", &Listener::getUpVector, this);
        module.set_function("getOffsetZ", &Listener::getOffsetZ, this);
        module.set_function("setOffsetZ", &Listener::setOffsetZ, this);

    }

    void Listener::setGlobalVolume(float volume)
    {
        sf::Listener::setGlobalVolume(volume);
    }

    float Listener::getGlobalVolume() const
    {
        return sf::Listener::getGlobalVolume();
    }

    void Listener::setPosition(float x, float y, float z)
    {
        sf::Listener::setPosition(x, y, z + _offsetZ);
    }

    glm::vec3 Listener::getPosition() const
    {
        auto vec = sf::Listener::getPosition();
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    void Listener::setDirection(float x, float y, float z)
    {
        sf::Listener::setDirection(x, y, z);    
    }

    glm::vec3 Listener::getDirection() const
    {
        auto vec = sf::Listener::getDirection();
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    void Listener::setUpVector(float x, float y, float z)
    {
        sf::Listener::setUpVector(x, y, z);
    }

    glm::vec3 Listener::getUpVector() const
    {
        auto vec = sf::Listener::getUpVector();
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    void Listener::setOffsetZ(float offsetZ)
    {
        _offsetZ = offsetZ;
    }

    float Listener::getOffsetZ() const
    {
        return _offsetZ;
    }

}